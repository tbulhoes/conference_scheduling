#include "subProblem.h"
#include "bcModelingLanguageC.hpp"
#include "bcUsefulHeadFil.hpp"
#include <string>
#include <sstream>

bool SpOracle::operator()(BcFormulation spPtr,
	  double & objVal,
	  double & primalBound,
	  double & dualBound,
	  BcSolution & primalSol,
	  BcDualSolution & dualSol,
	  int & phaseOfStageApproach)
{
     SpData spData(spPtr, data.getN(), phaseOfStageApproach, data.getMinClusterSize(clusterType), data.getMaxClusterSize(clusterType));

     SpHeuristicSolver heurSolver(spData);
     SpSol* heurSpSol = heurSolver.solve();
     SpSol* spSol = NULL;

     if(phaseOfStageApproach == 0)
     {
	  SpMipSolver mipSolver(spData);
	  spSol = mipSolver.solve2(heurSpSol);

	  //SpSol* linSol = mipSolver.solve2(heurSpSol);
	  //std::cout << std::setprecision(10) << "cost nonlin:" << spSol->getCost() << " cost lin" << linSol->getCost() << std::endl;

	  //if(std::abs(spSol->getCost() - linSol->getCost()) > 0.001)
	  //{
	    //   std::cerr << "erro lin nonlin\n"; 
	      // exit(EXIT_FAILURE);
	  //}
	  delete heurSpSol;
     }
     else
	  spSol = heurSpSol;



//     std::cout << "heur ";
  //   heurSol->print();

    // std::cout << "exact ";
     //spSol.print();

     BcVarArray xVar(spPtr, "X");
     BcVarArray yVar(spPtr, "Y");

     double colCost = spSol->getCost();

     BcSolution mySol(spPtr);
     for(int i = 0; i < data.getN(); i++)
     {
	  if(spSol->hasVertex(i))
	  {
	       yVar[i] = 1;
	       mySol += yVar[i];
	  }

	  for(int j = i+1; j < data.getN(); j++)
	  {
	       if(spSol->hasVertex(i) && spSol->hasVertex(j))
	       {
		    xVar[i][j] = 1;
		    mySol += xVar[i][j];
	       }
	  }
     }

     primalSol += mySol;
     objVal = primalBound = dualBound = colCost;

     std::ostringstream str1; 
     str1 << iter;
     //spData.exportSpData((std::string("spData") + str1.str()).c_str(), colCost);
     iter++;

     delete spSol;

     return true;
}

SpSol* SpMipSolver::solve(SpSol* incumbent)
{
     IloEnv env;
     IloModel model(env);

     IloBoolVarArray x(env, spData.n);
     for(int i = 0; i < spData.n; i++)
     {
	  model.add(x[i]);
     }

     IloExpr fo(env);
     for(int i = 0; i < spData.n; i++)
     {
	  fo += spData.vertexRedCosts[i]*x[i];
	  for(int j = i + 1; j < spData.n; j++)
	  {
	       fo += spData.edgeRedCosts[i][j]*x[i]*x[j];
	  }
     }
     model.add(IloMinimize(env, fo));

     model.add(IloSum(x) <= spData.maxClusterSize);
     model.add(IloSum(x) >= spData.minClusterSize);

     IloCplex KS(model);
     ////////////////////////////////////////////
     //incumbent
     if(incumbent)
     {
	  IloNumVarArray vars(env);
	  IloNumArray values(env);

	  for(int i = 0; i < spData.getN(); i++)
	  {
	       vars.add(x[i]);
	       if(incumbent->hasVertex(i))
		    values.add(1);
	       else
		    values.add(0);
	  }

	  KS.addMIPStart(vars, values);
     }
     ////////////////////////////////////////////

     KS.setParam(IloCplex::Threads, 1);
//     KS.setParam(IloCplex::EpGap, 0.000001);
     KS.setParam(IloCplex::EpGap, 0.0);
     KS.setOut(env.getNullStream());
     KS.setWarning(env.getNullStream());
     const IloBoolVarArray& x_ref = x;
     MyIncumbentCallback* incumbentCbk = new (env) MyIncumbentCallback(env, x_ref, spData);
     KS.use(incumbentCbk);

     try
     {
	  KS.solve();
     }
     catch(IloException& e)
     {
	  std::cerr << "Exception raised by cplex:" << e;
	  exit(EXIT_FAILURE);
     }

     if(not incumbentCbk->hasFoundColumn())
     {
	  std::cerr << "no column was found by CPLEX!";
	  exit(EXIT_FAILURE);
     }
    
     if(spData.phase == 0 && KS.getCplexStatus() != IloCplex::Optimal && KS.getCplexStatus() != IloCplex::OptimalTol)
     {
	  std::cerr << "column found by CPLEX is not optimal and phase=0!";
	  exit(EXIT_FAILURE);
     }

     std::cout << "time nonlin:" << KS.getTime() << std::endl;
     SpSol* spSol = incumbentCbk->getSpSol();
     delete incumbentCbk;
     env.end();

     return spSol;
}

SpSol* SpMipSolver::solve2(SpSol* incumbent)
{
   int knap_cpt = spData.maxClusterSize;

   IloEnv env;
   IloModel model(env);

   IloBoolVarArray x(env, spData.getN());
   for(int i = 0; i < spData.getN(); i++)
   {
      char var[100];
      sprintf(var, "X(%d)", i);
      x[i].setName(var);
      model.add(x[i]);
   }

   IloNumVarArray z(env, spData.getN() - 1, -IloInfinity, IloInfinity);
   for(int i = 0; i < spData.getN() - 1; i++)
   {
      char var[100];
      sprintf(var, "Z(%d)", i);
      z[i].setName(var);
      model.add(z[i]);
   }
   
   //fo
   IloExpr fo(env);
   fo += IloSum(z);
   for(int i = 0; i < spData.getN(); i++)
   {
      fo += spData.vertexRedCosts[i]*x[i];
   }
   model.add(IloMinimize(env, fo));

   //restricoes
   model.add(spData.minClusterSize <= IloSum(x) <= knap_cpt);

   std::vector<double> sum_pos(spData.getN() - 1, 0);
   std::vector<double> sum_neg(spData.getN() - 1, 0);
   for(int i = 0; i < spData.getN() - 1; i++)
   {
      for(int j = i+1; j < spData.getN(); j++)
      {
	 if(spData.edgeRedCosts[i][j] > 0)
	 {
	    sum_pos[i] += spData.edgeRedCosts[i][j];
	 }
	 else
	 {
	    sum_neg[i] += spData.edgeRedCosts[i][j];
	 }
      }
   }

   for(int i = 0; i < spData.getN() - 1; i++)
   {
      model.add(z[i] >= sum_neg[i]*x[i]);
      
      IloExpr fi_expr(env);
      for(int j = i+1; j < spData.getN(); j++)
      {
	 fi_expr += spData.edgeRedCosts[i][j]*x[j];
      }

      model.add(z[i] >= fi_expr + (x[i] - 1)*sum_pos[i]);
   }


   IloCplex KS(model);
   ////////////////////////////////////////////
   //incumbent
   if(incumbent)
   {
	IloNumVarArray vars(env);
	IloNumArray values(env);

	for(int i = 0; i < spData.getN(); i++)
	{
	     vars.add(x[i]);
	     if(incumbent->hasVertex(i))
		  values.add(1);
	     else
		  values.add(0);
	}

	KS.addMIPStart(vars, values);
   }
   ////////////////////////////////////////////

   KS.setParam(IloCplex::Threads, 1);
   //     KS.setParam(IloCplex::EpGap, 0.000001);
   KS.setParam(IloCplex::EpGap, 0.0);
   KS.setOut(env.getNullStream());
   KS.setWarning(env.getNullStream());
   const IloBoolVarArray& x_ref = x;
   MyIncumbentCallback* incumbentCbk = new (env) MyIncumbentCallback(env, x_ref, spData);
   KS.use(incumbentCbk);

   try
   {
	KS.solve();
   }
   catch(IloException& e)
   {
	std::cerr << "Exception raised by cplex:" << e;
	exit(EXIT_FAILURE);
   }

   if(not incumbentCbk->hasFoundColumn())
   {
	std::cerr << "no column was found by CPLEX!";
	exit(EXIT_FAILURE);
   }

   if(spData.phase == 0 && KS.getCplexStatus() != IloCplex::Optimal && KS.getCplexStatus() != IloCplex::OptimalTol)
   {
	std::cerr << "column found by CPLEX is not optimal and phase=0!";
	exit(EXIT_FAILURE);
   }

   //  std::cout << "time lin:" << KS.getTime() << std::endl;
   SpSol* spSol = incumbentCbk->getSpSol();
   delete incumbentCbk;
   env.end();

   return spSol;
}


SpSol* SpHeuristicSolver::ch1(const int initialVertex)
{
     SpSol* sol = new SpSol(spData);

     sol->addVertex(initialVertex);

     while(true)
     {
	  double bestDelta = DBL_MAX;
	  int bestVertex = -1;

	  for(int j = 0; j < spData.getN(); j++)
	  {
	       if(!sol->hasVertex(j))
	       {
		    double deltaVal = sol->getDeltaValOfVertex(j);
		    if(deltaVal < bestDelta)
		    {
			 bestDelta = deltaVal;
			 bestVertex = j;
		    }
	       }
	  }
	  
	  bool addVertex = (sol->getSize() < spData.minClusterSize)
	                  || (bestDelta < -BUILD_EPS && (sol->getSize() < spData.maxClusterSize));
	  if(addVertex)
	  {
	       sol->addVertex(bestVertex);
	  }
	  else
	       break;
	  
     }

//     std::cout << "ch1 cost:" << sol->getCost() << " size:" << sol->getSize() << std::endl; 
     return sol;
}

SpSol* SpHeuristicSolver::ch2()
{
     SpSol* sol = new SpSol(spData);

     for(int j = 0; j < spData.getN(); j++)
     {
	  sol->addVertex(j);
     }

     while(true)
     {
	  double bestDelta = DBL_MAX;
	  int bestVertex = -1;

	  for(int j = 0; j < spData.getN(); j++)
	  {
	       if(sol->hasVertex(j))
	       {
		    double deltaVal = sol->getDeltaValOfVertex(j);
		    if(deltaVal < bestDelta)
		    {
			 bestDelta = deltaVal;
			 bestVertex = j;
		    }
	       }
	  }
	  
	  bool rmVertex = (sol->getSize() > spData.maxClusterSize)
	                  || (bestDelta < -BUILD_EPS && (sol->getSize() > spData.minClusterSize));
	  if(rmVertex)
	  {
	       sol->rmVertex(bestVertex);
	  }
	  else
	       break;
	  
     }

//     std::cout << "ch2 cost:" << sol->getCost() << " size:" << sol->getSize() << std::endl; 

     return sol;
}

SpSol* SpHeuristicSolver::solve()
{
     SpSol* bestSol = NULL;

     for(int iter = 0; iter <= spData.getN(); iter++)
     {
	  SpSol* sol = NULL;
	  if(iter < spData.getN())
	       sol = ch1(iter);
	  else
	       sol = ch2();

	  SpLocalSearch ls(spData);
	  ls.run(sol);

	  if((iter == 0) || (sol->getCost() < bestSol->getCost()))
	  {
	       delete bestSol;
	       bestSol = sol;
	  }
	  else
	       delete sol;
     }

     return bestSol;
}

SpData::SpData(BcFormulation spPtr, int n, int phase, int minClusterSize, int maxClusterSize): n(n), phase(phase), minClusterSize(minClusterSize), 
     maxClusterSize(maxClusterSize), zeroReducedCost(spPtr.zeroReducedCostThreshold())
{
     BcVarArray xVar(spPtr, "X");
     BcVarArray yVar(spPtr, "Y");

     edgeRedCosts = std::vector<std::vector<double> >(n, std::vector<double>(n));
     vertexRedCosts = std::vector<double>(n);
     for(int i = 0; i < n; i++)
     {
	  vertexRedCosts[i] = yVar[i].curCost();
	  for(int j = i + 1; j < n; j++)
	  {
	       edgeRedCosts[i][j] = edgeRedCosts[j][i] = xVar[i][j].curCost();
	  }
     }
}

void SpData::exportSpData(const char* filePath, double opt)
{
     FILE* f = fopen(filePath, "w");

     fprintf(f, "%d %d %d %lf\n", n, minClusterSize, maxClusterSize, opt);
     for(int i = 0; i < n; i++)
     {
	  fprintf(f, "%lf ", vertexRedCosts[i]);
     }
     fprintf(f, "\n");

     for(int i = 0; i < n; i++)
     {
	  for(int j = 0; j < n; j++)
	  {
	       fprintf(f, "%lf ", edgeRedCosts[i][j]);
	  }
	  fprintf(f, "\n");
     }

     fclose(f);
}

//construtor da classe MyIncumbentCallBack
MyIncumbentCallback::MyIncumbentCallback(IloEnv env, const IloBoolVarArray& x, SpData& spData) :
          IloCplex::IncumbentCallbackI(env), x(x), spData(spData), foundCol(false), spSol(NULL)
{}

//retorna uma cópia do callback. Este método é uma exigência do cplex.
IloCplex::CallbackI* MyIncumbentCallback::duplicateCallback() const
{
        return new (getEnv()) MyIncumbentCallback(getEnv(), x, spData);
}

void MyIncumbentCallback::main()
{
     double reduced_cost = 0;
     
     for(int i = 0; i < spData.n; i++)
     {
	  if(getValue(x[i]) > 0.9)
	  {
	       reduced_cost += spData.vertexRedCosts[i];
	       for(int j = i+1; j < spData.n; j++)
	       {
		    if(getValue(x[j]) > 0.9)
			 reduced_cost += spData.edgeRedCosts[i][j];
	       }
	  }
     }

     if(!foundCol || reduced_cost < spSol->getCost())
     {
	  if(foundCol)
	       delete spSol;
	  foundCol = true;
	  spSol = new SpSol(spData);
	  for(int i = 0; i < spData.getN(); i++)
	  {
	       if(getValue(x[i]) > 0.9)
	       {
		    spSol->addVertex(i);
	       }
	  }

	  if(reduced_cost < spData.zeroReducedCost
		    && spData.phase == 1)
	       abort();
     }
}

bool MyIncumbentCallback::hasFoundColumn()
{
     return foundCol;
}

SpSol* MyIncumbentCallback::getSpSol()
{
     return spSol;
}

SpSol::SpSol(SpData& spData): 
     cost(0), size(0), spData(spData), bitmap(spData.getN(), false), deltaVals(spData.getN(), 0)
{
     for(int i = 0; i < spData.getN(); i++)
	  deltaVals[i] = spData.vertexRedCosts[i];
}

void SpSol::clear() 
{
     cost = 0;
     for(int i = 0; i < spData.getN(); i++)
     {
	  bitmap[i] = false;
	  deltaVals[i] = spData.vertexRedCosts[i];
     }
}

void SpSol::print() 
{
//     std::cout << std::setprecision(10) << "solution with cost = " << cost << std::endl;
     for(int i = 0; i < spData.getN(); i++)
     {
	  if(bitmap[i])
	       std::cout << i << " ";
     }

     std::cout << "\n";
}

void SpSol::debug(std::string whereFrom) 
{
     std::vector<double> dVals(spData.getN(), 0);
     for(int i = 0; i < spData.getN(); i++)
     {
	  if(bitmap[i])
	  {
	       dVals[i] -= spData.vertexRedCosts[i];
	       for(int j = 0; j < spData.getN(); j++)
	       {
		    if(i != j && bitmap[j])
			 dVals[i] -= spData.edgeRedCosts[i][j];
	       }
	  }
	  else
	  {
	       dVals[i] += spData.vertexRedCosts[i];
	       for(int j = 0; j < spData.getN(); j++)
	       {
		    if(i != j && bitmap[j])
			 dVals[i] += spData.edgeRedCosts[i][j];
	       }
	  }
     }

     for(int i = 0; i < spData.getN(); i++)
     {
	  if(abs(deltaVals[i] - dVals[i]) > 0.0001)
	  {
	       std::cerr << "bug! " << whereFrom << std::endl ;
	       exit(EXIT_FAILURE);
	  }
     }

     double dcost = 0;
     for(int i = 0; i < spData.getN(); i++)
     {
	  if(bitmap[i])
	  {

	       dcost += spData.vertexRedCosts[i];

	       for(int j = i+1; j < spData.getN(); j++)
	       {
		    if(bitmap[j])
			 dcost += spData.edgeRedCosts[i][j];
	       }
	  }
     }

     if(abs(dcost - cost) > 0.0001)
     {
	  std::cerr << "bug! " << whereFrom << std::endl ;
	  exit(EXIT_FAILURE);
     }
}

void SpSol::addVertex(const int vertex)
{
     if(bitmap[vertex])
	  return;

     bitmap[vertex] = true;
     cost += deltaVals[vertex];

     deltaVals[vertex] *= -1;
     for(int i = 0; i < spData.getN(); i++)
     {
	  if(i != vertex)
	  {
	       if(bitmap[i])
		    deltaVals[i] -= spData.edgeRedCosts[i][vertex];
	       else
		    deltaVals[i] += spData.edgeRedCosts[i][vertex];
	  }
     }

     size++;

     //debug(std::string("addVertex"));
}

void SpSol::rmVertex(const int vertex)
{
     if(!bitmap[vertex])
	  return;

     bitmap[vertex] = false;
     cost += deltaVals[vertex];

     deltaVals[vertex] *= -1;
     for(int i = 0; i < spData.getN(); i++)
     {
	  if(i != vertex)
	  {
	       if(bitmap[i])
		    deltaVals[i] += spData.edgeRedCosts[i][vertex];
	       else
		    deltaVals[i] -= spData.edgeRedCosts[i][vertex];
	  }
     }

     size--;

     //debug(std::string("rmVertex"));
}

void SpSol::swapVertices(const int vertexIn, const int vertexOut)
{
     if(bitmap[vertexIn] || !bitmap[vertexOut])
	  return;

     bitmap[vertexIn] = true;
     bitmap[vertexOut] = false;

     cost += deltaVals[vertexIn] + deltaVals[vertexOut] - spData.edgeRedCosts[vertexIn][vertexOut];

     deltaVals[vertexIn] = -deltaVals[vertexIn] + spData.edgeRedCosts[vertexIn][vertexOut];
     deltaVals[vertexOut] = -deltaVals[vertexOut] + spData.edgeRedCosts[vertexIn][vertexOut];
     for(int i = 0; i < spData.getN(); i++)
     {
	  if(i != vertexIn && bitmap[i])
	  {
	       deltaVals[i] += spData.edgeRedCosts[vertexOut][i] - spData.edgeRedCosts[vertexIn][i];
	  }
	  else if(i != vertexOut && !bitmap[i])
	  {
	       deltaVals[i] += -spData.edgeRedCosts[vertexOut][i] + spData.edgeRedCosts[vertexIn][i];
	  }
     }

     //debug(std::string("swapVertex"));
}


bool SpLocalSearch::add(SpSol* sol, bool stopAfterOneMove)
{
     int nbMoves = 0;
     while(true)
     {
	  double bestDelta = DBL_MAX;
	  int bestVertex = -1;

	  for(int j = 0; j < spData.getN(); j++)
	  {
	       if(!sol->hasVertex(j))
	       {
		    double deltaVal = sol->getDeltaValOfVertex(j);
		    if(deltaVal < bestDelta)
		    {
			 bestDelta = deltaVal;
			 bestVertex = j;
		    }
	       }
	  }
	  
	  if(bestDelta < -LS_EPS && sol->getSize() < spData.maxClusterSize)
	  {
	       sol->addVertex(bestVertex);
//               std::cout << "add cost:" << sol->getCost() << " size:" << sol->getSize() << std::endl; 
	       nbMoves++;
	       if(stopAfterOneMove)
		    return true;
	  }
	  else
	       break;
	  
     }

     return nbMoves > 0;
}

bool SpLocalSearch::rm(SpSol* sol, bool stopAfterOneMove)
{
     int nbMoves = 0;
     while(true)
     {
	  double bestDelta = DBL_MAX;
	  int bestVertex = -1;

	  for(int j = 0; j < spData.getN(); j++)
	  {
	       if(sol->hasVertex(j))
	       {
		    double deltaVal = sol->getDeltaValOfVertex(j);
		    if(deltaVal < bestDelta)
		    {
			 bestDelta = deltaVal;
			 bestVertex = j;
		    }
	       }
	  }
	  
	  if(bestDelta < -LS_EPS && sol->getSize() > spData.minClusterSize)
	  {
	       sol->rmVertex(bestVertex);
               //std::cout << "rm cost:" << sol->getCost() << " size:" << sol->getSize() << std::endl; 
	       nbMoves++;
	       if(stopAfterOneMove)
		    return true;
	  }
	  else
	       break;
	  
     }

     return nbMoves > 0;
}

bool SpLocalSearch::swap(SpSol* sol, bool stopAfterOneMove)
{
     int nbMoves = 0;
     while(true)
     {
	  double bestDelta = DBL_MAX;
	  int bestVertexIn = -1, bestVertexOut = -1;

	  for(int j = 0; j < spData.getN(); j++)
	  {
	       if(sol->hasVertex(j))
	       {
		    for(int i = 0; i < spData.getN(); i++)
		    {
			 if(!sol->hasVertex(i))
			 {
			      double deltaVal = sol->getDeltaValOfVertex(j) + sol->getDeltaValOfVertex(i) - spData.edgeRedCosts[i][j];
			      if(deltaVal < bestDelta)
			      {
				   bestDelta = deltaVal;
				   bestVertexIn = i;
				   bestVertexOut = j;
			      }
			 }
		    }
	       }
	  }
	  
	  if(bestDelta < -LS_EPS)
	  {
	       sol->swapVertices(bestVertexIn, bestVertexOut);
//               std::cout << bestDelta << " " << -LS_EPS <<  " swap cost:" << sol->getCost() << " size:" << sol->getSize() << " in:" << bestVertexIn << " out:" << bestVertexOut << std::endl; 
	       nbMoves++;
	       if(stopAfterOneMove)
		    return true;
	  }
	  else
	       break;
	  
     }

     return nbMoves > 0;
}

void SpLocalSearch::run(SpSol* sol)
{
     while(true)
     {
	  while(rm(sol, false));

	  if(add(sol, true))
	  {
	       continue;
	  }

	  if(!swap(sol, true))
	  {
	       break;
	  }
     }
}
