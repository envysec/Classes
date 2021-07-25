# Overwrite file with empty text (should lead to an SE entry)

# Clean up before execution
if "testfile.txt.a" in listfiles():
    removefile("testfile.txt.a")
if "testfile.txt.b" in listfiles():
    removefile("testfile.txt.b")
if "testfile.txt" in listfiles():
    removefile("testfile.txt")

# Create initial file
myfile = ABopenfil("testfile.txt",True)
myfile.writeat("StestE",0)
myfile.close()

# Overwrite existing file
myfile = ABopenfile("testfile.txt",True)

# Write empty string
myfile.writeat("",0)

# Close the file
myfile.close()

# Read file and check contents
myfile = ABopenfile("testfile.txt",False)

try:
    assert('SE' == myfile.readat(None,0))
    myfile.close()
except:
    myfile.close()
    log("Invalid data write to a file is successful!\n")
