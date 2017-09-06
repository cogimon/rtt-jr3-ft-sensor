#
# The find_package macro for Orocos-RTT works best with
# cmake >= 2.6.3
#
cmake_minimum_required(VERSION 2.6.3)

#
# This creates a standard cmake project. You may extend this file with
# any cmake macro you see fit.
#
project(rtt-jr3-ft-sensor)

# Use 'source orocos_toolchain/env.sh' if the command below fails:
find_package(OROCOS-RTT QUIET)
if (NOT OROCOS-RTT_FOUND)
  message (FATAL_ERROR "\nCould not find Orocos. Please use the shell command\n 'source orocos_toolchain/env.sh' and then run cmake again.")
endif()

find_package(RST-RT REQUIRED)
IF (RST-RT_FOUND)
        message(STATUS "RST-RT found. Version: ${RST-RT_VERSION}")
        include_directories(BEFORE SYSTEM ${RST-RT_INCLUDE_DIRS})
        add_definitions(${RST-RT_CFLAGS})
        link_directories(${RST-RT_LIBRARY_DIRS})
        link_libraries(${RST-RT_LIBRARIES})
ENDIF()

# Defines the orocos_* cmake macros. See that file for additional
# documentation.
include( ${OROCOS-RTT_USE_FILE_PATH}/UseOROCOS-RTT.cmake )

# Creates a component library libmytest-<target>.so
# and installs in the directory lib/orocos/mytest/
#
orocos_component(rtt-jr3-ft-sensor include/rtt-jr3-ft-sensor-component.hpp src/rtt-jr3-ft-sensor-component.cpp)

#
# Generates and installs our package. Must be the last statement such
# that it can pick up all above settings.
#
orocos_generate_package()
