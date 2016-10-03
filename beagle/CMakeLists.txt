cmake_minimum_required(VERSION 2.6)
project(common_misc_beagle)

SET(src beagle_pio.c)

ADD_LIBRARY(${PROJECT_NAME} STATIC ${src})		 

INCLUDE_DIRECTORIES(.)
ADD_EXECUTABLE(beagle_test_pio test/main.cpp)
TARGET_LINK_LIBRARIES(beagle_test_pio ${PROJECT_NAME} )
FILE(GLOB src *.h)    
INSTALL(FILES ${src} DESTINATION include/common/misc/beagle/ ) 

INSTALL_TARGETS(/lib ${PROJECT_NAME})
INSTALL_TARGETS(/bin beagle_test_pio)

