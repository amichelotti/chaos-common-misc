cmake_minimum_required(VERSION 2.6)
project(common_misc_data_cassandra)

find_library(CUSTOMLIB libcassandra_static.a PATHS ${CMAKE_INSTALL_PREFIX}/lib DOC "cassandra client library")
if(NOT CUSTOMLIB)
  message(STATUS "installing cassandra client library")
  file(REMOVE_RECURSE external)
  file(MAKE_DIRECTORY external)
  message(STATUS "cloning cassandra into ${CMAKE_CURRENT_SOURCE_DIR}/external")
  execute_process( 
    COMMAND git clone  https://github.com/datastax/cpp-driver.git
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/external
    RESULT_VARIABLE custom_err
    OUTPUT_VARIABLE out_mod
    )
  if( custom_err)
    message(FATAL_ERROR "cloning ${out_mod} ${custom_err}")
  endif()

  message(STATUS "cloning libuv")
  execute_process( 
    COMMAND git clone https://github.com/libuv/libuv
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/external
    RESULT_VARIABLE custom_err
    OUTPUT_VARIABLE out_mod
    )
  if( custom_err)
    message(FATAL_ERROR "cloning ${out_mod} ${custom_err}")
  endif()


  message(STATUS "generate Makefile libuv")
  execute_process( 
    COMMAND ./autogen.sh
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/external/libuv
    RESULT_VARIABLE custom_err
    OUTPUT_VARIABLE out_mod
    )
  if( custom_err)
    message(FATAL_ERROR "autogen.sh failed ${out_mod} ${custom_err}")
  endif()

  execute_process( 
    COMMAND ./configure  --with-pic --enable-shared=false --enable-static=true --prefix=${CMAKE_INSTALL_PREFIX} ${CROSS_HOST}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/external/libuv
    RESULT_VARIABLE custom_err
    OUTPUT_VARIABLE out_mod
    )
  if( custom_err)
    message(FATAL_ERROR "configuration failed ${out_mod} ${custom_err}")
  endif()

  message(STATUS "compiling libuv")

  execute_process( 
    COMMAND make install
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/external/libuv
    RESULT_VARIABLE custom_err
    OUTPUT_VARIABLE out_mod
    )
  if( custom_err)
    message(FATAL_ERROR "autogen ${out_mod} ${custom_err}")
  endif()

  # execute_process( 
  #   COMMAND cp libuv.a ${CMAKE_INSTALL_PREFIX}/lib
  #   WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/external/libuv
  #   RESULT_VARIABLE custom_err
  #   OUTPUT_VARIABLE out_mod
  #   )
  # if( custom_err)
  #   message(FATAL_ERROR "autogen ${out_mod} ${custom_err}")
  # endif()
  SET(FF "${CMAKE_CXX_FLAGS} -Wno-narrowing -Wno-type-limits")
  message(STATUS "creating makefile cpp-driver configuring to ${CMAKE_INSTALL_PREFIX}")
  execute_process( 
    COMMAND cmake . -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX} -DCMAKE_CXX_FLAGS=${FF} -DCASS_BUILD_STATIC=ON -DCASS_USE_STATIC_LIBS=ON -DCASS_USE_OPENSSL=OFF -DCASS_INCLUDES=${CMAKE_CURRENT_SOURCE_DIR}/external/libuv/include $CHAOS_CMAKE_FLAGS ${CMAKE_CHAOS} .
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/external/cpp-driver
    RESULT_VARIABLE _err
    OUTPUT_VARIABLE out_mod
    )

  if( modbus_err)
    message(FATAL_ERROR "configuring ${out_mod}")
  endif()

  message(STATUS "compiling")
  execute_process( 
    COMMAND make -j 4 install
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/external/cpp-driver
    RESULT_VARIABLE custom_err
    OUTPUT_VARIABLE out_mod
    )

  if( custom_err)
    message(FATAL_ERROR "compiling ${out_mod}")
  endif()


else()
  message(STATUS "using cpp-driver")
endif()

  INCLUDE_DIRECTORIES(../../../../)
SET(src DBCassandra.cpp )

IF(FrameworkLib)
SET(libs  ${FrameworkLib})
ELSE()
SET(libs pthread)
ENDIF()


ADD_LIBRARY(${PROJECT_NAME} STATIC ${src})
TARGET_LINK_LIBRARIES(${PROJECT_NAME} cassandra_static uv common_debug ${libs} boost_log boost_system boost_thread)
FILE(GLOB headers "*.h")
INSTALL(FILES ${headers} DESTINATION include/common/misc/data/cassandra)
INSTALL_TARGETS(/lib ${PROJECT_NAME})
