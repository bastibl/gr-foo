# Copyright 2011,2012 Free Software Foundation, Inc.
#
# This file is part of GNU Radio
#
# GNU Radio is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3, or (at your option)
# any later version.
#
# GNU Radio is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with GNU Radio; see the file COPYING.  If not, write to
# the Free Software Foundation, Inc., 51 Franklin Street,
# Boston, MA 02110-1301, USA.

########################################################################
# Setup library
########################################################################
include(GrPlatform) #define LIB_SUFFIX

include_directories(
    ${Boost_INCLUDE_DIR}
    ${UHD_INCLUDE_DIRS}
)
link_directories(
    ${Boost_LIBRARY_DIRS}
    ${UHD_LIBRARY_DIRS}
)

list(APPEND foo_sources
    burst_tagger_impl.cc
    channel_model_impl.cc
    packet_dropper_impl.cc
    packet_pad2.cc
    packet_pad_impl.cc
    periodic_msg_source_impl.cc
    random_periodic_msg_source_impl.cc
    rtt_measure_impl.cc
    wireshark_connector_impl.cc
)


add_library(gnuradio-foo SHARED ${foo_sources})
target_link_libraries(gnuradio-foo ${Boost_LIBRARIES} ${GNURADIO_ALL_LIBRARIES} ${UHD_LIBRARIES})
set_target_properties(gnuradio-foo PROPERTIES DEFINE_SYMBOL "gnuradio_foo_EXPORTS")

########################################################################
# Install built library files
########################################################################
install(TARGETS gnuradio-foo
    LIBRARY DESTINATION lib${LIB_SUFFIX} # .so/.dylib file
    ARCHIVE DESTINATION lib${LIB_SUFFIX} # .lib file
    RUNTIME DESTINATION bin              # .dll file
)
