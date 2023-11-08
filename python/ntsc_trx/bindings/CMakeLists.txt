# Copyright 2020 Free Software Foundation, Inc.
#
# This file is part of GNU Radio
#
# SPDX-License-Identifier: GPL-3.0-or-later
#

########################################################################
# Check if there is C++ code at all
########################################################################
if(NOT ntsc_trx_sources)
    MESSAGE(STATUS "No C++ sources... skipping python bindings")
    return()
endif(NOT ntsc_trx_sources)

########################################################################
# Check for pygccxml
########################################################################
GR_PYTHON_CHECK_MODULE_RAW(
    "pygccxml"
    "import pygccxml"
    PYGCCXML_FOUND
    )

include(GrPybind)

########################################################################
# Python Bindings
########################################################################

list(APPEND ntsc_trx_python_files
    transmitter_c_python.cc
    decoder_c_python.cc
    video_stream_converter_c_python.cc python_bindings.cc)

GR_PYBIND_MAKE_OOT(ntsc_trx
   ../../..
   gr::ntsc_trx
   "${ntsc_trx_python_files}")

# copy bindings extension for use in QA test module
add_custom_command(TARGET ntsc_trx_python POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:ntsc_trx_python>
    ${PROJECT_BINARY_DIR}/test_modules/gnuradio/ntsc_trx/
)

install(TARGETS ntsc_trx_python DESTINATION ${GR_PYTHON_DIR}/gnuradio/ntsc_trx COMPONENT pythonapi)
