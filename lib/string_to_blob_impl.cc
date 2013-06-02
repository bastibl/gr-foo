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
#include <ccs/string_to_blob.h>

#include <gnuradio/io_signature.h>
#include <gnuradio/block_detail.h>
#include <string.h>

using namespace gr::ccs;

class string_to_blob_impl : public string_to_blob {

public:

string_to_blob_impl() :
		block("string_to_blob",
				gr::io_signature::make(0, 0, 0),
				gr::io_signature::make(0, 0, 0)) {

    message_port_register_out(pmt::mp("out"));

    message_port_register_in(pmt::mp("in"));
    set_msg_handler(pmt::mp("in"), boost::bind(&string_to_blob_impl::make_blob, this, _1));
}

~string_to_blob_impl() {

}

void make_blob(pmt::pmt_t msg) {

	if(pmt::is_eof_object(msg)) {
		message_port_pub(pmt::mp("out"), pmt::PMT_EOF);
		detail().get()->set_done(true);
		return;
	}

	assert(pmt::is_symbol(msg));

	int len = pmt::symbol_to_string(msg).length();
	const char *data = pmt::symbol_to_string(msg).data();

	pmt::pmt_t blob = pmt::make_blob(data, len);

	message_port_pub(pmt::mp("out"), blob);
}

};

string_to_blob::sptr
string_to_blob::make() {
	return gnuradio::get_initial_sptr(new string_to_blob_impl());
}


