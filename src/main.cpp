#include "bcUsefulHeadFil.hpp"
#include "bcModelingLanguageC.hpp"
#include "Data.h"
#include "model.h"
#include "parameters.h"
#include <cstdlib>
#include <stdlib.h>
#include "bcApplicationException.hpp"

using namespace std;

int main(int argc, char *argv[])
{
     BcInitialisation bapcodInitPtr(argc, argv, "", false); /// do not output BaPcod parameters
     ApplicationSpecificParam applSpecParam(argc, argv, bapcodInitPtr.configFile());
     applSpecParam.print();

     string instPath = string(bapcodInitPtr.instanceFile());
     Data data(instPath.c_str());

     try
     {
	  BcModel model(bapcodInitPtr, "rcp", BcObjStatus::minInt);
	  buildModel(data, applSpecParam, model);
	  BcSolution solPtr = model.solve();

	  if(solPtr.defined())
	  {
	       std::cout << solPtr << endl;
	  }
	  bapcodInitPtr.outputBaPCodStatistics(data.getInstName());

     } catch(const ApplicationException& e)
     {
	  std::cerr << e << std::endl;
     }


     return EXIT_SUCCESS;
}
