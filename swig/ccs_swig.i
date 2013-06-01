#define CCS_API

%include "gnuradio.i"

//load generated python docstrings
%include "ccs_swig_doc.i"

%{
#include "ccs/packet_dropper.h"
#include "ccs/wireshark_connector.h"
%}


%include "ccs/packet_dropper.h"
%include "ccs/wireshark_connector.h"

GR_SWIG_BLOCK_MAGIC2(ccs, packet_dropper);
GR_SWIG_BLOCK_MAGIC2(ccs, wireshark_connector);
