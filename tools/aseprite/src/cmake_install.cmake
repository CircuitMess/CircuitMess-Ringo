# Install script for directory: C:/aseprite/src

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files (x86)/aseprite")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "RelWithDebInfo")
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

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "C:/aseprite/build/bin/aseprite.exe")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/aseprite" TYPE DIRECTORY FILES "C:/aseprite/src/../data")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("C:/aseprite/build/src/observable/cmake_install.cmake")
  include("C:/aseprite/build/src/clip/cmake_install.cmake")
  include("C:/aseprite/build/src/undo/cmake_install.cmake")
  include("C:/aseprite/build/src/cfg/cmake_install.cmake")
  include("C:/aseprite/build/src/doc/cmake_install.cmake")
  include("C:/aseprite/build/src/filters/cmake_install.cmake")
  include("C:/aseprite/build/src/fixmath/cmake_install.cmake")
  include("C:/aseprite/build/src/flic/cmake_install.cmake")
  include("C:/aseprite/build/src/gen/cmake_install.cmake")
  include("C:/aseprite/build/src/gfx/cmake_install.cmake")
  include("C:/aseprite/build/src/net/cmake_install.cmake")
  include("C:/aseprite/build/src/render/cmake_install.cmake")
  include("C:/aseprite/build/src/dio/cmake_install.cmake")
  include("C:/aseprite/build/src/ft/cmake_install.cmake")
  include("C:/aseprite/build/src/she/cmake_install.cmake")
  include("C:/aseprite/build/src/ui/cmake_install.cmake")
  include("C:/aseprite/build/src/script/cmake_install.cmake")
  include("C:/aseprite/build/src/updater/cmake_install.cmake")
  include("C:/aseprite/build/src/app/cmake_install.cmake")

endif()

