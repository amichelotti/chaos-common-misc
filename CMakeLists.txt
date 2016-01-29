cmake_minimum_required(VERSION 2.6)
project(daq)
include($ENV{CHAOS_BUNDLE}/tools/project_template/CMakeChaos.txt)

ADD_SUBDIRECTORY(wavegenerators)
ADD_SUBDIRECTORY(data/btf)

# install(DIRECTORY DESTINATION include/driver/daq)  


