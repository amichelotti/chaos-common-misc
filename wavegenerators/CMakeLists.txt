cmake_minimum_required(VERSION 2.6)
IF (DEFINED ENV{CHAOS_BUNDLE} )
  include($ENV{CHAOS_BUNDLE}/tools/project_template/CMakeChaos.txt)
ELSE()
  MESSAGE(STATUS "STANDALONE, environment CHAOS_BUNDLE defined")
  IF (NOT CMAKE_INSTALL_PREFIX) 
    MESSAGE(fatal "you must add -DCMAKE_INSTALL_PREFIX=<dir>")
  ELSE()
    MESSAGE(STATUS "install prefix ${CMAKE_INSTALL_PREFIX}")
  ENDIF()
  INCLUDE_DIRECTORIES(.  ${CMAKE_INSTALL_PREFIX}/include)
  LINK_DIRECTORIES(${CMAKE_BINARY_DIR}/build ${CMAKE_INSTALL_PREFIX}/lib)
ENDIF()

if(DEFINED ENV{CHAOS_CROSS_HOST})
SET(CROSS "--host=$ENV{CHAOS_CROSS_HOST}")
message(status "CROSS host configure params:${CHAOS_CROSS_HOST}")
else()
SET(CROSS "")
endif()

file (GLOB src *.cpp)



IF(BUILD_FORCE_STATIC)
ADD_LIBRARY(common_misc_wavegenerators STATIC ${src})
ELSE()
ADD_LIBRARY(common_misc_wavegenerators SHARED ${src})
ENDIF()


ADD_EXECUTABLE(wave_test test/main.cpp)
TARGET_LINK_LIBRARIES(wave_test common_misc_wavegenerators boost_program_options boost_regex)
INSTALL_TARGETS(/lib  common_misc_wavegenerators)
INSTALL_TARGETS(/bin  wave_test)

FILE(GLOB headers "*.h")
INSTALL(FILES ${headers} DESTINATION include/common/misc/wavegenerators)
