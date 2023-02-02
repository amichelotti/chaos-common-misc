cmake_minimum_required(VERSION 2.9)
file (GLOB src *.cpp)
 INCLUDE_DIRECTORIES(. ../../..)


ADD_LIBRARY(common_misc_wavegenerators STATIC ${src})


#ADD_EXECUTABLE(wave_test test/main.cpp)
#INSTALL_TARGETS(/lib  )
INSTALL(TARGETS common_misc_wavegenerators
        DESTINATION "lib"
        PERMISSIONS OWNER_WRITE OWNER_READ OWNER_EXECUTE GROUP_READ GROUP_WRITE GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
#INSTALL_TARGETS(/bin  wave_test)

FILE(GLOB headers "*.h")
INSTALL(FILES ${headers} DESTINATION include/common/misc/wavegenerators)
