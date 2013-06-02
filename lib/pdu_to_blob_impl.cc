/*
 * Copyright (C) 2013 Bastian Bloessl <bloessl@ccs-labs.org>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <ccs/pdu_to_blob.h>

#include <gnuradio/io_signature.h>
#include <string>

using namespace gr::ccs;

class pdu_to_blob_impl : public pdu_to_blob {

public:

pdu_to_blob_impl() :
		block("pdu_to_blob",
				gr::io_signature::make(0, 0, 0),
				gr::io_signature::make(0, 0, 0)) {

	message_port_register_out(pmt::mp("out"));

	message_port_register_in(pmt::mp("in"));
	set_msg_handler(pmt::mp("in"), boost::bind(&pdu_to_blob_impl::make_blob, this, _1));
}

~pdu_to_blob_impl() {

}

void make_blob(pmt::pmt_t msg) {

	assert(pmt::is_pair(msg));

	pmt::pmt_t vec = pmt::cdr(msg);
	assert(pmt::is_u8vector(vec));

	size_t len;

	const void *data = pmt::uniform_vector_elements(vec, len);
	assert(len < 4000);

	std::memcpy(buf, data, len);

	pmt::pmt_t blob = pmt::make_blob(buf, len);

	message_port_pub(pmt::mp("out"), blob);
}

private:
	char buf[4000];

};

pdu_to_blob::sptr
pdu_to_blob::make() {
	return gnuradio::get_initial_sptr(new pdu_to_blob_impl());
}
