/*
 * Copyright (C) 2013 Christoph Leitner <c.leitner@student.uibk.ac.at>
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
#ifndef INCLUDED_FOO_PACKET_DROPPER_H
#define INCLUDED_FOO_PACKET_DROPPER_H

#include <foo/api.h>
#include <gnuradio/block.h>

namespace gr {
namespace foo {

	class FOO_API packet_dropper : virtual public gr::block
	{
	public:
		typedef std::shared_ptr<packet_dropper> sptr;
		static sptr make(double drop_rate, unsigned long seed);
	};
}  // namespace foo
}  // namespace gr

#endif /* INCLUDED_FOO_PACKET_DROPPER_H */
