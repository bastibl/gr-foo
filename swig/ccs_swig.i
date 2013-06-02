#define CCS_API

%include "gnuradio.i"

//load generated python docstrings
%include "ccs_swig_doc.i"

%{
#include "ccs/burst_tagger.h"
#include "ccs/packet_dropper.h"
#include "ccs/packet_pad.h"
#include "ccs/pdu_to_blob.h"
#include "ccs/periodic_msg_source.h"
#include "ccs/string_to_blob.h"
#include "ccs/wireshark_connector.h"
%}


%include "ccs/burst_tagger.h"
%include "ccs/packet_dropper.h"
%include "ccs/packet_pad.h"
%include "ccs/pdu_to_blob.h"
%include "ccs/periodic_msg_source.h"
%include "ccs/string_to_blob.h"
%include "ccs/wireshark_connector.h"

GR_SWIG_BLOCK_MAGIC2(ccs, burst_tagger);
GR_SWIG_BLOCK_MAGIC2(ccs, packet_dropper);
GR_SWIG_BLOCK_MAGIC2(ccs, wireshark_connector);
GR_SWIG_BLOCK_MAGIC2(ccs, packet_pad);
GR_SWIG_BLOCK_MAGIC2(ccs, pdu_to_blob);
GR_SWIG_BLOCK_MAGIC2(ccs, periodic_msg_source);
GR_SWIG_BLOCK_MAGIC2(ccs, string_to_blob);
