#ifndef PROBPARAMETERS_H
#define PROBPARAMETERS_H

#include "bcDoubleC.hpp"
#include "bcParameterParserC.hpp"
#include "bcModelParameterC.hpp"

/* PROBLEM SPECIFIC PARAMETERS */
class ApplicationSpecificParam : public ParameterParser
{
   private:
      
      ApplicationParameter<int> _cutOffValue;
      
   public:
     
      ApplicationSpecificParam(int argc, char* argv[],
	    std::string parameterFileName, std::ostream& os = std::cout);
      virtual ~ApplicationSpecificParam()
      {}

      std::ostream& print(std::ostream& os = std::cout) const;

      int cutOffValue() const;
};

inline std::ostream& operator<<(std::ostream& os,
      const ApplicationSpecificParam & that)
{
   return that.print(os);
}

#endif 
