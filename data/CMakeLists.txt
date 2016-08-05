cmake_minimum_required(VERSION 2.8)
project(common_misc_data)
ADD_SUBDIRECTORY(cassandra)
ADD_DEFINITIONS(-fPIC -std=c++11 )
SET(CHAOS_CXX_FLAGS "${CHAOS_CXX_FLAGS} -Wno-shorten-64-to-32 -Wno-error")
SET(CHAOS_C_FLAGS "${CHAOS_C_FLAGS} -Wno-shorten-64-to-32 -Wno-error")

# IF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
CheckConfigureBuild(memcached libmemcached "--with-pic;--disable-memaslap;--disable-sasl;--enable-static" https://github.com/amichelotti)
  ADD_SUBDIRECTORY(btf)

# ENDIF()

file (GLOB src core/DataSet.cpp core/DBbase.cpp core/DBbaseFactory.cpp)


ADD_LIBRARY(${PROJECT_NAME} STATIC ${src})


TARGET_LINK_LIBRARIES(${PROJECT_NAME} common_misc_data_cassandra jsoncpp common_debug boost_date_time)
INSTALL_TARGETS(/lib  ${PROJECT_NAME})

FILE(GLOB headers "*.h")
INSTALL(FILES ${headers} DESTINATION include/common/misc/data)


ADD_EXECUTABLE(test_db test/test_db.cpp)

TARGET_LINK_LIBRARIES(test_db ${PROJECT_NAME} common_debug ${libs} boost_program_options boost_system boost_thread)
INSTALL_TARGETS(/bin test_db)

# install(DIRECTORY DESTINATION include/driver/daq)  


