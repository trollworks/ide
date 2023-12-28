cmake_minimum_required(VERSION 3.22)
project(lua LANGUAGES C VERSION 5.4.6)

option(LUA_SUPPORT_DL "Support dynamic loading of compiled modules" OFF)
option(LUA_BUILD_AS_CXX "Build lua as C++" ON)
option(LUA_ENABLE_SHARED "Build dynamic liblua" OFF)
option(LUA_BUILD_BINARY "Build lua binary" OFF)
option(LUA_BUILD_COMPILER "Build luac compiler" OFF)

enable_language(CXX)
add_subdirectory(lua-${PROJECT_VERSION})

install(
  FILES "lua-${PROJECT_VERSION}/LICENSE.txt"
  DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/licenses/${PROJECT_NAME}"
  COMPONENT library
)
