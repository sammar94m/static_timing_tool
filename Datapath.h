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
#include "Cell.h"
#include "InputNet.h"
#include "OutputNet.h"
#include "Flipflop.h"
#include "path.h"
using namespace std;

void dataPathDelayCalc();
void BackwardPropagateReq(Net& OutNet);
void ForwardPropagateValid(const vector<Net*>& inputNetVec);
void GraphPartition(Net& OutNet, vector<Net*>& PrimeNet, vector<FlipFlop*>& FFvec);
void BuildCritandBS(const vector<Net*>& inputNetVec, vector<_PATH*>& pPATHvec,
		MAXMIN M, PriorityQ<branchslack, BRANCHCompare>& BS);
void BSAux(Net* inNet, MAXMIN M, _PATH* PA, margin PATHMARG,
		margin& maxdiscovered, PriorityQ<branchslack, BRANCHCompare>& BS,
		unsigned int& numbranch, bool enforcestate = false, Tr state = FALL);

void BuildPartialPATHS(vector<_PATH*>& pPATHvec,
		PriorityQ<branchslack, BRANCHCompare>& BSQ,
		PriorityQ<_PATH*, PATHCompare>& PAQ, MAXMIN M);
#endif /* DATAPATH_H_ */
