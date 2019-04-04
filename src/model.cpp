#include "model.h" 
#include "Data.h"
#include "subProblem.h"
#include "lazyCut.h"
#include "bcModelingLanguageC.hpp"
#include <string>

void buildModel(Data& data, const ApplicationSpecificParam& params, BcModel & model)
{
     BcObjectiveArray objective(model);
     objective() == 1000*data.getN();
     if(params.cutOffValue() != -1)
	  objective() <= params.cutOffValue();

     BcMasterArray master(model);

     BcColGenSpArray colGenSp(model);

     //convexity constraints
     BcConstrArray convConstr(master(), "CONV");
     convConstr.toBeUsedInPreprocessing(false);
     for (int j = 0; j < data.getN(); j++)
	  convConstr(j) == 1;

     for(int clusterType = 0; clusterType < data.getNbClusterTypes(); clusterType++)
     {
	  SpOracle * oraclePtr = new SpOracle(data, clusterType);
	  colGenSp(clusterType) << oraclePtr;
	  colGenSp[clusterType] <= data.getMaxNbClusters(clusterType);

	  //edge variables	
	  BcVarArray xVar(colGenSp[clusterType], "X");
	  xVar == 'B';
	  xVar ^= MultiIndexNames('i','j');
          xVar.priorityForMasterBranching(-1);
          xVar.priorityForSubproblemBranching(-1);

	  //node variables	
	  BcVarArray yVar(colGenSp[clusterType], "Y");
	  yVar == 'B';
	  yVar ^= MultiIndexNames('i');
	  yVar.priorityForMasterBranching(-1);
	  yVar.priorityForSubproblemBranching(-1);

	  //writing down convexity constraints
	  for (int i = 0; i < data.getN(); i++)
	  {
	       convConstr(i) += yVar(i);
	  }

	  //writing down obj function
	  for (int i = 0; i < data.getN(); i++)
	  {
	       for(int j = i+1; j < data.getN(); j++)
	       {
		    objective() += data.getWeight(i,j)*xVar(i,j);
	       }
	  }

     }

     //branching on aggregate edge variables
     BcAggrSubProbVarBranching aggrSubProbVarBranching(master(), "X", 0.5, 1.0);

     BcCutConstrArray lazyConstr(master(), "LAZY", 'C');
     LazyCutSeparationRoutine * lazyCutRoutinePtr = new LazyCutSeparationRoutine(data);
     lazyConstr << lazyCutRoutinePtr;
}
