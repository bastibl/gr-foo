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
#include "periodic_msg_source_impl.h"

#include <gnuradio/io_signature.h>
#include <gnuradio/block_detail.h>
#include <string.h>

using namespace gr::foo;

#define dout d_debug && std::cout

periodic_msg_source_impl::periodic_msg_source_impl(pmt::pmt_t msg,
			float interval, int num_msg, bool debug) :
		block("periodic_msg_source",
				gr::io_signature::make(0, 0, 0),
				gr::io_signature::make(0, 0, 0)),
		d_msg(msg),
		d_nmsg_total(num_msg),
		d_nmsg_left(num_msg),
		d_interval(interval),
		d_debug(debug),
		d_finished(false) {

	message_port_register_out(pmt::mp("out"));

	message_port_register_in(pmt::mp("eof in"));
	set_msg_handler(pmt::mp("eof in"), boost::bind(&periodic_msg_source_impl::eof_in, this, _1));

	d_thread = new boost::thread(boost::bind(&periodic_msg_source_impl::run, this, this));
}

periodic_msg_source_impl::~periodic_msg_source_impl() {
	gr::thread::scoped_lock(d_mutex);

	d_finished = true;
	d_thread->interrupt();
	d_thread->join();
	delete d_thread;
}

void
periodic_msg_source_impl::eof_in (pmt::pmt_t msg) {

	std::cout << "EOF IN CALLED" << std::endl;

	if(pmt::is_eof_object(msg)) {
		detail().get()->set_done(true);
		dout << "stopping msg source" << std::endl;
		message_port_pub(pmt::mp("out"), pmt::PMT_EOF);
		return;
	} else {
		dout << "non EOF message at eof port!" << std::endl;
	}
}

void
periodic_msg_source_impl::run(periodic_msg_source_impl *instance) {

	try {

	// flow graph startup delay
	boost::this_thread::sleep(boost::posix_time::milliseconds(500));

	std::cout << "starting thread" << std::endl;

	while(1) {
		std::cout << "in while loop" << std::endl;
		float delay;
		{
			std::cout << "in scope" << std::endl;
			gr::thread::scoped_lock(d_mutex);
			std::cout << "got lock" << std::endl;
			if(d_finished || !d_nmsg_left) {
				d_finished = true;
				break;
			}

			dout << "number of messages left: " << d_nmsg_left << std::endl;

			message_port_pub( pmt::mp("out"), d_msg );

			if(d_nmsg_left > 0) {
				d_nmsg_left--;
			}
			
			delay = d_interval;
		}
		boost::this_thread::sleep(boost::posix_time::milliseconds(delay));
	} 

	} catch(boost::thread_interrupted) {
		std::cout << "interrupted start" << std::endl;
		gr::thread::scoped_lock(d_mutex);
		d_finished = true;
		std::cout << "interrupted end" << std::endl;
	}
}

void
periodic_msg_source_impl::set_nmsg(int nmsg) {
	gr::thread::scoped_lock(d_mutex);
	d_nmsg_total = nmsg;
}

int
periodic_msg_source_impl::get_nmsg() {
	gr::thread::scoped_lock(d_mutex);
	return d_nmsg_total;
}

void
periodic_msg_source_impl::set_delay(float delay) {
	gr::thread::scoped_lock(d_mutex);
	d_interval = delay;
}

float
periodic_msg_source_impl::get_delay() {
	gr::thread::scoped_lock(d_mutex);
	return d_interval;
}


void
periodic_msg_source_impl::start_tx() {
	gr::thread::scoped_lock(d_mutex);

	if(is_running()) return;

	d_nmsg_left = d_nmsg_total;
	d_finished = false;
	d_thread->join();
	delete d_thread;

	d_thread = new boost::thread(boost::bind(&periodic_msg_source_impl::run, this, this));
}

void
periodic_msg_source_impl::stop_tx() {
	d_thread->interrupt();
}

bool
periodic_msg_source_impl::is_running() {
	return !d_finished;
}

periodic_msg_source::sptr
periodic_msg_source::make(pmt::pmt_t msg, float interval, int num_msg, bool debug) {
	return gnuradio::get_initial_sptr(new periodic_msg_source_impl(msg, interval, num_msg, debug));
}

