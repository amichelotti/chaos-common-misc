cmake_minimum_required(VERSION 2.6)
project(common_misc_scheduler)
file (GLOB src *.cpp)
 INCLUDE_DIRECTORIES(. ../../..)

ADD_DEFINITIONS(-fPIC)
ADD_LIBRARY(${PROJECT_NAME} STATIC ${src})



#INSTALL_TARGETS(/lib  ${PROJECT_NAME})
INSTALL(TARGETS ${PROJECT_NAME}
        DESTINATION "lib"
        PERMISSIONS OWNER_WRITE OWNER_READ GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)

FILE(GLOB headers "*.h")
INSTALL(FILES ${headers} DESTINATION include/common/misc/scheduler)
