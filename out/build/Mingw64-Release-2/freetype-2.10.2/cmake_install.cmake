# Install script for directory: C:/Users/Chris/source/repos/MagicGUI/freetype-2.10.2

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Users/Chris/source/repos/MagicGUI/out/build/Mingw64-Release-2")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Debug")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xheadersx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/freetype2" TYPE DIRECTORY FILES "C:/Users/Chris/source/repos/MagicGUI/freetype-2.10.2/include/" REGEX "/internal$" EXCLUDE REGEX "/ftconfig\\.h$" EXCLUDE REGEX "/ftoption\\.h$" EXCLUDE)
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xheadersx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/freetype2/freetype/config" TYPE FILE FILES
    "C:/Users/Chris/source/repos/MagicGUI/out/build/Mingw64-Release-2/freetype-2.10.2/include/freetype/config/ftconfig.h"
    "C:/Users/Chris/source/repos/MagicGUI/out/build/Mingw64-Release-2/freetype-2.10.2/include/freetype/config/ftoption.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "C:/Users/Chris/source/repos/MagicGUI/out/build/Mingw64-Release-2/freetype-2.10.2/libfreetyped.a")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xheadersx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/freetype/freetype-config.cmake")
    file(DIFFERENT EXPORT_FILE_CHANGED FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/freetype/freetype-config.cmake"
         "C:/Users/Chris/source/repos/MagicGUI/out/build/Mingw64-Release-2/freetype-2.10.2/CMakeFiles/Export/lib/cmake/freetype/freetype-config.cmake")
    if(EXPORT_FILE_CHANGED)
      file(GLOB OLD_CONFIG_FILES "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/freetype/freetype-config-*.cmake")
      if(OLD_CONFIG_FILES)
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/freetype/freetype-config.cmake\" will be replaced.  Removing files [${OLD_CONFIG_FILES}].")
        file(REMOVE ${OLD_CONFIG_FILES})
      endif()
    endif()
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/freetype" TYPE FILE FILES "C:/Users/Chris/source/repos/MagicGUI/out/build/Mingw64-Release-2/freetype-2.10.2/CMakeFiles/Export/lib/cmake/freetype/freetype-config.cmake")
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/freetype" TYPE FILE FILES "C:/Users/Chris/source/repos/MagicGUI/out/build/Mingw64-Release-2/freetype-2.10.2/CMakeFiles/Export/lib/cmake/freetype/freetype-config-debug.cmake")
  endif()
endif()

