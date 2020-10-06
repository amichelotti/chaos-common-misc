cmake_minimum_required(VERSION 2.6)
project(common_misc_data_btf)
ADD_DEFINITIONS( -fno-rtti -fno-exceptions)
SET(src dump_btf_data.cpp )

IF(FrameworkLib)
SET(libs  ${FrameworkLib})
ELSE()
SET(libs pthread)
ENDIF()

ADD_EXECUTABLE(dump_btf_data ${src})
TARGET_LINK_LIBRARIES(dump_btf_data memcached m)
#FILE(GLOB headers "core/*.h")
#INSTALL(FILES ${headers} DESTINATION include/common/core/debug)

INSTALL(TARGETS dump_btf_data 
        DESTINATION "bin"
        PERMISSIONS OWNER_WRITE OWNER_READ OWNER_EXECUTE GROUP_READ GROUP_WRITE GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)