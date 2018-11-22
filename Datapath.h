/*
 * Datapath.h
 *
 *  Created on: Nov 22, 2018
 *      Author: compm
 */

#ifndef DATAPATH_H_
#define DATAPATH_H_



#include "enums.h"
#include "Cell.h"
#include "Net.h"
#include "Flipflop.h"
#include "Clockdat.h"
extern queue<Net*> InputDataTable;
using namespace std;

void dataPathDelayCalc();

#endif /* DATAPATH_H_ */
