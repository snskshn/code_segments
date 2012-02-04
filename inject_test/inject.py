#!/usr/bin/python
import sys
import pylorcon
wifi = pylorcon.Lorcon("ath0", "madwifing")
wifi.setfunctionalmode("INJECT");
wifi.setmode("MONITOR");
wifi.setchannel(1);
essid = "HelloWorld"
length_of_essid = chr(len(essid))
destination_addr = '\xff\xff\xff\xff\xff\xff';
source_addr = '\xde\xad\xde\xad\xde\xad';
bss_id_addr = '\x00\x1f\xb8\xff\xe2\x28';
# Type/Subtype 0/8 Management/Beacon
packet = '\x80\x00'
# flags and duration
packet = packet + '\x00\x00';
packet = packet + destination_addr
packet = packet + source_addr
packet = packet + bss_id_addr
# sequence number
packet = packet + '\x90\x70';
# fixed params, timestamp, beacon interval, capability interval
packet = packet + '\x8a\xd1\xf7\x3c\x00\x00\x00\x00\x64\x00\x11\x04';
# tag number 0
packet = packet + '\x00' + length_of_essid + essid;
# tag number 1
packet = packet + '\x01' + '\x08\x82\x84\x8b\x96\x24\x30\x48\x6c'
# tag number 3
packet = packet + '\x03' + '\x01\x0b'
packet = packet + '\x05\x04\x02\x03\x00\x00'
packet = packet + '\x2a\x01\x00'
packet = packet + '\x2f\x01\x00'
packet = packet + '\x32\x04\x0c\x12\x18\x60'
packet = packet + '\xdd\x06\x00\x10\x18\x02\x00\x00';
print "About to transmit HelloWorld beacon";
for n in range(10000):
	wifi.txpacket (packet);
print "Done";
