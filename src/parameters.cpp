#include "parameters.h"

ApplicationSpecificParam::ApplicationSpecificParam(int argc, char* argv[],
    std::string parameterFileName, std::ostream& os) :
    ParameterParser(parameterFileName),
      _cutOffValue(std::string("cutOffValue"), -1, "cut off value") 
{
     addApplicationParameter(_cutOffValue); 

     parse(argc, argv);
}

std::ostream& ApplicationSpecificParam::print(std::ostream& os) const
{
     os << std::endl;
     os << "##### RCP Parameters #####" << std::endl;
     os << _cutOffValue << std::endl;
     os << "#############################################" << std::endl << std::endl;

     return (os);
}

int ApplicationSpecificParam::cutOffValue() const
{
     return _cutOffValue();
}
