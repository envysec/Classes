from socket import *
import os
import sys
import struct
import time
import select
import binascii

ICMP_ECHO_REQUEST = 8
MAX_HOPS = 30
TIMEOUT = 2.0
TRIES = 2
#The packet that we shall send to each router along the path is the ICMP echo
#request packet, which is exactly what we had used in the ICMP ping exercise.
#We shall use the same packet that we built in the Ping exercise

def checksum(string):
    #In this function we make the checksum of our packet

    csum = 0
    count_to = (len(string) // 2) * 2
    count = 0

    while count < count_to:
        this_value = (string[count + 1]) * 256 + (string[count])
        csum += this+_value
        csum &= 0xffffffff
        count += 2

    if count_to < len(string):
        csum += (string[len(string) - 1])
        csum &= 0xffffffff

    csum = (csum >> 16) + (csum & 0xfff)
    csum = csum + (csum >> 16)
    answer = ~csum
    answer = answer & 0xffff
    answer = answer >> 8 | (answer << 8 & 0xff00)
    return answer

def build_packet():
    #In the sendOnePing() method of the ICMP Ping exercise, firstly the header of our
    #packet to be sent was made, secondly the checksum was appended to the header and
    #then finally the complete packet was sent to the destination.

    #Make the header in a similar way to the ping exercise
    #Append checksum to the header

    #Don't send the packet yet, just return the final packet in this function
    #So the function ending should look like this

    ID = os.getpid() & 0xFFFF  #Return the current process i
    my_checksum = 0

    #Make a dummy header with a 0 checksum
    #struct -- Interpret strings as packed binary data
    header = struct.pack("bbHHh",ICMP_ECHO_REQUEST,0,my_checksum,ID,1)
    data = struct.pack("d",time.time())

    #Calculate the checksum on the data and the dummy header
    my_checksum = checksum(header + data)

    #Get the right checksum and put in the header
    if sys.platform == 'darwin':
        #Convert 16-bit integers from host to network byte order
        my_checksum = htons(my_checksum) & 0xffff
    else:
        my_checksum = htons(my_checksum)

    #Create finalized packet
    header = struct.pack("bbHHh",ICMP_ECHO_REQUEST,0,my_checksum,ID,1)
    packet = header + data

    #Return packet
    return packet

def get_route(hostname):
    time_left = TIMEOUT
    
    for ttl in range(1,MAX_HOPS):
        for tries in range(TRIES):
            dest_addr = gethostbyname(hostname)

            #Make a raw socket named my_socket
            icmp = getprotobyname("icmp")
            my_socket = socket(AF_INET, SOCK_RAW, icmp)
            
            my_socket.setsockopt(IPPROTO_IP,IP_TTL,struct.pack('I',ttl))
            my_socket.settimeout(TIMEOUT)

            try:
                d = build_packet()
                my_socket.sendto(d,(hostname,0))
                t = time.time()
                started_select = time.time()
                what_ready = select.select([my_socket],[],[],time_left)
                how_long_in_select = (time.time() - started_select)
                if what_ready[0] == []: #Timeout
                    print(" * * * Request timed out.")
                recv_packet, addr = my_socket.recvfrom(1024)
                time_received = time.time()
                time_left = time_left - how_long_in_select
                if time_left <= 0:
                    print(" * * * Request timed out.")

            except timeout:
                continue

            else:
                #Fetch the icmp type from the IP packet
                types, codes, csums, IDs, seqs = struct.unpack("bbHHh",recv_packet[20:28])
                print("==========================")
                print("Type: " + str(types))
                print("Code: " + str(codes))
                print("Checksum: " + str(csums))
                print("ID: " + str(IDs))
                print("Seq: " + str(seqs))

                if types == 11:
                    bytes = struct.calcsize("d")
                    time_sent = struct.unpack("d", recv_packet[28:28 + bytes])[0]
                    print(" %d rtt=%.0f ms %s" % (ttl,(time_received - t) * 1000,addr[0]))
                elif types == 3:
                    bytes = struct.calcsize("d")
                    time_sent = struct.unpack("d", recv_packet[28:28 + bytes])[0]
                    print(" %d rtt=%.0f ms %s" % (ttl,(time_received - t) * 1000,addr[0]))
                elif types == 0:
                    bytes = struct.calcsize("d")
                    time_sent = struct.unpack("d", recv_packet[28:28 + bytes])[0]
                    print(" %d rtt=%.0f ms %s" % (ttl,(time_received - time_sent) * 1000, addr[0]))
                    return;
                else:
                    print("error")
                break
            finally:
                my_socket.close()

get_route("google.com")          #North America
get_route("protonmail.ch")       #Europe
get_route("piku.co.kr")          #Asia
get_route("amazon.co.nz")        #Australia
