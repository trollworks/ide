cmake_minimum_required(VERSION 3.22.0)
project(fmt)

set(FMT_DOC OFF)
set(FMT_TEST OFF)
set(FMT_INSTALL ON)

add_subdirectory(fmt-10.1.1)

install(
  FILES "fmt-10.1.1/LICENSE.rst"
  DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/licenses/${PROJECT_NAME}"
  COMPONENT library
)
