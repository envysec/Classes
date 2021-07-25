#Import modules
import sys
from scapy.all import *

#Read in IP address
target_ip = raw_input("Please enter an IP address: ")
print "The IP address is: ", target_ip

#Loop throuh multiple ports to send SYN packets
for i in range(1,1000):
    #Craft SYN packet with new source port
    new_packet = IP(dst=target_ip)/TCP(dport=135,sport=i,flags="S")
    
    #Print Packet
    print "========New SYN Packet========"
    new_packet.show()
    print "=============================="

    #Send packet
    sr1(new_packet)
