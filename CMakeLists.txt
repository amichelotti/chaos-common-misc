cmake_minimum_required(VERSION 2.6)
project(common_misc)
find_package(Boost MODULE REQUIRED REQUIRED COMPONENTS program_options system)
include_directories(../.. ${Boost_INCLUDE_DIRS})
link_directories(${CMAKE_INSTALL_PREFIX}/lib)
ADD_DEFINITIONS(-fPIC)
ADD_SUBDIRECTORY(scheduler)
ADD_SUBDIRECTORY(wavegenerators)
ADD_SUBDIRECTORY(data)
ADD_SUBDIRECTORY(beagle)
ADD_SUBDIRECTORY(driver)



# install(DIRECTORY DESTINATION include/driver/daq)  


