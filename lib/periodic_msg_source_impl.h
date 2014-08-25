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
#ifndef INCLUDED_FOO_PERIODIC_MSG_SOURCE_IMPL_H
#define INCLUDED_FOO_PERIODIC_MSG_SOURCE_IMPL_H

#include <foo/periodic_msg_source.h>

namespace gr {
namespace foo {

	class periodic_msg_source_impl : public periodic_msg_source {
		private:
			void run(periodic_msg_source_impl *instance);
			void eof_in(pmt::pmt_t msg);

			int d_nmsg_total;
			int d_nmsg_left;
			bool d_debug;
			bool d_finished;
			float d_interval;
			pmt::pmt_t d_msg;
			boost::thread *d_thread;
			gr::thread::mutex d_mutex;

		public:
			periodic_msg_source_impl(pmt::pmt_t msg,
					float interval, int num_msg, bool debug);
			virtual ~periodic_msg_source_impl();

			void set_nmsg(int nmsg);
			int get_nmsg();

			void set_delay(float delay);
			float get_delay();

			void start_tx();
			void stop_tx();
			bool is_running();
			uint32_t n_sent();

	};

}  // namespace foo
}  // namespace gr

#endif /* INCLUDED_FOO_PERIODIC_MSG_SOURCE_IMPL_H */
