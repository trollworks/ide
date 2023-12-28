cmake_minimum_required(VERSION 3.22.0)
project(nativefiledialog)

if(WIN32)
  set(NFD_SOURCES
    "include/nfd.h"
    "include/nfd/common.h"
    "include/nfd/ftg_core.h"
    "include/nfd/nfd_common.h"
    "src/nfd_common.c"
    "src/nfd_win.cpp"
  )
elseif(APPLE)
  set(NFD_SOURCES
    "include/nfd.h"
    "include/nfd/common.h"
    "include/nfd/ftg_core.h"
    "include/nfd/nfd_common.h"
    "src/nfd_common.c"
    "src/nfd_cocoa.m"
  )
  find_library(APPKIT_LIBRARY AppKit)
elseif(UNIX)
  set(NFD_SOURCES
    "include/nfd.h"
    "include/nfd/common.h"
    "include/nfd/ftg_core.h"
    "include/nfd/nfd_common.h"
    "include/nfd/simple_exec.h"
    "src/nfd_common.c"
    "src/nfd_zenity.cpp"
  )
  find_program(ZENITY zenity REQUIRED)
else()
  message(FATAL_ERROR "Platform not supported")
endif()

add_library(nfd STATIC ${NFD_SOURCES})

if(APPLE)
  target_link_libraries(nfd PRIVATE ${APPKIT_LIBRARY})
endif()

target_compile_definitions(nfd PUBLIC UNICODE _UNICODE)
target_include_directories(nfd PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

install(FILES "LICENSE.txt"
  DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/licenses/${PROJECT_NAME}"
  COMPONENT library
)
