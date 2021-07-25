#!/usr/bin/env python3
import sys
try:
    from pwn import *
except ImportError:
    print("In order to complete this challenge, please install pwntools")
    print("https://pwntools.readthedocs.io/en/stable/install.html")
    sys.exit(1)

word_number_mapping = { 'ZERO' : '0', 'ONE' : '1', 'TWO' : '2', 'THREE' : '3', 'FOUR' : '4',
                        'FIVE' : '5', 'SIX' : '6', 'SEVEN' : '7', 'EIGHT' : '8', 'NINE' : '9'} 

"""
Example input
3974 - 4440 = ?
-466
Aww yeah!
NINE-SIX-SEVEN + 2204 = ?
3171
Aww yeah!
0b1100110011110 + 0xd2a = ?
9928
Aww yeah!
""" 

def stringToDec(number_string):
    translation = ''
    # Separate each digit and translate to number form
    digits = number_string.split('-')
    for digit in digits:
        translation += word_number_mapping[digit]
    return translation
    
def translateNumber(string):
    # Determines if string is hex or word format
    if string[1] == 'x':
        return int(string,16)
    elif string[1] == 'b':
        return int(string, 2)
    else:
        return stringToDec(string)

def calculateAnswer(num1, num2, operator):
    # Interpret and execute expression
    if operator == '+':
        return num1 + num2
    elif operator == '-':
        return num1 - num2
    elif operator == '*':
        return num1 * num2
    elif operator == '/':
        return int(num1 / num2)
    else:
        print("Operator does not exist")
        return -1

def processResponse(data):
    #Break down response
    expression = data.decode('utf-8').split(' ')
    
    #Process first number
    if not expression[0].isdigit():
        num1 = translateNumber(expression[0]) 
    else:
        num1 = expression[0]

    #Process second number
    if not expression[2].isdigit():
        num2 = translateNumber(expression[2])
    else:
        num2 = expression[2]

    #Evaluate expression
    return str(calculateAnswer(int(num1),int(num2),expression[1]))

def pwn(address, port, netid):
    # Establish connection
    connection = remote(address, port)
    # Catch request for NetID
    print(connection.recvuntil(':'))
    # Reply with NetID
    connection.sendline(netid)
    # Catch long message about being a math whiz
    response = connection.recvuntil("??\n")
    print(response)

    # Solve all math questions
    for i in range(200):
        # Catch math question
        response = connection.recvline()
        print(response)
        
        # Process question and respond with answer
        connection.sendline(processResponse(response))
        
        # Catch the "\n?Aww yeah!\n" to clear response for next question
        print(connection.recvuntil("!\n"))

def main():
    try:
        address = sys.argv[1]
        port = sys.argv[2]
        netid = sys.argv[3]
    except IndexError:
        print("Usage: ./mathwhiz.py [IP] [Port] [NetID]")
        sys.exit(1)
    pwn(address, port, netid)

if __name__ == "__main__":
    main()


