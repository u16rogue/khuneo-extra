cmake_minimum_required(VERSION 3.18)

# Extra utility libraries aka bloat
project(kh_extra)

#TODO: separate it by options depending on available targets

add_library(
  kh_extra
  STATIC
  "include/kh-extra/lexer.h"
  "src/lexer.c"
)

target_include_directories(
  kh_extra
  PUBLIC
  "include"
)

set_target_properties(
  kh_extra
  PROPERTIES
  C_STANDARD 17
)

target_link_libraries(
  kh_extra
  PRIVATE
  kh_core
  kh_astgen
)

# if (TARGET kh_astgen)
# endif()
