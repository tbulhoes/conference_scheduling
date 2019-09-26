import os
import sys
from subprocess import call 
import sqlite3

def getUb(inst):

   f = file("entrada/lars/heuristic_results/" + inst + ".res.txt", "r")
   val = 0
   while True:
      l = f.readline()
      if l == "":
         break
      if int(l) > val:
         val = int(l)

   f.close()
   return -val + 1

def main():

   dir_in = sys.argv[1]
   instances=os.listdir(dir_in)

   #executa cada uma das entradas
   for inst in instances:
      if inst.find("results") != -1:
         continue
      inst_name = inst.split(".")[0]
      if os.path.exists("logsscreenlars/" + inst_name):
         print("skipping " + inst_name)
         continue
   
      ub = getUb(inst)

#      print("./conference_scheduling -b config/GenericBc.cfg -a config/GenericApp.cfg -i " + dir_in+inst + " --cutOffValue " + str(ub) + " > logsscreenlars/" + inst_name + " 2> logsscreenlars/logsaida2_" + inst_name)
#     continue
      
      ret_code = call("./conference_scheduling -b config/GenericBc.cfg -a config/GenericApp.cfg -i " + dir_in+inst + " --cutOffValue " + str(ub) + " > logsscreenlars/" + inst_name + " 2> logsscreenlars/logsaida2_" + inst_name, shell=True)
      f = file("ret_codes", "a")
      f.write(inst_name + " " + str(ret_code) + "\n")
      f.close()

if __name__=="__main__":
   main()
