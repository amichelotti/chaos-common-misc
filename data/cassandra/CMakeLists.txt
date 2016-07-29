cmake_minimum_required(VERSION 2.8)
project(common_misc_data_cassandra)
CheckConfigureBuild(uv libuv "--with-pic;--enable-shared=false;--enable-static=true" https://github.com/libuv)
CheckConfigureBuild(cassandra_static cpp-driver "-DCASS_BUILD_STATIC=ON;-DCASS_USE_STATIC_LIBS=ON;-DCASS_USE_OPENSSL=OFF;-DCASS_INCLUDES=${CMAKE_CURRENT_SOURCE_DIR}/external/libuv/include" https://github.com/datastax)

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
