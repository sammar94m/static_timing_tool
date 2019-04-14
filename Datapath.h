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
void BuildCritandBS(const vector<Net*>& inputNetVec, vector<_PATH*>& pPATHvec,
		MAXMIN M, PriorityQ<branchslack>& BS);
void BSAux(Net* inNet, MAXMIN M, _PATH* PA, margin PATHMARG,
		margin& maxdiscovered, PriorityQ<branchslack>& BS,
		unsigned int& numbranch, bool enforcestate = false, Tr state = FALL);

void BuildPartialPATHS(vector<_PATH*>& pPATHvec, PriorityQ<branchslack>& BSQ,
	PriorityQ<_PATH*>& PAQ, MAXMIN M);
#endif /* DATAPATH_H_ */
