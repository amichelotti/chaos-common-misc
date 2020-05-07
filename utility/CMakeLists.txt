cmake_minimum_required(VERSION 2.8.9)
project(common_misc_utility)
include_directories({$chaos_INCLUDE_DIRS})
SET(httpsender_lib_src   HttpSender.cpp)
IF (BUILD_FORCE_STATIC)
ADD_LIBRARY(${PROJECT_NAME} STATIC ${httpsender_lib_src})
ELSE()
ADD_LIBRARY(${PROJECT_NAME} SHARED ${httpsender_lib_src})
ENDIF()
TARGET_LINK_LIBRARIES(${PROJECT_NAME} boost_system)
FILE(GLOB files "HttpSender.h")


INSTALL(TARGETS ${PROJECT_NAME}
        DESTINATION "lib"
        PERMISSIONS OWNER_WRITE OWNER_READ OWNER_EXECUTE GROUP_READ GROUP_WRITE GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
INSTALL(FILES ${files} DESTINATION include/common/misc/utility)
