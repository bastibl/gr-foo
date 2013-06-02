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
#include <ccs/periodic_msg_source.h>

#include <gnuradio/io_signature.h>
#include <gnuradio/block_detail.h>
#include <string.h>

using namespace gr::ccs;

class periodic_msg_source_impl : public periodic_msg_source {

#define dout d_debug && std::cout

public:

periodic_msg_source_impl(pmt::pmt_t msg, float interval, int num_msg, bool debug) :
		block("periodic_msg_source",
				gr::io_signature::make(0, 0, 0),
				gr::io_signature::make(0, 0, 0)),
		d_msg(msg),
		d_num_msg(num_msg),
		d_interval(interval),
		d_debug(debug),
       		d_finished(false) {

	message_port_register_out(pmt::mp("out"));
	d_thread = boost::shared_ptr<boost::thread>(new boost::thread(boost::bind(&periodic_msg_source_impl::run, this, this)));
}

~periodic_msg_source_impl() {
	d_finished = true;
	d_thread->interrupt();
	d_thread->join();
}


void run(periodic_msg_source_impl *instance) {
	while(!d_finished) {

		dout << "number of messages left: " << d_num_msg << std::endl;

		if(!d_num_msg || d_finished) {
			d_finished = true;
			break;
		}

		message_port_pub( pmt::mp("out"), d_msg );

		boost::this_thread::sleep(boost::posix_time::milliseconds(d_interval)); 

		if(d_num_msg > 0) {
			d_num_msg--;
		}
	} 

	dout << "stopping msg source" << std::endl;
	message_port_pub(pmt::mp("out"), pmt::PMT_EOF);
}

private:
	int d_num_msg;
	bool d_debug;
	bool d_finished;
	float d_interval;
	pmt::pmt_t d_msg;
	boost::shared_ptr<boost::thread> d_thread;

};

periodic_msg_source::sptr
periodic_msg_source::make(pmt::pmt_t msg, float interval, int num_msg, bool debug) {
	return gnuradio::get_initial_sptr(new periodic_msg_source_impl(msg, interval, num_msg, debug));
}

