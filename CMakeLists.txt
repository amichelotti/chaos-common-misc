cmake_minimum_required(VERSION 2.6)
project(common_misc)
IF (DEFINED ENV{CHAOS_BUNDLE} )
  include($ENV{CHAOS_BUNDLE}/tools/project_template/CMakeChaos.txt)

ELSE()
  function (GitClone arg)
  message(STATUS "Cloning ${arg}")
  file(REMOVE_RECURSE external)
  file(MAKE_DIRECTORY external)
  execute_process( 
    COMMAND git clone https://github.com/amichelotti/${arg}.git 
    WORKING_DIRECTORY external
    RESULT_VARIABLE modbus_err
    OUTPUT_VARIABLE out_mod
    )
  if( modbus_err)
    message(FATAL_ERROR "cloning ${arg} ${modbus_err}")
  endif()
endfunction()

  MESSAGE(STATUS "STANDALONE, environment CHAOS_BUNDLE defined")
  IF (NOT CMAKE_INSTALL_PREFIX) 
    MESSAGE(fatal "you must add -DCMAKE_INSTALL_PREFIX=<dir>")
  ELSE()
    MESSAGE(STATUS "install prefix ${CMAKE_INSTALL_PREFIX}")
  ENDIF()
  INCLUDE_DIRECTORIES(. ../..)
  LINK_DIRECTORIES(${CMAKE_BINARY_DIR}/build ${CMAKE_INSTALL_PREFIX}/lib)
ENDIF()
ADD_DEFINITIONS(-std=c++11)

ADD_SUBDIRECTORY(wavegenerators)
ADD_SUBDIRECTORY(data)
ADD_SUBDIRECTORY(beagle)


# install(DIRECTORY DESTINATION include/driver/daq)  


