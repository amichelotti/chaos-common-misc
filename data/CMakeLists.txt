cmake_minimum_required(VERSION 2.8)
project(common_misc_data)
option(CASSANDRA_DRIVER "enable support for cassandra" OFF)

IF(CASSANDRA_DRIVER)
  MESSAGE(STATUS "ENABLING CASSANDRA DRIVER")
  ADD_DEFINITIONS(-DCASSANDRA_DRIVER )
  ADD_SUBDIRECTORY(cassandra)
ENDIF()
ADD_DEFINITIONS(-fPIC -std=c++11 )
SET(CHAOS_CXX_FLAGS "${CHAOS_CXX_FLAGS} -Wno-shorten-64-to-32 -Wno-error")
SET(CHAOS_C_FLAGS "${CHAOS_C_FLAGS} -Wno-shorten-64-to-32 -Wno-error")

IF(ENABLE_MEMCACHE)
CheckConfigureBuild(memcached libmemcached "--with-pic;--disable-memaslap;--disable-sasl;--enable-static" https://github.com/amichelotti)
  ADD_SUBDIRECTORY(btf)

ENDIF()


IF(GPUFIT)
ExternalProject_Add(
  GpuFit
  GIT_REPOSITORY https://github.com/gpufit/Gpufit.git
  GIT_TAG master
  PREFIX "${CMAKE_BINARY_DIR}/ext_dep/gpufit"
  SOURCE_DIR "${CMAKE_BINARY_DIR}/ext_dep/gpufit-src"
  BINARY_DIR "${CMAKE_BINARY_DIR}/ext_dep/gpufit-src"
  CMAKE_ARGS  -DBUILD_SHARED_LIBS=ON
                -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_INSTALL_PREFIX}
  LOG_DOWNLOAD ON
  LOG_CONFIGURE ON
  LOG_BUILD ON)
  # add definition for compilation
add_definitions(-DGPUFIT)
MESSAGE(STATUS "Enabling GPUFIT")
ENDIF()

file (GLOB src core/DataSet.cpp core/DBbase.cpp core/DBbaseFactory.cpp core/Property.cpp core/CacheString.cpp core/SharedMem.cpp)


ADD_LIBRARY(${PROJECT_NAME} STATIC ${src})


TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${DRIVER_LIBS} jsoncpp common_debug boost_date_time)
#INSTALL_TARGETS(/lib  ${PROJECT_NAME})
INSTALL(TARGETS ${PROJECT_NAME}
        DESTINATION "lib"
        PERMISSIONS OWNER_WRITE OWNER_READ GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)


FILE(GLOB headers "core/*.h")
INSTALL(FILES ${headers} DESTINATION include/common/misc/data/core)
FILE(GLOB headers "*.h")
INSTALL(FILES ${headers} DESTINATION include/common/misc/data)

IF(COMMON_TEST)
ADD_EXECUTABLE(test_db test/test_db.cpp)

TARGET_LINK_LIBRARIES(test_db ${PROJECT_NAME} common_debug boost_program_options boost_system boost_thread ${FrameworkLib} pthread)
#INSTALL_TARGETS(/bin )
INSTALL(TARGETS test_db
        DESTINATION "bin"
        PERMISSIONS OWNER_WRITE OWNER_READ GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
 install(DIRECTORY DESTINATION include/driver/daq)  

ENDIF()
