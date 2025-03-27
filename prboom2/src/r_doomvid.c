#include "r_doomvid.h"

#include "r_main.h"
#include "r_draw.h"

seg_t *curline;
subsector_t *frontsector, *backsector;
fixed_t rw_y1, rw_h1, rw_ystep, rw_hstep;

static void R_dvDrawLineColLoop(seg_t *line, fixed_t d1, fixed_t d2, int x1, int x2, fixed_t u1, fixed_t u2)
{
  int i, x;

  const byte *colormap = colormaps[0];
  byte *dest = drawvars.topleft;

  fixed_t worldheightspan;
  fixed_t alpha, numer, denom;
  fixed_t screenleftheight;
  fixed_t u, v, vstep, alphastep;
  fixed_t y, h;
  int il, ih;
  const rpatch_t *texpatch;
  const byte *source;

  texpatch = R_TextureCompositePatchByNum(line->sidedef->midtexture);
  worldheightspan = line->frontsector->ceilingheight - line->frontsector->floorheight;
  
  alphastep = FixedDiv(FRACUNIT, (x2-x1) << FRACBITS);
  for(x=x1, y=rw_y1, h=rw_h1, alpha=0; x<x2; x++, y+=rw_ystep, h+=rw_hstep, alpha+=alphastep)
  {
    numer = FixedMul(FRACUNIT - alpha, FixedDiv(u1, d1)) + FixedMul(alpha, FixedDiv(u2, d2));
    denom = FixedMul(FRACUNIT - alpha, FixedDiv(FRACUNIT, d1)) + FixedMul(alpha, FixedDiv(FRACUNIT, d2));
    u = FixedDiv(numer, denom);
    v = 0;

    vstep = FixedDiv(worldheightspan, h);

    il = (y+h) >> FRACBITS;
    ih = y >> FRACBITS;

    if(ih < 0)
    {
      v += FixedMul(-y, vstep);
      ih = 0;
    }
    if(il >= SCREENHEIGHT)
      il = SCREENHEIGHT-1;

    source = R_GetTextureColumn(texpatch, u >> FRACBITS);

    for(i=ih; i<=il; i++, v+=vstep)
    {
      dest[i * SCREENWIDTH + x] = colormap[source[v>>FRACBITS]];
    }
  }
}

static void R_dvDrawLine(seg_t *line, int x1, int x2)
{
  int x, y;

  const byte *colormap = colormaps[0];
  byte *dest = drawvars.topleft;
  angle_t a1, a2;
  fixed_t d1, d2, vx, vy;
  fixed_t numer, denom, t1, t2;
  fixed_t p1x, p1y, p2x, p2y;
  fixed_t lineheight, linebottom;
  fixed_t aspect, temp, hplane, vplane;
  fixed_t baseheight;
  angle_t hfov, vfov;
  fixed_t u1, u2;
  fixed_t l1, h1, l2, h2, lstep, hstep, l, h;
  int il, ih;

  vx = finecosine[viewangle >> ANGLETOFINESHIFT];
  vy = finesine[viewangle >> ANGLETOFINESHIFT];

  a1 = xtoviewangle[x1] + viewangle;
  a2 = xtoviewangle[x2] + viewangle;

  numer  = FixedMul(line->v1->px - viewx, -finesine[line->pangle >> ANGLETOFINESHIFT]);
  numer += FixedMul(line->v1->py - viewy, finecosine[line->pangle >> ANGLETOFINESHIFT]);
  
  denom = finesine[(a1 - line->pangle) >> ANGLETOFINESHIFT];
  t1 = FixedDiv(numer, denom);
  denom = finesine[(a2 - line->pangle) >> ANGLETOFINESHIFT];
  t2 = FixedDiv(numer, denom);
  
  p1x = FixedMul(finecosine[a1 >> ANGLETOFINESHIFT], t1);
  p1y = FixedMul(  finesine[a1 >> ANGLETOFINESHIFT], t1);
  p2x = FixedMul(finecosine[a2 >> ANGLETOFINESHIFT], t2);
  p2y = FixedMul(  finesine[a2 >> ANGLETOFINESHIFT], t2);

  u1  = FixedMul(finecosine[line->pangle>>ANGLETOFINESHIFT], p1x+viewx-line->v1->px);
  u1 += FixedMul(  finesine[line->pangle>>ANGLETOFINESHIFT], p1y+viewy-line->v1->py);
  u2  = FixedMul(finecosine[line->pangle>>ANGLETOFINESHIFT], p2x+viewx-line->v1->px);
  u2 += FixedMul(  finesine[line->pangle>>ANGLETOFINESHIFT], p2y+viewy-line->v1->py);

  d1 = FixedMul(p1x, vx) + FixedMul(p1y, vy);
  d2 = FixedMul(p2x, vx) + FixedMul(p2y, vy);

  lineheight = line->frontsector->ceilingheight - line->frontsector->floorheight;
  linebottom = line->frontsector->floorheight - viewz;

  hfov = FieldOfView << ANGLETOFINESHIFT;
  aspect = FixedDiv(SCREENWIDTH << FRACBITS, SCREENHEIGHT << FRACBITS);
  hplane = finetangent[((hfov>>1) + ANG90)>>ANGLETOFINESHIFT] << 1;
  vplane = FixedDiv(hplane, aspect);

  // 1 unit to 1 pixel if the unit is 1 unit away
  baseheight = FixedDiv(SCREENHEIGHT << FRACBITS, vplane);

  l1 = (SCREENHEIGHT >> 1 << FRACBITS) - FixedMul(baseheight, FixedDiv(linebottom, d1));
  l2 = (SCREENHEIGHT >> 1 << FRACBITS) - FixedMul(baseheight, FixedDiv(linebottom, d2));

  h1 = l1 - FixedMul(lineheight, FixedDiv(baseheight, d1));
  h2 = l2 - FixedMul(lineheight, FixedDiv(baseheight, d2));

  lstep = FixedDiv(l2 - l1, (x2 - x1) << FRACBITS);
  hstep = FixedDiv(h2 - h1, (x2 - x1) << FRACBITS);

  rw_y1 = h1;
  rw_h1 = l1 - h1;
  rw_ystep = hstep;
  rw_hstep = FixedDiv((l2 - h2) - (l1 - h1), (x2 - x1) << FRACBITS);

  R_dvDrawLineColLoop(line, d1, d2, x1, x2, u1, u2);
}

static void R_dvAddLine (seg_t *line)
{
  int      x1;
  int      x2;
  angle_t  angle1;
  angle_t  angle2;
  angle_t  span;
  angle_t  tspan;
  static sector_t tempsec;     // killough 3/8/98: ceiling/water hack

  #if 0
    if(line->linedef - lines != 46)
      return;
  #endif

  curline = line;
  backsector = line->backsector;
  frontsector = line->frontsector;

  angle1 = R_PointToAngleEx(line->v1->px, line->v1->py);
  angle2 = R_PointToAngleEx(line->v2->px, line->v2->py);

  // Clip to view edges.
  span = angle1 - angle2;

  // Back side, i.e. backface culling
  if (span >= ANG180)
    return;

  // Global angle needed by segcalc.
  angle1 -= viewangle;
  angle2 -= viewangle;

  tspan = angle1 + clipangle;
  if (tspan > 2*clipangle)
    {
      tspan -= 2*clipangle;

      // Totally off the left edge?
      if (tspan >= span)
        return;

      angle1 = clipangle;
    }

  tspan = clipangle - angle2;
  if (tspan > 2*clipangle)
    {
      tspan -= 2*clipangle;

      // Totally off the left edge?
      if (tspan >= span)
        return;
      angle2 = 0-clipangle;
    }

    // The seg is in the view range,
    // but not necessarily visible.

    angle1 = (angle1+ANG90)>>ANGLETOFINESHIFT;
    angle2 = (angle2+ANG90)>>ANGLETOFINESHIFT;

    x1 = viewangletox[angle1];
    x2 = viewangletox[angle2];

    if (x1 >= x2)
        return;

    if(line->sidedef->midtexture)
        R_dvDrawLine(line, x1, x2);
}


void r_dvRenderSubsector(subsector_t* sub)
{
    int i;

    for(i=0; i<sub->numlines; i++)
        R_dvAddLine(&segs[sub->firstline+i]);
}