/* Emacs style mode select   -*- C -*-
 *-----------------------------------------------------------------------------
 *
 *
 *  PrBoom: a Doom port merged with LxDoom and LSDLDoom
 *  based on BOOM, a modified and improved DOOM engine
 *  Copyright (C) 1999 by
 *  id Software, Chi Hoang, Lee Killough, Jim Flynn, Rand Phares, Ty Halderman
 *  Copyright (C) 1999-2000 by
 *  Jess Haas, Nicolas Kalkhof, Colin Phipps, Florian Schulze
 *  Copyright 2005, 2006 by
 *  Florian Schulze, Colin Phipps, Neil Stevens, Andrey Budko
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  as published by the Free Software Foundation; either version 2
 *  of the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
 *  02111-1307, USA.
 *
 * DESCRIPTION:
 *  DoomDef - basic defines for DOOM, e.g. Version, game mode
 *   and skill level, and display parameters.
 *
 *-----------------------------------------------------------------------------
 */

#include "doomdef.h"

// Location for any defines turned variables.
// None.

// proff 08/17/98: Changed for high-res
int SCREENWIDTH=320;
int SCREENHEIGHT=200;
int ACTUALHEIGHT=240;
int SCREENPITCH=320;

// e6y: wide-res
int SCREEN_320x200;
int WIDE_SCREENWIDTH = 320;
int WIDE_SCREENHEIGHT = 200;

/*
 * ANALYSIS ADDITIONS
*/

dboolean clearscreen = false;
dboolean doplanes = true;
dboolean domasked = true;
dboolean dofog = true;
dboolean dolight = true;;