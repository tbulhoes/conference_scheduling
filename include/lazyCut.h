/*
 *  BaPCod  -  a generic Branch-And-Price Code
 *
 *  VERSION %I% (%G%)
 *
 *  Developed by Francois VANDERBECK (fv@math.u-bordeaux1.fr).
 *  The Cvrp application was developed by Artur Pessoa and Ruslan Sadykov
 *  All Rights Reserved.
 */

#ifndef lazyCut_H_
#define lazyCut_H_

#include "Data.h"
#include "bcModelCutConstrC.hpp"

class LazyCutSeparationRoutine: public BcCutSeparationFunctor
{
  const Data& data;
  int cutCount;
  int cutRound;

  std::vector<std::vector<int> > extractClusters(std::vector<std::vector<double> >& xSol);
  bool feasibleClustering(std::vector<std::vector<int> >& clusters);

public:

  LazyCutSeparationRoutine(const Data & data);
  virtual ~LazyCutSeparationRoutine();
  virtual int operator() (BcFormulation formPtr,
			  BcSolution & primalSol,
			  double & maxViolation,
			  std::list< BcConstr > & cutList);
};


#endif
