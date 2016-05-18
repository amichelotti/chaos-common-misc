cmake_minimum_required(VERSION 2.6)
project(common_misc_beagle)
IF (DEFINED ENV{CHAOS_BUNDLE} )
  include($ENV{CHAOS_BUNDLE}/tools/project_template/CMakeChaos.txt)
ELSE()
  MESSAGE(STATUS "STANDALONE, environment CHAOS_BUNDLE defined")
  IF (NOT CMAKE_INSTALL_PREFIX) 
    MESSAGE(fatal "you must add -DCMAKE_INSTALL_PREFIX=<dir>")
  ELSE()
    MESSAGE(STATUS "install prefix ${CMAKE_INSTALL_PREFIX}")
  ENDIF()
  INCLUDE_DIRECTORIES(. ../..)
  LINK_DIRECTORIES(${CMAKE_BINARY_DIR}/build ${CMAKE_INSTALL_PREFIX}/lib)
ENDIF()



SET(src beagle_pio.c)

IF(BUILD_FORCE_STATIC)
   ADD_LIBRARY(${PROJECT_NAME} STATIC ${src})		 

ELSE()
    ADD_LIBRARY(${PROJECT_NAME} SHARED ${src})		 

ENDIF()

ADD_EXECUTABLE(beagle_test_pio test/main.cpp)
TARGET_LINK_LIBRARIES(beagle_test_pio ${PROJECT_NAME} )
FILE(GLOB src *.h)    
INSTALL(FILES ${src} DESTINATION include/common/misc/beagle/ ) 

INSTALL_TARGETS(/lib ${PROJECT_NAME})
INSTALL_TARGETS(/bin beagle_test_pio)

