cmake_minimum_required(VERSION 3.22)
project(glm)

add_subdirectory(glm-0.9.9.9)

install(
  DIRECTORY "glm-0.9.9.9/glm"
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
  PATTERN "CMakeLists.txt" EXCLUDE
)

install(
  FILES "glm-0.9.9.9/copying.txt"
  DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/licenses/${PROJECT_NAME}"
)
