cmake_minimum_required(VERSION 2.6)
project(common_misc_beagle)
include($ENV{CHAOS_BUNDLE}/tools/project_template/CMakeChaos.txt)

SET(src beagle_pio.c)

IF(BUILD_FORCE_STATIC)
  SET(CMAKE_EXE_LINKER_FLAGS "-static -Wl,--whole-archive -lchaos_common -Wl,--no-whole-archive")
   ADD_LIBRARY(${PROJECT_NAME} STATIC ${src})		 

ELSE()
    ADD_LIBRARY(${PROJECT_NAME} SHARED ${src})		 

ENDIF()
SET(DRIVERS_LIB ${DRIVERS_LIB} ${PROJECT_NAME} PARENT_SCOPE)
TARGET_LINK_LIBRARIES(${PROJECT_NAME}  ${FrameworkLib})

FILE(GLOB src *.h)    
INSTALL(FILES ${src} DESTINATION include/common/misc/beagle/ ) 

INSTALL_TARGETS(/lib ${PROJECT_NAME})

