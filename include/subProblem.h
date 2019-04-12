#ifndef CVXP_SUBPROB_H_
#define CVXP_SUBPROB_H_

#include "Data.h"
#include "model.h"
#include "bcModelFormulationC.hpp"
#include <iostream>

#define IL_STD //this macro is needed to compile with concert 
#include <ilcplex/ilocplex.h>

class SpOracle : public BcSolverOracleFunctor
{
	Data&  data;
	const int clusterType;
        int iter;

     public:
	SpOracle(Data& data, const int clusterType): 
	     data(data), clusterType(clusterType), iter(0) {}
        ~SpOracle(){}

        virtual bool operator()(BcFormulation spPtr,
				     double & objVal,
				     double & primalBound,
				     double & dualBound,
				     BcSolution & primalSol,
				     BcDualSolution & dualSol,
				     int & phaseOfStageApproach);
};

struct SpData
{
     int n,minClusterSize,maxClusterSize,phase;

     std::vector<std::vector<double> > edgeRedCosts;
     std::vector<double> vertexRedCosts;
     double zeroReducedCost;

     SpData(BcFormulation spPtr, int n, int phase, int minClusterSize, int maxClusterSize);

     void exportSpData(const char* filePath, double opt);

     inline int getN() const
     {
	  return n;
     }
};

class SpSol
{
     private:
	  std::vector<bool> bitmap;
	  std::vector<double> deltaVals;
	  SpData& spData;
	  double cost;
	  int size;

     public:

	  SpSol(SpData& spData); 

	  inline bool hasVertex(const int vertex) const
	  {
	       return bitmap[vertex];
	  }

	  inline double getCost() const
	  {
	       return cost;
	  }

	  inline double getSize() const
	  {
	       return size;
	  }

	  inline double getDeltaValOfVertex(const int vertex) const
	  {
	       return deltaVals[vertex];
	  }

	  void clear();

	  void addVertex(const int vertex);
	  void rmVertex(const int vertex);
	  void swapVertices(const int vertexIn, const int vertexOut);

	  void debug(std::string whereFrom);

	  void print();
};

class SpMipSolver 
{
     private:
	  SpData& spData;

     public:
	  SpMipSolver(SpData& spData): spData(spData) {}
	  ~SpMipSolver(){}

	  SpSol* solve_nonlinear(SpSol* incumbent);
	  SpSol* solve_linear(SpSol* incumbent);
};

class SpHeuristicSolver 
{
     private:
	  SpData& spData;

	  SpSol* ch1(const int initialVertex);
	  SpSol* ch2();

     public:
	  SpHeuristicSolver(SpData& spData): spData(spData) {}
	  ~SpHeuristicSolver(){}

	  SpSol* solve();

	  static const double BUILD_EPS = 0.0001;
};

class SpEnumSolver 
{
     private:
	  SpData& spData;

     public:
	  SpEnumSolver(SpData& spData): spData(spData) {}
	  ~SpEnumSolver(){}

	  SpSol* solve();
};


class SpLocalSearch 
{
     private:
	  SpData& spData;

	  bool add(SpSol* sol, bool stopAfterOneMove);
	  bool rm(SpSol* sol, bool stopAfterOneMove);
	  bool swap(SpSol* sol, bool stopAfterOneMove);

     public:
	  SpLocalSearch(SpData& spData): spData(spData) {}
	  void run(SpSol* sol);

	  static const double LS_EPS = 0.0001;
};

class MyIncumbentCallback : public IloCplex::IncumbentCallbackI 
{
     public:

	  MyIncumbentCallback(const IloEnv env, const IloBoolVarArray& x, SpData& spData);

	  IloCplex::CallbackI *duplicateCallback() const; 

	  void main();

	  SpSol* getSpSol();
	  bool hasFoundColumn();

     private:

	  IloBoolVarArray x;

	  SpSol* spSol;

	  SpData& spData;

	  bool foundCol;
};

#endif 
