/*
 *  BaPCod  -  a generic Branch-And-Price Code
 *
 *  VERSION %I% (%G%)
 *
 *  Developed by Francois VANDERBECK (fv@math.u-bordeaux1.fr).
 *  The Cvrp application was developed by Artur Pessoa and Ruslan Sadykov
 *  All Rights Reserved.
 */

#include "lazyCut.h"
#include <vector>

#define IL_STD //this macro is needed to compile with concert 
#include <ilcplex/ilocplex.h>


LazyCutSeparationRoutine::LazyCutSeparationRoutine(const Data & data):
  data(data)
{
  cutCount = 0;
  cutRound = 0;
}

LazyCutSeparationRoutine::~LazyCutSeparationRoutine()
{}

int LazyCutSeparationRoutine::operator()(BcFormulation formPtr,
		BcSolution & primalSol,
		double & maxViolation,
		std::list< BcConstr > & cutList)
{
     const std::set< BcVar > & xvarList = primalSol.extractVar("X");
     std::vector<std::vector<double> > xSol(data.getN(), std::vector<double>(data.getN(), 0.0));
     for (std::set< BcVar >::const_iterator varIt = xvarList.begin();
	       varIt != xvarList.end(); ++varIt)
     {
	  int i = varIt->id().first(), j = varIt->id().second();
	  xSol[i][j] += (*varIt).solVal();
	  xSol[j][i] += (*varIt).solVal();
     }

     //returns if solution is not integral
     for(int i = 0; i < data.getN(); i++)
     {
	  for(int j = i+1; j < data.getN(); j++)
	  {
	       if((xSol[i][j] > 0.0001) && (xSol[i][j] < 0.9999))
	       {
		    return cutCount;
	       }
	  }
     }

     std::vector<std::vector<int> > clusters = extractClusters(xSol);

     if(not feasibleClustering(clusters))
     {
          int nbEdges = 0;
	  for(int clusterId = 0; clusterId < clusters.size(); clusterId++)
	       nbEdges += (clusters[clusterId].size()*(clusters[clusterId].size() - 1)/2);

	  BcCutConstrArray lazyConstrArray(formPtr, "LAZY", 'C');
	  lazyConstrArray.sense('L');

	  BcConstr lazyCut = lazyConstrArray(cutCount);
	  lazyCut <= nbEdges - 1 ;

	  for (std::list<BcFormulation>::const_iterator formIt = formPtr.colGenSubProblemList().begin();
		    formIt != formPtr.colGenSubProblemList().end(); ++formIt)
	  {
	       BcVarArray x(*formIt, "X");

	       for (int i = 0; i < data.getN(); i++)
	       {
		    for (int j = i+1; j < data.getN(); j++)
		    {
			 if(xSol[i][j] >= 0.9999)
			 {
			      lazyCut += x[i][j];
			 }
		    }
	       }
	  }

	  cutList.push_back(lazyCut);
	  cutCount++;
     }

     return cutCount;
}

std::vector<std::vector<int> > LazyCutSeparationRoutine::extractClusters(std::vector<std::vector<double> >& xSol)
{
     std::vector<std::vector<int> > clusters;

     std::vector<bool> reached(data.getN(), false);

     for(int i = 0; i < data.getN(); i++)
     {
	  //dfs from i
	  if(!reached[i])
	  {
	       std::vector<int> cluster;
	       std::vector<int> dfs_stack;
	       dfs_stack.push_back(i);
	       reached[i] = true;

	       while(dfs_stack.size() > 0)
	       {
		    int v = dfs_stack.back();
		    dfs_stack.pop_back();
		    cluster.push_back(v);

		    for(int j = 0; j < data.getN(); j++)
		    {
			 if((j != v) && (xSol[v][j] >= 0.9999))
			 {
			      if(!reached[j])
			      {
				   dfs_stack.push_back(j);
				   reached[j] = true;
			      }
			 }
		    }
	       }

	       clusters.push_back(cluster);
	  }
     }

     return clusters;
}

bool LazyCutSeparationRoutine::feasibleClustering(std::vector<std::vector<int> >& clusters)
{
     std::cout << "calling checker\n";
     const CheckerData& checkerData = data.getCheckerData();

     IloEnv env;
     IloModel model(env);

     for(int clusterId = 0; clusterId < clusters.size(); clusterId++)
     {
	  std::cout << "cluster " << clusterId << ":";
	  for(int i = 0; i < clusters[clusterId].size(); i++)
	       std::cout << clusters[clusterId][i] << " ";
	  std::cout << "\n";
     }

     //x[i][j] = 1: cluster i allocated in session j
     IloArray<IloBoolVarArray> x(env, clusters.size());
     for(int i = 0; i < clusters.size(); i++)
     {
	  x[i] = IloBoolVarArray(env, checkerData.sessions.size());
	  for(int j = 0; j < checkerData.sessions.size(); j++)
	  {
	       if(checkerData.sessions[j].getSize() >= clusters[i].size())
	       {
		    char var[100];
		    sprintf(var, "X(%d,%d)", i, j);
		    x[i][j].setName(var);
		    model.add(x[i][j]);
	       }
	  }
     }

     //each cluster is allocated
     for(int i = 0; i < clusters.size(); i++)
     {
	  IloExpr sumX(env);
	  for(int j = 0; j < checkerData.sessions.size(); j++)
	  {
	       if(checkerData.sessions[j].getSize() >= clusters[i].size())
	       {
		    sumX += x[i][j];
	       }
	  }

	  IloRange rng(env, 1, sumX, 1); 
	  char name[100];
	  sprintf(name, "cluster-%d", i);
	  rng.setName(name);
	  model.add(rng);
     }

     //papers of a given author cannot be in parallel
     for(int authorId = 0; authorId < checkerData.authors.size(); authorId++)
     {
	  const Author& author = checkerData.authors[authorId];

	  const std::vector<int>& papersIds = author.getPapersIds();
	  std::vector<int> clustersIds;
	  for(int clusterId = 0; clusterId < clusters.size(); clusterId++)
	  {
	       for(int i = 0; i < clusters[clusterId].size(); i++)
	       {
		    if(std::find(papersIds.begin(), papersIds.end(), clusters[clusterId][i]) != papersIds.end())
		    {
			 clustersIds.push_back(clusterId);
			 break;
		    }
	       }
	  }

	  for(int dayId = 0; dayId < checkerData.days.size(); dayId++)
	  {
	       const Day& day = checkerData.days[dayId];
	       const std::vector<int>& sessionsIds = day.getSessionsIds();

	       for(int timeSlot = 0; timeSlot < day.getNbTimeSlots(); timeSlot++)
	       {
		    IloExpr sumX(env);
		    for(int i = 0; i < sessionsIds.size(); i++)
		    {
			 int sessionId = sessionsIds[i];
			 if(checkerData.sessions[sessionId].getTimeSlot() == timeSlot)
			 {
			      for(int j = 0; j < clustersIds.size(); j++)
			      {
				   int clusterId = clustersIds[j];
				   if(checkerData.sessions[sessionId].getSize() >= clusters[clusterId].size())
					sumX += x[clusterId][sessionId];
			      }
			 }

		    }

		    IloRange rng = (sumX <= 1); 
		    char name[100];
		    sprintf(name, "author%dday%dslot%d", authorId, dayId, timeSlot);
		    rng.setName(name);
		    model.add(rng);
	       }
	  }
     }

     //for each theme, there is an upper bound on the number of parallel papers 
     for(int themeId = 0; themeId < checkerData.themes.size(); themeId++)
     {
	  const Theme& theme = checkerData.themes[themeId];
	  const std::vector<int>& papersIds = theme.getPapersIds();

	  std::vector<std::pair<int,int> > clusterIdCoeffPairs;
	  for(int clusterId = 0; clusterId < clusters.size(); clusterId++)
	  {
	       int clusterCoeff = 0;
	       for(int i = 0; i < clusters[clusterId].size(); i++)
	       {
		    if(std::find(papersIds.begin(), papersIds.end(), clusters[clusterId][i]) != papersIds.end())
		    {
			 clusterCoeff++;
		    }
	       }

	       if(clusterCoeff)
	       {
		    clusterIdCoeffPairs.push_back(std::make_pair(clusterId, clusterCoeff));
	       }
	  }

	  for(int dayId = 0; dayId < checkerData.days.size(); dayId++)
	  {
	       const Day& day = checkerData.days[dayId];
	       const std::vector<int>& sessionsIds = day.getSessionsIds();

	       for(int timeSlot = 0; timeSlot < day.getNbTimeSlots(); timeSlot++)
	       {
		    IloExpr sumXSlot(env);
		    for(int i = 0; i < sessionsIds.size(); i++)
		    {
			 int sessionId = sessionsIds[i];
			 if(checkerData.sessions[sessionId].getTimeSlot() == timeSlot)
			 {
			      for(int j = 0; j < clusterIdCoeffPairs.size(); j++)
			      {
				   int clusterId = clusterIdCoeffPairs[j].first;
				   int clusterCoeff = clusterIdCoeffPairs[j].second;
				   if(checkerData.sessions[sessionId].getSize() >= clusters[clusterId].size())
				   {
					sumXSlot += clusterCoeff*x[clusterId][sessionId];
				   }
			      }

			 }
		    }

		    IloRange rng = (sumXSlot <= checkerData.getThemeLimitPerTimeSlot(day, timeSlot)); 
		    char name[100];
		    sprintf(name, "theme%dday%dslot%d", themeId, dayId, timeSlot);
		    rng.setName(name);
		    model.add(rng);
	       }
	  }
     }

     //for each theme, there is an upper bound on the number of papers in a given day
     for(int themeId = 0; themeId < checkerData.themes.size(); themeId++)
     {
	  const Theme& theme = checkerData.themes[themeId];
	  const std::vector<int>& papersIds = theme.getPapersIds();

	  std::vector<std::pair<int,int> > clusterIdCoeffPairs;
	  for(int clusterId = 0; clusterId < clusters.size(); clusterId++)
	  {
	       int clusterCoeff = 0;
	       for(int i = 0; i < clusters[clusterId].size(); i++)
	       {
		    if(std::find(papersIds.begin(), papersIds.end(), clusters[clusterId][i]) != papersIds.end())
		    {
			 clusterCoeff++;
		    }
	       }

	       if(clusterCoeff)
	       {
		    clusterIdCoeffPairs.push_back(std::make_pair(clusterId, clusterCoeff));
	       }
	  }

	  for(int dayId = 0; dayId < checkerData.days.size(); dayId++)
	  {
	       const Day& day = checkerData.days[dayId];
	       const std::vector<int>& sessionsIds = day.getSessionsIds();

	       IloExpr sumXDay(env);

	       for(int i = 0; i < sessionsIds.size(); i++)
	       {
		    int sessionId = sessionsIds[i];
		    for(int j = 0; j < clusterIdCoeffPairs.size(); j++)
		    {
			 int clusterId = clusterIdCoeffPairs[j].first;
			 int clusterCoeff = clusterIdCoeffPairs[j].second;
			 if(checkerData.sessions[sessionId].getSize() >= clusters[clusterId].size())
			 {
			      sumXDay += clusterCoeff*x[clusterId][sessionId];
			 }
		    }
	       }

	       IloRange rng = (sumXDay <= checkerData.getThemeLimitPerDay(theme)); 
	       char name[100];
	       sprintf(name, "theme%dday%d", themeId, dayId);
	       rng.setName(name);
	       model.add(rng);
	  }
     }


     IloCplex checker(model);
     checker.exportModel("modelcheck.lp");
     try 
     {   
	  checker.solve();
     }   
     catch(IloException& e)
     {   
	  std::cerr << "CPLEX exception in checker!" << std::endl << e << std::endl;
	  exit(EXIT_FAILURE);
     }   
 
     if(checker.getCplexStatus() != IloCplex::Optimal && checker.getCplexStatus() != IloCplex::OptimalTol && checker.getCplexStatus() != IloCplex::Infeasible)
     {
	  std::cerr << "unexpected CPLEX status in checker!" << std::endl << checker.getCplexStatus();
	  exit(EXIT_FAILURE);
     }

     bool feasible = checker.getCplexStatus() != IloCplex::Infeasible;

     env.end();

     return feasible;
}
