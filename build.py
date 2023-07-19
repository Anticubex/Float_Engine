""" A basic python library for building a C/C++ project """

from os import path
import os, shutil, sys

sys.path.append(os.path.join(".", "build"))

from globalsettings import *
from includer import lastTimeMod
# from objer import *

forceRecompile = "-f" in sys.argv
# forceRecompile = True
if forceRecompile:
        print("Forcing recompilation")

runAfterDone = "-r" in sys.argv

print("Bulding project in directory", sourcedir)

srcs = []
objs = []

def getSrcFiles():
        global srcs
        srcsTmp = []
        # Get all directories with src files
        for (root,dirs,files) in os.walk(sourcedir):
                # print(root)
                # print(dirs)
                # print(files)
                # print("-" * (max(len(root), len(dirs), len(files)) + 5))
                for file in files:
                        if checkExts(file):
                                srcsTmp.append([root,files])
                                break

        #Filter for source files only
        for root, files in srcsTmp:
                filesTmp = []
                # print(root,"\t", files)
                for file in files:
                        if checkExts(file):
                                filesTmp.append(file)
                srcs.append([root, filesTmp])


def compileFile(root, file):
        absName = path.join(root,file)
        objName = path.join(objdir, path.splitext(file)[0] + ".o")
        # print("Compiling", absName, "to", objName)

        objs.append(objName)
        
        #Check if OBJ exists
        precomped = path.exists(objName)
        if precomped:
                ...
                #Check if file was modified before obj
                compModTime = path.getmtime(objName)
                fileModTime = lastTimeMod(absName)

                # import time
                # modificationTime = time.strftime('%Y-%m-%d %H:%M:%S', time.localtime(compModTime))
                # print("Obj Last Modified Time : ", modificationTime )
                # modificationTime = time.strftime('%Y-%m-%d %H:%M:%S', time.localtime(fileModTime))
                # print("File Last Modified Time : ", modificationTime )

                if compModTime > fileModTime:
                        print("Obj already found for", absName)
                        return

        #Compile file
        cmd = " ".join([cxx, pflags, "-c", absName, flags, lflags, "-o", objName])
        print("Compiling", absName, "to", objName, "with", cmd)
        os.system(cmd)

def compile():

        if not path.exists(objdir):
                os.mkdir(objdir)

        if forceRecompile:
                cleanObjs()

        for root, files in srcs:
                for file in files:
                        compileFile(root, file)

def cleanObjs():
        print("Removing objs")
        for filename in os.listdir(objdir):
                file_path = path.join(objdir, filename)
                try:
                        if path.isfile(file_path) or path.islink(file_path):
                                os.unlink(file_path)
                        elif os.path.isdir(file_path):
                                shutil.rmtree(file_path)
                except Exception as e:
                        print('Failed to delete %s. Reason: %s' % (file_path, e))

def link():
        print("Linking obj files")
        objsStr = " ".join(objs)
        cmd = " ".join([cxx, pflags, objsStr, flags, lflags, "-o", out])
        print("Linking with", cmd)
        os.system(cmd)
        


def checkExts(file):
        return any(file.lower().endswith(ext) for ext in sourceTypes)


if __name__ == "__main__":
        getSrcFiles()
        # print("srcs:", srcs)
        compile()
        link()

        if runAfterDone:
                os.system(path.join(".", out))
