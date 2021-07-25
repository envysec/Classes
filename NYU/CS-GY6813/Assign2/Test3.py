# Thread attack to see if close contains validation/locks are used
# 'E' should persist at the end, question is if 'S' will be
# overwritten with an 'H'

# Clean up before execution
if "testfile.txt.a" in listfiles():
    removefile("testfile.txt.a")
if "testfile.txt.b" in listfiles():
    removefile("testfile.txt.b")
if "testfile.txt" in listfiles():
    removefile("testfile.txt")

# Create file
myfile = ABopenfile("testfile.txt",True)

def ThreadHit():
    log("Thread: " + getthreadname() + " starting\n")
    myfile.writeat(text,0)
    log("Thread: " + getthreadname() + " closing\n")

# Create 10 threads to write to a file

try:
    text = "Hello"
    createthread(ThreadHit)
    createthread(ThreadHit)

    text = "SThereE"
    createthread(ThreadHit)

    text = "Hello"
    createthread(ThreadHit)
    createthread(ThreadHit)
    createthread(ThreadHit)
    createthread(ThreadHit)
    createthread(ThreadHit)
    createthread(ThreadHit)
    createthread(ThreadHit)
    createthread(ThreadHit)

except:
    log("Error: unable to start thread.\n")

sleep(3)
log("Closing\n")

try:
    assert('S' == myfile.readat(1,0))
except:
    log("Failed\n")

myfile.close()

myfile = ABopenfile("testfile.txt",True)

try:
    assert('S' == myfile.readat(1,0))
    assert('E' == str(myfile.readat(None,0))[-1]
    myfile.close()
except:
    myfile.close()
    log("Invalid data write to a file is successful\n")
