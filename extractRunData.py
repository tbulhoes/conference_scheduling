import sqlite3
import sys
import os.path
import math

def getGap(lb, ub):
   return 100*(ub-lb)/lb

def getNbMachines(inst):
   f = file(inst, "r")
   f.readline()
   m = int(f.readline().split()[1])
   f.close()
 
   return m

def getUbAtRoot(log):
   f = file(log , "r")
  
   ub = 0
   while True:
      l = f.readline()
      if l == "":
         break
      if l.find("SB phase") != -1:
         if ub == None:
	    print "ERROR ub is None"
	    return None
         return ub

      if l.find("PB=") != -1:
         ub = float(l.split("PB=")[-1].split(">")[0])

	
   if ub == 0:
      print "ERROR:UB AT ROOT NOT FOUND FOR INST:", inst
      return None
   return ub  


def getFirstLPVal(log):
   f = file(log , "r")
   
   while True:
      l = f.readline()
      if l == "":
         break
      if l.find("ColGenEvalAlg final dual bound") != -1:
         
         f.close()
         return float(l.split(":")[1].split("(")[0])

	
   f.close()

   print "ERROR:FIRST LP VAL NOT FOUND FOR INST:", inst
   return None  

def getRootLB(inst):
   f = file(inst , "r")
   while True:
      l = f.readline()
      if l == "":
         break
      if (l.find("&") != -1) or (l.find("key") != -1):
         continue
      if l.find("bcRecRootDb") != -1:
         f.close()
         return float(l.split(" ")[1])

   f.close()
   print "ERROR: bcRecRootDb NOT FOUND FOR INST:", inst
   return None  

def getRootTime(log):

   f = file(log , "r")
   while True:
      l = f.readline()
      if l == "":
         break
      if l.find("bcTimeRootEval") != -1: 
         f.close()
         return float(l.split("&")[0].split()[1])/100.0
   f.close()

   f = file(log , "r")
   while True:
      l = f.readline()
      if l == "":
         break
      if l.find("bcTimeRoot") != -1: 
         f.close()
         return float(l.split("&")[0].split()[1])/100.0
   f.close()
   
   print "ERROR: bcTimeRoot NOT FOUND FOR INST:", log
   f.close()

   return None  

def getNbLazyCuts(inst, rootOnly):
   f = file(inst , "r")
   nbCuts = 0 
   while True:
      l = f.readline()
      if l == "": 
         break
      if rootOnly and l.find("SB phase") != -1: 
         break
      if l.find("lazy cut added") != -1: 
         nbCuts += 1
   f.close()
   return nbCuts

def instanceFinished(inst):
   f = file(inst , "r")
   finished = False
   while True:
      l = f.readline()
      if l == "":
         break
      if l.find("Program exited normaly") != -1:
         finished = True
         break

   f.close()
   return finished  

def getNbNodes(inst):
   f = file(inst , "r")
   nbNodes = 0

   while True:
      l = f.readline()
      if l == "":
         break

      if (l.find("&") != -1) or (l.find("key") != -1):
         continue
      if l.find("bcCountNodeProc") != -1: 
        nbNodes = int(l.split(" ")[1])

   f.close()
   return nbNodes


def getLb(inst):
   f = file(inst , "r")
   lb = 0

   while True:
      l = f.readline()
      if l == "":
         break

      if (l.find("&") != -1) or (l.find("key") != -1):
         continue
      if l.find("bcRecBestDb") != -1:
        lb = float(l.split(" ")[1])

   f.close()
   return lb

def getUb(inst):
   f = file(inst , "r")
   ub = 0

   while True:
      l = f.readline()
      if l == "":
         break

      if (l.find("&") != -1) or (l.find("key") != -1):
         continue
      if l.find("bcRecBestInc") != -1:
        ub = float(l.split(" ")[1])

   f.close()
   return ub

def finishedByTimeLimit(inst):
   f = file(inst , "r")
   flag = False
   while True:

      l = f.readline()
      if l == "":
         break

      if l.find("TIME LIMIT IS HIT") != -1:
	flag = True
        break

   f.close()
   return flag

def getTime(inst):
   f = file(inst , "r")
   time = 0

   while True:
      l = f.readline()
      if l == "":
         break

      if (l.find("\\\\") != -1) or (l.find("key") != -1):
         continue
      if l.find("bcTimeMain") != -1:
        time = float(l.split(" ")[1])/100

   f.close()
   return time

def getHeurUb(inst):
   f = file(inst , "r")
   ub = 0

   while True:
      l = f.readline()
      if l.find("cutOffValue") != -1:
        ub = float(l.split(" = ")[-1]) - 1
	break

   f.close()
   return ub

def main():

   if len(sys.argv) != 3:
      print "python insertData.py logsDir instDir"
      sys.exit()

  

   logsDir = sys.argv[1]
   instsDir = sys.argv[2]
   if logsDir[-1] != "/":
      logsDir += "/"
   if instsDir[-1] != "/":
      instsDir += "/"
  
   instances = os.listdir(instsDir)
   instances.sort()
   logs = os.listdir(logsDir)
   for inst in instances:
      if inst.find("results") != -1:
         continue
      log = logsDir + inst.replace(".txt", "")
      if instanceFinished(log):
         try:
            rootLB, rootUB, LB, UB = -getUbAtRoot(log), -getRootLB(log), -getUb(log), -getLb(log)
            print inst.replace(".txt",""),rootLB,rootUB,getGap(rootLB,rootUB),getRootTime(log),getNbLazyCuts(log,True),LB,UB,getGap(LB,UB),getTime(log),getNbNodes(log),getNbLazyCuts(log,False),-getHeurUb(log)        
	 except Exception as e:
            print e
      else:
         print inst
if __name__ == "__main__":
   main()
