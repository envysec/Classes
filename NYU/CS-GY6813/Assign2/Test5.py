# Test to check if .b file is assigned prior to close() function

# Clean up before execution
if "testfile.txt.a" in listfiles():
    removefile("testfile.txt.a")
if "testfile.txt.b" in listfiles():
    removefile("testfile.txt.b")

# Create File
myfile = ABopenfile("testfile.txt", True)

try:
    assert('SE' == myfile.readat(None,0))
    myfile.close()
except:
    myfile.close()
    log("Invalid time for writing to file!\n")
