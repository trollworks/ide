cmake_minimum_required(VERSION 3.22.0)
project(trollworks-editor CXX)

file(GLOB_RECURSE EDITOR_SOURCES
  ${PROJECT_SOURCE_DIR}/src/*.cpp
)

file(GLOB_RECURSE EDITOR_HEADERS
  ${PROJECT_SOURCE_DIR}/include/*.hpp
)

add_executable(${PROJECT_NAME}
  ${EDITOR_HEADERS}
  ${EDITOR_SOURCES}
)

if(WIN32)
  target_link_libraries(${PROJECT_NAME}
    mingw32
    SDL2main
    SDL2-static
    SDL2_image
    opengl32
    glu32
    glad
    glm
    imgui
    fmt
    nfd
    efsw
    crossguid
    Lua::Library
  )
  target_link_options(${PROJECT_NAME} PUBLIC
    -Wl,-subsystem,console
  )
elseif(APPLE)
  target_link_libraries(${PROJECT_NAME}
    SDL2-static
    SDL2_image
    opengl
    glu
    glad
    glm
    imgui
    fmt
    nfd
    efsw
    crossguid
    Lua::Library
  )
elseif(UNIX)
  target_link_libraries(${PROJECT_NAME}
    SDL2-static
    SDL2_image
    opengl
    glu
    glad
    glm
    imgui
    fmt
    nfd
    efsw
    crossguid
    Lua::Library
  )
else()
  message(FATAL_ERROR "Platform not supported")
endif()

target_include_directories(${PROJECT_NAME}
  PRIVATE "${PROJECT_SOURCE_DIR}/include"
  # Utils
  PRIVATE "$<TARGET_PROPERTY:fmt,INTERFACE_INCLUDE_DIRECTORIES>"
  PRIVATE "$<TARGET_PROPERTY:nlohmann-json,INTERFACE_INCLUDE_DIRECTORIES>"
  PRIVATE "$<TARGET_PROPERTY:crossguid,INTERFACE_INCLUDE_DIRECTORIES>"
  PRIVATE "$<TARGET_PROPERTY:efsw-static,INTERFACE_INCLUDE_DIRECTORIES>"
  # Graphics
  PRIVATE "$<TARGET_PROPERTY:SDL2-static,INTERFACE_INCLUDE_DIRECTORIES>"
  PRIVATE "$<TARGET_PROPERTY:SDL2_image,INTERFACE_INCLUDE_DIRECTORIES>"
  PRIVATE "$<TARGET_PROPERTY:glad,INTERFACE_INCLUDE_DIRECTORIES>"
  PRIVATE "$<TARGET_PROPERTY:glm_static,INTERFACE_INCLUDE_DIRECTORIES>"
  # UI
  PRIVATE "$<TARGET_PROPERTY:argparse,INTERFACE_INCLUDE_DIRECTORIES>"
  PRIVATE "$<TARGET_PROPERTY:imgui,INTERFACE_INCLUDE_DIRECTORIES>"
  PRIVATE "$<TARGET_PROPERTY:nfd,INTERFACE_INCLUDE_DIRECTORIES>"
  # Scripts
  PRIVATE "$<TARGET_PROPERTY:lua_static,INTERFACE_INCLUDE_DIRECTORIES>"
  # ECS
  PRIVATE "$<TARGET_PROPERTY:entt,INTERFACE_INCLUDE_DIRECTORIES>"
)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  target_compile_definitions(${PROJECT_NAME} PUBLIC
    DEBUG
    _USE_MATH_DEFINES
  )

  target_compile_options(${PROJECT_NAME} PUBLIC
    -g -Wall -std=c++23
  )

  target_link_options(${PROJECT_NAME} PUBLIC -static-libstdc++)
else()
  target_compile_definitions(${PROJECT_NAME} PUBLIC
    _USE_MATH_DEFINES
  )

  target_compile_options(${PROJECT_NAME} PUBLIC
    -O2 -Wall -Werror -std=c++23
  )

  target_link_options(${PROJECT_NAME} PUBLIC -static-libstdc++)
endif()

install(
  TARGETS ${PROJECT_NAME}
  ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}" COMPONENT devel
  LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}" COMPONENT library
  RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}" COMPONENT library
)
