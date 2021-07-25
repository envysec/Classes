#Import modules
import sys
from scapy.all import *

#Read in IP address
target_ip = raw_input("Please enter an IP address: ")
print "The IP address given is: ", target_ip

#Craft ICMP packet
ICMP_Packet = IP(dst = target_ip)/ICMP()

#Send ICMP packet and record response
reply = sr1(ICMP_Packet, timeout = 10)

#Print crafted packet and response from ICMP packet
print "========Crafted ICMP Packet========"
ICMP_Packet.show()
print "========Response Packet============"
reply.show()
