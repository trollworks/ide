cmake_minimum_required(VERSION 3.22.0)
project(efsw)

set(BUILD_TEST_APP OFF)
set(EFSW_INSTALL OFF)

add_subdirectory(efsw-1.0.2)

install(
  FILES "efsw-1.0.2/LICENSE"
  DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/licenses/${PROJECT_NAME}"
  COMPONENT library
)
