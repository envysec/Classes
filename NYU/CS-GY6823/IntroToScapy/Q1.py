#Import Modules
import sys
from scapy.all import *

#Read in IP range in CIDR notation
ip_range = raw_input("Please enter an IP range in CIDR notation: ")
print "The range given is: ", ip_range

#Create Net object using given ip_range
#num_elements used to keep track of number of elements in ip_range
ip_range = Net(ip_range)
ip_list = []
num_elements = 0

#Iterate through Net object ip_range and insert into ip_list and increment num_elements
for ip in ip_range:
    ip_list.append(ip)
    num_elements += 1

#If subnet is routing mask value is <= 30 then remove base and broadcast IPs from list
if num_elements > 2:
    del ip_list[0]
    del ip_list[-1]

#Iterate through ip_list and craft a packet for each destination address then add to packet_set
packet_set = []
for ip in ip_list:
    new_packet = IP(dst = ip)/TCP(dport = 53)
    packet_set.append(new_packet)

for ip in ip_list:
    new_packet = IP(dst = ip)/TCP(dport = 80)
    packet_set.append(new_packet)

#Iterate through packet set and print each packet
for packet in packet_set:
    packet.show()
