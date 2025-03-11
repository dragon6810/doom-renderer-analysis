            set(CPACK_OUTPUT_DIR "${CPACK_BUILD_DIR}/${CPACK_PACKAGE_FILE_NAME}")
            file(MAKE_DIRECTORY ${CPACK_OUTPUT_DIR})

            find_program(DYLIBBUNDLER_EXECUTABLE NAMES dylibbundler)
            if(NOT DYLIBBUNDLER_EXECUTABLE)
                message(FATAL_ERROR "Missing dylibbundler (brew install dylibbundler)")
            endif()

            file(COPY_FILE
                /Users/henrydunn/Documents/GitHub/doom-renderer-analysis/prboom2/build/dsda-doom
                ${CPACK_OUTPUT_DIR}/dsda-doom)

            execute_process(COMMAND
                ${CMAKE_COMMAND} -E env
                OUTPUT=${CPACK_PACKAGE_FILE_NAME}.zip
                VERSION=${CPACK_PACKAGE_VERSION}
                ${DYLIBBUNDLER_EXECUTABLE}
                --bundle-deps
                --create-dir
                --fix-file ${CPACK_OUTPUT_DIR}/dsda-doom
                --install-path @executable_path/libs_${CPACK_SYSTEM_PROCESSOR}
                --dest-dir ${CPACK_OUTPUT_DIR}/libs_${CPACK_SYSTEM_PROCESSOR})

            file(COPY_FILE
                ${CPACK_TEMPORARY_DIRECTORY}/${CPACK_PACKAGING_INSTALL_PREFIX}/share/games/doom/dsda-doom.wad
                ${CPACK_OUTPUT_DIR}/dsda-doom.wad)
            file(COPY_FILE
                ${CPACK_TEMPORARY_DIRECTORY}/${CPACK_PACKAGING_INSTALL_PREFIX}/share/doc/dsda-doom/COPYING
                ${CPACK_OUTPUT_DIR}/COPYING.txt)

            file(CONFIGURE
                OUTPUT ${CPACK_OUTPUT_DIR}/Troubleshooting.txt
                CONTENT "If you are getting errors like 'libzip.5.5.dylib cant be opened because Apple cannot check it for malicious software.' Run the following command in the dsda-doom folder:\n\nxattr -dr com.apple.quarantine path/to/folder")

            execute_process(COMMAND zip -r ${CPACK_PACKAGE_FILE_NAME}.zip ${CPACK_PACKAGE_FILE_NAME})
            