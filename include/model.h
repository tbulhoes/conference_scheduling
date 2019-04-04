#ifndef CVXP_MODEL_H
#define CVXP_MODEL_H

#include "bcModelPointerC.hpp"
#include "Data.h"
#include "parameters.h"

//column generation based on edge variables x_ij (job i and j are adjacent)
void buildModel(Data& data, const ApplicationSpecificParam & params, BcModel & model);

#endif 
