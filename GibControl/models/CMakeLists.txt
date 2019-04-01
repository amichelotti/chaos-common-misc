cmake_minimum_required(VERSION 2.6)
add_subdirectory(SimGib)
add_subdirectory(gibccaltDrv)
SET(DRIVERS_LIB ${DRIVERS_LIB} PARENT_SCOPE)

#INSTALL_TARGETS(/lib ${PROJECT_NAME})
