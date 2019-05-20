cmake_minimum_required(VERSION 2.6)
file (GLOB src *.cpp)
 INCLUDE_DIRECTORIES(. ../../..)


ADD_LIBRARY(common_misc_wavegenerators STATIC ${src})


#ADD_EXECUTABLE(wave_test test/main.cpp)
#TARGET_LINK_LIBRARIES(wave_test common_misc_wavegenerators boost_program_options boost_regex pthread)
INSTALL_TARGETS(/lib  common_misc_wavegenerators)
#INSTALL_TARGETS(/bin  wave_test)

FILE(GLOB headers "*.h")
INSTALL(FILES ${headers} DESTINATION include/common/misc/wavegenerators)
