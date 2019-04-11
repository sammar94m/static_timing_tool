/*
 * Datapath.h
 *
 *  Created on: Nov 22, 2018
 *      Author: compm
 */

#ifndef DATAPATH_H_
#define DATAPATH_H_

#include "enums.h"
#include "cellfwd.h"
#include "Flipflop.h"
#include "Clockdat.h"
#include "Cell.h"
#include "InputNet.h"
#include "OutputNet.h"
#include "path.h"
using namespace std;

void dataPathDelayCalc();
void BackwardPropagateReq(Net& OutNet);
void ForwardPropagateValid(const vector<Net*>& inputNetVec);
void GraphPartition(Net& OutNet, vector<Net*>& PrimeNet);

#endif /* DATAPATH_H_ */
