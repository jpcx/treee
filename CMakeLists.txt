# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
#    _
#   | |_ _ __ ___  ___  ___   treee: interactive file tree viewer
#   | __| '__/ _ \/ _ \/ _ \  Copyright (C) 2020 Justin Collier
#   | |_| | |  __/  __/  __/
#    \__|_|  \___|\___|\___|  - - - - - - - - - - - - - - - - - -
# 
#     This program is free software: you can redistribute it and/or modify
#     it under the terms of the GNU General Public License as published by
#     the Free Software Foundation, either version 3 of the License, or
#     (at your option) any later version.
# 
#     This program is distributed in the hope that it will be useful,
#     but WITHOUT ANY WARRANTY; without even the internalied warranty of
#     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#     GNU General Public License for more details.
#
#   You should have received a copy of the GNU General Public License
#   along with this program.  If not, see <https://www.gnu.org/licenses/>.
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

cmake_minimum_required(VERSION 3.0)
project(treee
        VERSION 0.2.1
        DESCRIPTION "an interactive file tree viewer"
        HOMEPAGE_URL "https://github.com/jpcx/treee"
        LANGUAGES CXX)

set(CLANG_WARNINGS
    -Wall
    -Weverything
    # -Werror
    -Wno-c++98-c++11-compat
    -Wno-c++98-compat
    -Wno-c++98-compat-pedantic
    -Wno-missing-variable-declarations
    -Wno-global-constructors
    -Wno-exit-time-destructors
    -Wno-documentation-unknown-command)

set(GNU_WARNINGS
    -Wall
    -Wextra
    # -Werror
    -pedantic)

set(CLANG_CXX_FLAGS
    # -g
    -O3
    -std=c++17)

set(GNU_CXX_FLAGS
    # -g
    -O3
    -std=c++17)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
     ${PROJECT_SOURCE_DIR}/src/**.cc)

find_library(LIBGIT2 NAMES git2)
find_library(NCURSES NAMES ncurses)
find_library(PTHREAD NAMES pthread)
find_library(TINFO   NAMES tinfo)

if(NOT TREEE_NO_GIT AND ${LIBGIT2} STREQUAL "LIBGIT2-NOTFOUND")
  message("libgit2 not found")
  message("please install libgit2 or configure CMake with \"cmake .. -DTREEE_NO_GIT=ON\"")
endif()
if(${NCURSES} STREQUAL "NCURSES-NOTFOUND")
  message("ncurses not found")
  message("please install ncurses to build treee")
endif()
if(${PTHREAD} STREQUAL "PTHREAD-NOTFOUND")
  message("pthread not found")
  message("please install pthread to build treee")
endif()
if(${TINFO} STREQUAL "TINFO-NOTFOUND")
  message("tinfo not found")
  message("please install tinfo to build treee")
endif()

add_executable(${PROJECT_NAME} ${SOURCES})

if (TREEE_NO_GIT)
  target_compile_definitions(${PROJECT_NAME} PRIVATE TREEE_NO_GIT)
else()
  target_link_libraries(${PROJECT_NAME} ${LIBGIT2})
endif()
target_link_libraries(${PROJECT_NAME} ${NCURSES} ${PTHREAD} ${TINFO})

target_compile_options(${PROJECT_NAME} PRIVATE
                       ${CMAKE_CXX_FLAGS}
                       $<$<CXX_COMPILER_ID:Clang>:${CLANG_CXX_FLAGS}>
                       $<$<CXX_COMPILER_ID:GNU>:${GNU_CXX_FLAGS}>
                       $<$<CXX_COMPILER_ID:Clang>:${CLANG_WARNINGS}>
                       $<$<CXX_COMPILER_ID:GNU>:${GNU_WARNINGS}>)

target_include_directories(${PROJECT_NAME} PRIVATE
                           ${PROJECT_SOURCE_DIR}/src
                           ${PROJECT_SOURCE_DIR}/ext/ccutl/include)

install(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION bin)
