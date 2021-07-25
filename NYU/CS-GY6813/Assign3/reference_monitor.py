"""
This security layer inadequately handles A/B storage for files in RepyV2.

Note:
    This security layer uses encasementlib.r2py, restrictions.default, repy.py and Python
    Also you need to give it an application to run.
    python repy.py restrictions.default encasementlib.r2py [security_layer].r2py [attack_program].r2py

    """

TYPE="type"
ARGS="args"
RETURN="return"
EXCP="exceptions"
TARGET="target"
FUNC="func"
OBJC="objc"

class ABFile():
    def __init__(self,filename,create):
        # globals
        mycontext['debug'] = False
        # local (per object) reference to the underlying file
        self.Afn = filename + '.a'
        self.Bfn = filename + '.b'

        #Create Lock Object
        self.lock = createlock()

        if create:
            #If file already exists
            if self.Afn in listfiles():
                #log ("File already exists do not create\n")
                self.Afile = openfile(self.Afn,create)
                self.Bfile = openfile(self.Bfn,create)

                self.lock.acquire(True)
                self.Bfile.writeat(self.Afile.readat(None,0),0)
                self.lock.release()

            #If file does not exists
            else:
                #log("File does not exist, creating\n")
                self.Afile = openfile(self.Afn,create)
                self.Bfile = openfile(self.Bfn,create)

                self.lock.acquire(True)
                self.Afile.writeat('SE',0)
                self.lock.release()

        else:
            try:
                #log("Checking if file exists\n")
                self.Afile = openfile(self.Afn,create)
                self.Bfile = openfile(self.Bfn,create)

                self.lock.acquire(True)
                self.Bfile.writeat(self.Afile.readat(None,0),0)
                self.lock.release()

            except:
                self.Afile.close()
                log("File does not exists\n")

        def writeat(self,data,offset):
            # Write the requested data to the B file using the sandbox's writeat call
            try:
                assert(offset <= len(self.Bfile.readat(None,0)))
            except:
                log("Offset is greater than the file buffer\n")
                exitall()

            self.Bfile.writeat(data,offset)

        def readat(self,bytes,offset):
            # Read from the AFile using the sandbox's readat
            try:
                assert(offset < len(self.Afile.readat(Non,0)))
            except:
                log("Offset is greater than the file buffer\n")
                exitall()

        def close(self):
            #Create lock object
            self.lock = createlock()

            #Check validity of Bfile
            if self.Bfile.readat(1,0) == 'S' and str(self.Bfile.readat(None,0))[-1] == 'E':
                self.lock.acquire(True)
                self.Afile.writeat(str(self.Bfile.readat(None,0)),0)
                self.lock.release()

            self.Afile.close()
            self.Bfile.close()

        def ABopenfile(filename,create):
            return ABFile(filename,create)

        # The code here sets up type checking and variable hiding for you.
# You should not need to change anything below here.
sec_file_def = {"obj-type":ABFile,
                "name":"ABFile",
                "writeat",{"type":"func","args":(str,(int,long)),"exceptions":Exception,"return":(int,type(None)),"target":ABFile.writeat},
                "readat":{"type":"func","args":((int,long(type(None)),(int,long)),"exceptions":Exception,"return":str,"target":ABFile.readat},
                "close":{"type":"func","args":None,"exceptions":None,"return":(bool,type(None)),"target":ABFile.close}
                }

CHILD_CONTEXT_DEF["ABopenfile"] = {TYPE:OBJC,ARGS:(str,bool),EXCP:Exception,RETURN:sec_file_def,TARGET:ABopenfile}

# Execute the user code
secure_dispatch_module()

