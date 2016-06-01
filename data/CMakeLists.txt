cmake_minimum_required(VERSION 2.6)
project(common_misc_data)
ADD_DEFINITIONS(-DDEBUG)
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
ADD_DEFINITIONS(-fPIC)

ADD_SUBDIRECTORY(cassandra)
ADD_SUBDIRECTORY(btf)


file (GLOB src core/DataSet.cpp core/DBbase.cpp core/DBbaseFactory.cpp)


IF(BUILD_FORCE_STATIC)
ADD_LIBRARY(${PROJECT_NAME} STATIC ${src})
ELSE()
ADD_LIBRARY(${PROJECT_NAME} SHARED ${src})
ENDIF()
TARGET_LINK_LIBRARIES(${PROJECT_NAME} common_misc_data_cassandra cassandra_static jsoncpp common_debug boost_date_time)
INSTALL_TARGETS(/lib  ${PROJECT_NAME})

FILE(GLOB headers "*.h")
INSTALL(FILES ${headers} DESTINATION include/common/misc/data)


ADD_EXECUTABLE(test_db test/test_db.cpp)

TARGET_LINK_LIBRARIES(test_db ${PROJECT_NAME} common_debug ${libs} boost_program_options boost_system boost_thread)
INSTALL_TARGETS(/bin test_db)

# install(DIRECTORY DESTINATION include/driver/daq)  


