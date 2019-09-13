import sqlite3
import sys
import os.path
import math


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

def getNbCuts(inst):
   f = file(inst , "r")
   while True:
      l = f.readline()
      if l == "":
         break
      if l.find("bcCountCutInMaster") != -1:
         f.close()
         return int(l.split(" ")[1])

   print "ERROR: bcCountCutInMaster NOT FOUND FOR INST:", inst
   f.close()
   return None  

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

   #conecta-se ao banco sqlite3 e pega o cursor
   try:
      db=sqlite3.connect("data/bds/ubs.db")
      cursor=db.cursor()
   except:
      print "unable to connect to sqlite database"

   sql = "select ub from ubsAK where inst=?"
   cursor.execute(sql, (inst.replace(".dat", ""),))
   ub = int(cursor.fetchone()[0])
   cursor.close()
   db.close()

   return ub
 
def main():

   if len(sys.argv) != 3:
      print "python insertData.py logsDir instDir"
      sys.exit()

   global logsDir
   global instsDir

   logsDir = sys.argv[1]
   instsDir = sys.argv[2]
   if logsDir[-1] != "/":
      logsDir += "/"
   if instsDir[-1] != "/":
      instsDir += "/"
  
   instances = os.listdir(instsDir)
   instances.sort()
   logs = os.listdir(logsDir)
   avgGap1 = 0.0
   avgGap2=0.0
   avgTime = 0
   avgRootTime = 0
   nbInst = 0
   for inst in instances:
         if inst.find("25.dat") != -1:
	    continue
         for log in logs:
	    if log.find(inst.replace(".dat","")) != -1:
               if instanceFinished(log):
                  try:
                     print inst.replace(".dat",""),getT(log),getFirstLPVal(log),getRootLB(log),getUbAtRoot(log),getRootTime(log),getNbRank1Cuts(log,1,True), getNbRank1Cuts(log,1,False),getNbRank1Cuts(log,3,True),getNbRank1Cuts(log,3,False),getLb(log),getUb(log),getNbNodes(log),getTime(log), getHeurUb(inst)

                  except Exception as e:
                     print e
               else:
	          print inst
if __name__ == "__main__":
   main()
