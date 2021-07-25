#Import Modules
import sys
from scapy.all import *

#Read an IP address
target_ip = raw_input("Please enter an IP address: ")
print "The IP address is:", target_ip

#Set initial value of TTL to 1, all other variables set for do while loop of traceroute
ttl_value = 1
arrived = False
first_iter = True
counter = 1

#Loop ICMP ping with TTL incrementing by 1 every time
while not arrived or first_iter:
    first_iter = False

    #Craft ICMP packet with new TTL value
    ICMP_Packet = IP(dst = target_ip, ttl = ttl_value)/ICMP()
    
    #Send ICMP Packet and record reply, parse source IP address
    reply = sr1(ICMP_Packet, timeout = 5)
    source_ip = reply.src

    #Output packets
    print "========ICMP Packet============"
    ICMP_Packet.show()
    print "==============================="

    print "========Response Packet========"
    reply.show()
    print "==============================="

    #Output source address (address of next hop router)
    print "========Hop===================="
    print counter, " ", source_ip
    print "==============================="

    #If: Source IP address == Target IP then set arrived to True
    if source_ip == target_ip:
        arrived = True
    #Else: Increment Counter and TTL by 1
    else:
        counter += 1
        ttl_value += 1
