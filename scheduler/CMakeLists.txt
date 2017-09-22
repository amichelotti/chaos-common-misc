cmake_minimum_required(VERSION 2.6)
project(common_misc_scheduler)
file (GLOB src *.cpp)
 INCLUDE_DIRECTORIES(. ../../..)


ADD_LIBRARY(${PROJECT_NAME} STATIC ${src})



INSTALL_TARGETS(/lib  ${PROJECT_NAME})

FILE(GLOB headers "*.h")
INSTALL(FILES ${headers} DESTINATION include/common/misc/scheduler)
