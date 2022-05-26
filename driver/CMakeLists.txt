cmake_minimum_required(VERSION 2.9)
project(common_misc_driver)
FILE(GLOB headers "*.h")
INSTALL(FILES ${headers} DESTINATION include/common/misc/driver)
#file (GLOB src *.cpp)
#INCLUDE_DIRECTORIES(. ../../..)

#ADD_LIBRARY(${PROJECT_NAME} STATIC ${src})
#ADD_EXECUTABLE(channel_test_loop_server test/channel_test_loop_server.cpp)
#ADD_EXECUTABLE(channel_test_loop_client test/channel_test_loop_client.cpp)
#TARGET_LINK_LIBRARIES(${PROJECT_NAME}  common_serial boost_system)
#TARGET_LINK_LIBRARIES(channel_test_loop_server ${PROJECT_NAME} )
#TARGET_LINK_LIBRARIES(channel_test_loop_client ${PROJECT_NAME} )

#INSTALL_TARGETS(/lib  ${PROJECT_NAME})

