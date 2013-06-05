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
#include <foo/packet_pad.h>
#include <gnuradio/io_signature.h>

#include <iostream>

using namespace gr::foo;


class packet_pad_impl : public packet_pad {

static const int SYMBOL_LENGTH = 80;

#define dout d_debug && std::cout

public:
packet_pad_impl(bool debug, unsigned int pad_front, unsigned int pad_tail) : block("packet_pad",
			gr::io_signature::make(1, 1, sizeof(gr_complex)),
			gr::io_signature::make(1, 1, sizeof(gr_complex))),
			d_debug(debug), d_pad_front(pad_front),
			d_pad_tail(pad_tail), d_front_left(0),
			d_tail_left(0), d_frame_left(0), d_state(IDLE) {

	set_relative_rate(1);
	set_output_multiple(SYMBOL_LENGTH);
	set_tag_propagation_policy(block::TPP_DONT);
}

~packet_pad_impl(){
}

void insert_sob(uint64_t item) {
	dout << "OFDM PAD: insert sob at: " << item << std::endl;

	static const pmt::pmt_t sob_key = pmt::string_to_symbol("tx_sob");
	static const pmt::pmt_t value = pmt::PMT_T;
	static const pmt::pmt_t srcid = pmt::string_to_symbol(alias());
	add_item_tag(0, item, sob_key, value, srcid);
}

void insert_eob(uint64_t item) {
	dout << "OFDM PAD: insert eob at: " << item << std::endl;

	static const pmt::pmt_t eob_key = pmt::string_to_symbol("tx_eob");
	static const pmt::pmt_t value = pmt::PMT_T;
	static const pmt::pmt_t srcid = pmt::string_to_symbol(alias());
	add_item_tag(0, item, eob_key, value, srcid);
}

int general_work (int noutput_items, gr_vector_int& ninput_items,
		gr_vector_const_void_star& input_items,
		gr_vector_void_star& output_items) {

	int ninput = ninput_items[0] / SYMBOL_LENGTH;	// available input symbols
	//assert(ninput_items[0] % SYMBOL_LENGTH == 0);
	int noutput = noutput_items / SYMBOL_LENGTH;	// available output symbols


	// these 4 values are updated in each iteration
	const gr_complex *in = (const gr_complex*)input_items[0];
	gr_complex *out = (gr_complex*)output_items[0];

	int written = 0;		// written symbols
	int consumed = 0;		// read symbols

	// tag handling
	const uint64_t nread = this->nitems_read(0);
	std::vector<gr::tag_t> tags;

	dout << "OFDM PAD: input size: " << ninput_items[0] << "   output size: "
	     << noutput_items << std::endl;

	while(written < noutput) {

		// debug
		dout << "OFDM PAD: iteration - consumed: " << consumed <<
				"   written: " << written << std::endl;

		switch(d_state) {

		case IDLE:

			//assert(consumed < ninput);
			if(consumed >= ninput) {
				goto out;
			}

			get_tags_in_range(tags, 0, nread + consumed * SYMBOL_LENGTH,
					nread + (consumed + 1) * SYMBOL_LENGTH - 1,
					pmt::string_to_symbol("ofdm_start"));

			assert(tags.size() == 1);

			d_front_left = d_pad_front;
			d_frame_left = pmt::to_uint64(tags[0].value);
			d_tail_left = d_pad_tail;

			insert_sob(this->nitems_written(0) + written * SYMBOL_LENGTH);
			d_state = FRONT;

			dout << "OFDM PAD: new frame - length: " << d_frame_left << std::endl;

			break;

		case FRONT:

			if(!((ninput >= d_frame_left) || (ninput == 102))) {
				goto out;
			}

			if(!d_front_left) {
				d_state = FRAME;
				break;
			}

			dout << "OFDM PAD: pad front" << std::endl;

			memset(out, 0, SYMBOL_LENGTH * sizeof(gr_complex));

			d_front_left--;
			out += SYMBOL_LENGTH;
			written++;

			break;

		case FRAME:

			//assert((consumed + 1) * SYMBOL_LENGTH <= ninput_items[0]);
			if(consumed >= ninput) {
				goto out;
			}

			dout << "OFDM PAD: copy frame" << std::endl;

			memcpy(out, in, SYMBOL_LENGTH * sizeof(gr_complex));

			d_frame_left--;
			consumed++;
			written++;
			in += SYMBOL_LENGTH;
			out += SYMBOL_LENGTH;

			if(!d_frame_left) {
				d_state = TAIL;
				break;
			}

			break;

		case TAIL:

			if(!d_tail_left) {
				d_state = IDLE;
				insert_eob(this->nitems_written(0) + written * SYMBOL_LENGTH - 1);
				goto out;
			}

			dout << "OFDM PAD: pad tail" << std::endl;

			memset(out, 0, SYMBOL_LENGTH * sizeof(gr_complex));

			d_tail_left--;
			written++;
			out += SYMBOL_LENGTH;

			break;

		default:
			assert(false);
			break;
		}
	}

out:

	dout << "OFDM PAD: consumed: " << consumed << "   produced: " << written << std::endl;
	consume(0, consumed * SYMBOL_LENGTH);
	return written * SYMBOL_LENGTH;
}

void
forecast (int noutput_items, gr_vector_int &ninput_items_required)
{

	if(d_frame_left) {
		ninput_items_required[0] = noutput_items;

	} else if(d_state == TAIL) {
		ninput_items_required[0] = 0;
	} else {
		ninput_items_required[0] = 80;
	}

	dout << "PAD forecast noutput_items: " << noutput_items << "  required: " << ninput_items_required[0] << std::endl;
	dout << "PAD state " << d_state << std::endl;
	dout << "d_frame_left " << d_frame_left << std::endl;
}



private:
	bool d_debug;

	int d_pad_front;
	int d_pad_tail;

	int d_front_left;
	int d_tail_left;
	int d_frame_left;

	enum {IDLE, FRONT, FRAME, TAIL} d_state;
};

packet_pad::sptr
packet_pad::make(bool debug, unsigned int pad_front, unsigned int pad_tail) {
	return gnuradio::get_initial_sptr(new packet_pad_impl(debug, pad_front, pad_tail));
}
