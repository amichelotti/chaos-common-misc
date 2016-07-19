cmake_minimum_required(VERSION 2.6)
project(common_misc_data)
ADD_DEFINITIONS(-DDEBUG)
ADD_DEFINITIONS(-fPIC)

ADD_SUBDIRECTORY(cassandra)

IF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
  ADD_SUBDIRECTORY(btf)

  find_library(MEMCACHELIB memcached PATHS ${PROJECT_SOURCE_DIR}/usr/local/lib ${CMAKE_INSTALL_PREFIX}/lib /usr/lib,/usr/local/lib/modbus DOC "memcached library")
  IF(NOT MEMCACHELIB)
    GitClone(libmemcached)
    ConfigureAndBuild(libmemcached "--with-pic --disable-memaslap --disable-sasl --enable-static")
  ENDIF()
ENDIF()

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


