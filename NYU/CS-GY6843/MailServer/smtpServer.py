from socket import *

msg = "\r\n I love computer networks!"
endmsg = "\r\n.\r\n"

#Mailhog running on localhost port 1025
mailserver = "localhost"
serverPort = 1025

#Create socket claled clientSocket and establish a TCP connection with mailserver
clientSocket = socket(AF_INET,SOCK_STREAM)
clientSocket.connect((mailserver,serverPort))

recv = clientSocket.recv(1024).decode()
print(recv)

if recv[:3] != '220':
    print('220 reply not received from server.')

#Send HELO command and print server response
heloCommand = 'HELO Alice\r\n'
clientSocket.send(heloCommand.encode())
recv1 = clientSocket.recv(1024).decode()
print(recv1)

if recv1[:3] != '250':
    print('250 reply not received from server.')

#Send MAIL FROM command and print server response
mailFromComm = 'MAIL FROM:<alice@test.com>\r\n'
clientSocket.send(mailFromComm.encode())
recv2 = clientSocket.recv(1024).decode()
print(recv2)

if recv2[:3] != '250':
    print('250 reply not received from server.')

#Send RCPT TO command and print server response
rcptToComm = 'RCPT TO:<bob@test.com>\r\n'
clientSocket.send(rcptToComm.encode())
recv3 = clientSocket.recv(1024).decode()
print(recv3)

if recv3[:3] != '250':
    print('250 reply not received from server.')

#Send DATA command and print server response
dataComm = 'DATA\r\n'
clientSocket.send(dataComm.encode())
recv4 = clientSocket.recv(1024).decode()
print(recv4)

if recv4[:3] != '354':
    print('354 reply not received from server.')

#Send message data
clientSocket.send(msg.encode())

#Message ends with a single period
clientSocket.send(endmsg.encode())
recv5 = clientSocket.recv(1024).decode()
print(recv5)

if recv5[:3] != '250':
    print('250 reply not received from server.')

#Send QUIT command and get server response
quitComm = 'QUIT\r\n'
clientSocket.send(quitComm.encode())
recv6 = clientSocket.recv(1024).decode()
print(recv6)

if recv6[:3] != '221':
    print('221 reply not received from server.')

#Close socket
clientSocket.close()
