from socket import *
import os
import sys
import struct
import time
import select
import binascii

ICMP_ECHO_REQUEST = 8

def checksum(string):
    csum = 0
    count_to = (len(string) // 2) * 2
    count = 0

    while count < countTo:
        this_value = (string[count + 1]) * 256 + (string[count])
        csum += this_value
        csum &= 0xffffffff
        count += 2

    if count_to < len(string):
        csum += (string[len(string) - 1])
        csum &= 0xffffffff

    csum = (csum >> 16) + (csum & 0xffff)
    csum = csum + (csum >> 16)
    answer = ~csum
    answer = answer & 0xffff
    answer = answer >> 8 | (answer << 8 & 0xff00)
    return answer

def receiveOnePing(my_socket,ID,timeout,dest_addr):
    time_left = timeout

    while 1:
        started_select = time.time()
        what_ready = select.select([my_socket],[],[],time_left)
        how_long_in_select = (time.time() - started_select)
        if what_ready[0] == []: #Timeout
            return "Request timed out."

        time_received = time.time()
        rec_packet, addr = my_socket.recvfrom(1024)

        #Fetch the ICMP header from the IP packet
        rec_type, rec_code, rec_checksum, rec_id, rec_seq = struct.unpack("bbHHh",rec_packet[20:28])

        if rec_type == 0 and rec_code == 0 and rec_id == ID:

            #Calculate delay by subtracting when data is received by attached data
            #containing the time ping was sent
            return (time_received - (struct.unpack("d", rec_packet[28:28 + struct.calcsize("d")]))[0])

def sendOnePing(my_socket, dest_addr, ID):
    #Header is type(8), code (8), checksum (16), id (16), sequence (16)
    
    my_checksum = 0
    #Make a dummy header with a 0 checksum
    #Struct -- Interpret strings as packed binary data
    header = struct.pack("bbHHh",ICMP_ECHO_REQUEST, 0, my_checksum, ID, 1)
    data = struct.pack("d",time.time())
    #Calculate the checksum on the data and the dummy header
    my_checksum = checksum(header + data)

    #Get the right checksum, and put in the header

    if sys.platform == 'darwin':
        #Convert 16-bit integers from host to network byte order
        my_checksum = htons(my_checksum) & 0xffff
    else:
        my_checksum = htons(my_checksum)

    header = struct.pack("bbHHh",ICMP_ECHO_REQUEST,0,my_checksum,ID,1)
    packet = header + data

    my_socket.sendto(packet,(dest_addr,1)) #AF_INET address must be tuple, not str

    #Both LISTS and TUPLES consist of a number of objects
    #which can be referenced by their position number within the object

def doOnePing(dest_addr,timeout):
    icmp = getprotobyname("icmp")

    #SOCK_RAW is a powerful socket type. For more details: http://sockraw.org/papers/sock_raw
    my_socket = socket(AF_INET,SOCK_RAW,icmp)
    my_id = os.getpid() & 0xFFFF  #Return the current process i
    sendOnePing(my_socket,dest_addr,my_id)
    delay = receiveOnePing(my_socket,my_id,timeout,dest_addr)
    my_socket.close()
    return delay

def ping(host,timeout = 1):
    #timeout = 1 menas: If one second goes by without a reply from the server,
    #the client assumes that either the client's ping or the server's pong is lost
    dest = gethostbyname(host)
    print("Pinging " + dest + " using Python:")
    print("")
    #Send ping requests to a server separated by approximately one second
    while 1:
        delay = doOnePing(dest,timeout)
        print(delay)
        time.sleep(1)  #one second
    return delay

ping("nsa.gov")               #North America
ping("protonmail.ch")         #Europe
ping("piku.co.kr")            #Asia
ping("www.australia.gov.au")  #Australia
