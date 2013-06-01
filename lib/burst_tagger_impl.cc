/* 
 * Copyright 2013 Bastian Bloessl<bloessl@ccs-labs.org>.
 * 
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 * 
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#include "burst_tagger_impl.h"
#include <gnuradio/io_signature.h>

namespace gr {
namespace ccs {

burst_tagger_impl::burst_tagger_impl(pmt::pmt_t tag_name)
		: gr::sync_block("burst_tagger",
			gr::io_signature::make(1, 1, sizeof(gr_complex)),
			gr::io_signature::make(1, 1, sizeof(gr_complex))),
		d_tag_name(tag_name) {
}

burst_tagger_impl::~burst_tagger_impl() {
}

int
burst_tagger_impl::work(int noutput_items,
	gr_vector_const_void_star &input_items,
	gr_vector_void_star &output_items) {

	const gr_complex *in = (const gr_complex*)input_items[0];
	gr_complex *out = (gr_complex*)output_items[0];

	std::vector<gr::tag_t> tags;
	const uint64_t nread = nitems_read(0);

	get_tags_in_range(tags, 0, nread, nread + noutput_items - 1,
		d_tag_name);
	std::sort(tags.begin(), tags.end(), tag_t::offset_compare);

	if(tags.size()) {
	}

	return noutput_items;
}

burst_tagger::sptr burst_tagger::make(pmt::pmt_t tag_name) {
	return gnuradio::get_initial_sptr(new burst_tagger_impl(tag_name));
}


} /* namespace ccs */
} /* namespace gr */

