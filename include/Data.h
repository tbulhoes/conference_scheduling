#ifndef _DATA_H_
#define _DATA_H_

#include <vector>
#include <stdio.h>
#include <string>
#include <cmath>

class Author
{
     private:

	  int id, nbPapers;
	  std::vector<int> papersIds;

     public:
	  Author(int id): id(id), nbPapers(0) {}

	  void addPaper(int paperId) 
	  {
	       nbPapers++;
	       papersIds.push_back(paperId);
	  }

	  const std::vector<int>& getPapersIds() const
	  {
	       return papersIds;
	  }

};

struct Paper
{
     private:

	  int id, nbAuthors, nbThemes;
	  std::vector<int> authorsIds, themesIds;

     public:
	  Paper(int id): id(id), nbAuthors(0), nbThemes(0) {}

	  void addAuthor(int authorId)
	  {
	       nbAuthors++;
	       authorsIds.push_back(authorId);
	  }

	  void addTheme(int themeId)
	  {
	       nbThemes++;
	       themesIds.push_back(themeId);
	  }
};

struct Theme
{
     private:

	  int id, nbPapers;
	  std::vector<int> papersIds;

     public:

	  Theme(int id): id(id), nbPapers(0) {}

	  void addPaper(int paperId)
	  {
	       nbPapers++;
	       papersIds.push_back(paperId);
	  }

	  const std::vector<int>& getPapersIds() const
	  {
	       return papersIds;
	  }
};

struct Day
{
     private:

	  int id, nbTimeSlots, nbSessions;
	  std::vector<int> sessionsIds;

     public:
	  Day(int id, int nbTimeSlots): id(id), nbTimeSlots(nbTimeSlots), nbSessions(0) {}

	  void addSession(int sessionId)
	  {
	       nbSessions++;
	       sessionsIds.push_back(sessionId);
	  }

	  const std::vector<int>& getSessionsIds() const
	  {
	       return sessionsIds;
	  }

	  int getNbTimeSlots() const
	  {
	       return nbTimeSlots;
	  }
};

struct Session
{
     private:

	  int id, timeSlot, size, dayId;

     public:
	  Session(int id, int timeSlot, int size, int dayId): id(id), timeSlot(timeSlot), size(size), dayId(dayId) {}

	  int getId() const
	  {
	       return id;
	  }

	  int getTimeSlot() const
	  {
	       return timeSlot;
	  }

	  int getDayId() const
	  {
	       return dayId;
	  }

	  int getSize() const
	  {
	       return size;
	  }
};

struct CheckerData
{
     std::vector<Paper> papers;
     std::vector<Day> days;
     std::vector<Session> sessions;
     std::vector<Theme> themes;
     std::vector<Author> authors;

     int getThemeLimitPerTimeSlot(const Day& day, const int timeSlot) const
     {
	  int maxSize = 0;
	  const std::vector<int>& sessionsIds = day.getSessionsIds();
	  for(int i = 0; i < sessionsIds.size(); i++)
	  {
	       if(sessions[sessionsIds[i]].getTimeSlot() == timeSlot)
		    maxSize = std::max(maxSize, sessions[sessionsIds[i]].getSize());
	  }

	  return 2*maxSize;
     }
     
     int getThemeLimitPerDay(const Theme& theme) const
     {
	  int numPapersInTheme = theme.getPapersIds().size();
	  if (days.size() == 1)
	       return numPapersInTheme;

	  return (numPapersInTheme > 20) 
	       ? (int)ceil(numPapersInTheme/2.0)
	       : numPapersInTheme;
     }
};

class Data
{
     private:

	  int nbClusterTypes, n;
	  std::vector<int> minClusterSize, maxClusterSize, maxNbClusters;
	  std::vector< std::vector<int> > weights;
	  std::string instName;

	  CheckerData checkerData;

     public:

	  Data(const char* filePath);

	  int getN() const
	  {
	       return n;
	  }

	  int getWeight(const int i, const int j) const
	  {
	       return weights[i][j];
	  }

	  int getMinClusterSize(const int clusterType) const
	  {
	       return minClusterSize[clusterType];
	  }

	  int getMaxClusterSize(const int clusterType) const
	  {
	       return maxClusterSize[clusterType];
	  }

	  int getMaxNbClusters(const int clusterType) const
	  {
	       return maxNbClusters[clusterType];
	  }

	  int getNbClusterTypes() const
	  {
	       return nbClusterTypes;
	  }

	  std::string getInstName() const
	  {
	       return instName;
	  }

	  const CheckerData& getCheckerData() const
	  {
	       return checkerData;
	  }
};

#endif
