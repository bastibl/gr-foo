                     ____                 \
   ____ ______      / __/___  ____     \   \
  / __ `/ ___/_____/ /_/ __ \/ __ \  \  |   |
 / /_/ / /  /_____/ __/ /_/ / /_/ /  /  |   |
 \__, /_/        /_/  \____/\____/     /   /
/____/                                    /


Hi!

This is a collection of custom blocks that are not directly associated with a
project. For sample applications see:

https://github.com/bastibl/gr-ieee802-11
https://github.com/bastibl/gr-ieee802-15-4



### Wireshark Connector

This blocks outputs PDUs in the PCAP format which is supported by all common
network monitoring applications. Some examples are Wireshark, Tshark, and
tcpdump. Currently, it supports WiFi packets with Radiotap header and ZigBee.

Since GNU Radio File Sinks have hard-coded open flags that include O_TRUNC one
can not open Linux pipes. For that reason, it is not possible to monitor traffic
live by connection Wireshark to a pipe. However, opening the file and
investigating the traffic offline works fine.

I made a patch that adds an append option to the File Sink. With this patch you
can also open pipes. If you are interested see:
https://github.com/bastibl/gnuradio/

For further information about Radiotap, PCAP, and Wireshark see:
http://www.radiotap.org/
http://www.wireshark.org/



### Burst Tagger

Tags a burst / packet / bunch of samples with tx_sob und tx_eob tags. This is
useful if the SDR is operating in half-duplex mode. With adding a tx_eob the
USRP switches back to receive mode immediately. That way, direct responses like
ACKs are not missed.

The block searches for a special tag that indicates the start of a burst. The
name of this tag is configurable. The tag should indicate the length of the
packet in samples. Also a multiplier can be specified, which multiples the
length given in the tag.



### Packet Dropper

Drops a configurable percentage of messages. I used it to test protocol logic
like ACKs, retransmission and stuff.



### Packet Pad

Adds a configurable number of zeros before and after a burst. The burst has to
be started with an tx_sob, and ended with a tx_eob tag. From time to time I had
issues when I did not pad the sample stream. This block helps to investigate
that. It is also handy for loopback testing when there is no continues sample
stream.



### Periodic Msg Source

This block sends messages at regular intervals much like the Message Strobe
block in mainline GNU Radio. With this block you can also configure the number
of packets that are sent. When all normal messages are sent, the blocks sends a
final PMT_EOF message to indicate that it is done.
I used this block for automated performance tests with no-GUI flow graphs.


### Dependencies

GNU Radio v3.7


### Installation

git clone https://github.com/bastibl/gr-foo.git
cd gr-foo
mkdir build
cd build
cmake ..
make
sudo make install
sudo ldconfig

