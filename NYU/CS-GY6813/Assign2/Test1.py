# Create new file with lower case letters

# Clean up before execution
if "testfile1.txt.a" in listfiles():
    removefile("testfile1.txt.a")
if "testfile1.txt.b" in listfiles():
    removefile("testfile1.txt.b")
if "testfile.txt" in listfiles():
    removefile("testfile.txt")

# Create File
myfile = ABopenfile("testfile1.txt",True)

# Write lower case letters
myfile.writeat("steste",0)

# Close the file

# Read file
myfile = ABopenfile("testfile1.txt",True)

# Check contents
try:
    assert('SE' == myfile.readat(0,0))
    
    # Close the file
    myfile.close()
except:
    #Close the file
    myfile.close()
    log("Invalid data write to a file is successful!\n")
