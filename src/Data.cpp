#include "Data.h"
#include <stdlib.h>
#include <iostream>

Data::Data(const char* filePath)
{
   FILE* f = fopen(filePath, "r");

   std::string auxStr = std::string(filePath);
   instName = auxStr.substr(auxStr.rfind("/")+1);

   if(!f)
   {
	std::cerr << "Problem while reading instance" << std::endl;
	exit(EXIT_FAILURE);
   }

   if(fscanf(f, "%d", &n) != 1)
   {
	std::cerr << "Problem while reading instance" << std::endl;
	exit(EXIT_FAILURE);
   }

   //reading weights
   weights = std::vector<std::vector<int> >(n, std::vector<int>(n));
   for(int i = 0; i < n; i++)
   {
	for(int j = 0; j < n; j++)
	{
	     if(fscanf(f, "%d", &weights[i][j]) != 1)
	     {
		  std::cerr << "Problem while reading instance" << std::endl;
		  exit(EXIT_FAILURE);
	     }
	     weights[i][j] *= -1;
	}
   }

   //reading cluster types
   if(fscanf(f, "%d", &nbClusterTypes) != 1)
   {
	std::cerr << "Problem while reading instance" << std::endl;
	exit(EXIT_FAILURE);
   }

   minClusterSize.resize(nbClusterTypes);
   maxClusterSize.resize(nbClusterTypes);
   maxNbClusters.resize(nbClusterTypes);  
   for(int clusterType = 0; clusterType < nbClusterTypes; clusterType++)
   {
	if(fscanf(f, "%d %d %d", &maxClusterSize[clusterType], &maxNbClusters[clusterType], &minClusterSize[clusterType]) != 3)
	{
	     std::cerr << "Problem while reading instance" << std::endl;
	     exit(EXIT_FAILURE);
	}
   }

   //now reading checker's data
   int nbDays;
   if(fscanf(f, "%d", &nbDays) != 1)
   {
	std::cerr << "Problem while reading instance" << std::endl;
	exit(EXIT_FAILURE);
   }

   int totalNbSessions = 0;
   for(int dayId = 0; dayId < nbDays; dayId++)
   {
	int nbTimeSlots, nbSessions;
	if(fscanf(f, "%d %d", &nbTimeSlots, &nbSessions) != 2)
	{
	     std::cerr << "Problem while reading instance" << std::endl;
	     exit(EXIT_FAILURE);
	}
	checkerData.days.push_back(Day(dayId, nbTimeSlots));

	//now reading sessions of this day
        for(int i = 0; i < nbSessions; i++)
	{
	     int timeSlot, size;
	     if(fscanf(f, "%d %d", &timeSlot, &size) != 2)
	     {
		  std::cerr << "Problem while reading instance" << std::endl;
		  exit(EXIT_FAILURE);
	     }
	     checkerData.sessions.push_back(Session(totalNbSessions, timeSlot - 1, size, dayId));
	     checkerData.days[dayId].addSession(totalNbSessions++);
	}
   }

   //reading themes
   int nbPapers, totalNbThemes;
   if(fscanf(f, "%d %d", &nbPapers, &totalNbThemes) != 2)
   {
	std::cerr << "Problem while reading instance" << std::endl;
	exit(EXIT_FAILURE);
   }
   for(int themeId = 0; themeId < totalNbThemes; themeId++)
   {
	checkerData.themes.push_back(Theme(themeId));
   }
   

   for(int paperId = 0; paperId < nbPapers; paperId++)
   {
	checkerData.papers.push_back(Paper(paperId));
	
	int nbThemes;
	if(fscanf(f, "%d", &nbThemes) != 1)
	{
	     std::cerr << "Problem while reading instance" << std::endl;
	     exit(EXIT_FAILURE);
	}
	for(int i = 0; i < nbThemes; i++)
	{
	     int themeId;
	     if(fscanf(f, "%d", &themeId) != 1)
	     {
		  std::cerr << "Problem while reading instance" << std::endl;
		  exit(EXIT_FAILURE);
	     }
	     checkerData.papers[paperId].addTheme(themeId);
	     checkerData.themes[themeId].addPaper(paperId);
	}
   }

   //reading authors
   int totalNbAuthors;
   if(fscanf(f, "%d %d", &nbPapers, &totalNbAuthors) != 2)
   {
	std::cerr << "Problem while reading instance" << std::endl;
	exit(EXIT_FAILURE);
   }
   for(int authorId = 0; authorId < totalNbAuthors; authorId++)
   {
	checkerData.authors.push_back(Author(authorId));
   }
   for(int paperId = 0; paperId < nbPapers; paperId++)
   {
	int nbAuthors;
	if(fscanf(f, "%d", &nbAuthors) != 1)
	{
	     std::cerr << "Problem while reading instance" << std::endl;
	     exit(EXIT_FAILURE);
	}
	for(int i = 0; i < nbAuthors; i++)
	{
	     int authorId;
	     if(fscanf(f, "%d", &authorId) != 1)
	     {
		  std::cerr << "Problem while reading instance" << std::endl;
		  exit(EXIT_FAILURE);
	     }
	     checkerData.authors[authorId].addPaper(paperId);
	     checkerData.papers[paperId].addAuthor(authorId);
	}
   }

   fclose(f);
}
