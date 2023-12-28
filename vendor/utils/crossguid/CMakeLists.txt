cmake_minimum_required(VERSION 3.22.0)
project(crossguid)

set(CROSSGUID_TESTS OFF)

add_subdirectory(crossguid-0.2.3)

install(
  FILES "crossguid-0.2.3/LICENSE"
  DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/licenses/${PROJECT_NAME}"
  COMPONENT library
)
