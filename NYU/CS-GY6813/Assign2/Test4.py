# Thread attack to try and write to a .a file to write data not
# intended by the original attacker (.a backup should be locked
# and this attack should not work)

# Clean up before execution
if "testfile.txt.a" in listfiles():
    removefile("testfile.txt.a")
if "testfile.txt.b" in listfiles():
    removefile("testfile.txt.b")
if "testfile.txt.a.a" in listfiles():
    removefile("testfile.txt.a.a")
if "testfile.txt.a.b" in listfiles():
    removefile("testfile.txt.a.b")
if "testfile.txt" in listfiles():
    removefile("testfile.txt")

# Thread to overwrite a testfile.txt.a
def ThreadHit():
    log("Thread: " + getthreadname() + " starting\n")
    afile = ABopenfile("testfile.txt.a"), True)
    afile.writeat("SGeneral KenobiE",0)
    afile.close()
    log("Thread: " + getthreadname() + " closing\n")

# Create File
myfile = ABopenfile("testfile.txt",True)

# Write invalid string to force .a file to be used
myfile.writeat("test",0)

# Launch thread to force a new value instead of what is intended
createthread(ThreadHit)

# Wait for thread to complete
sleep(5)

try:
    assert('SE' == myfile.readat(None,0))
    myfile.close()
except:
    myfile.close()
    log("Thread hit success!")
