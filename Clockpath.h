/*
 * Clockpath.h
 *
 *  Created on: Nov 16, 2018
 *      Author: compm
 */

#ifndef CLOCKPATH_H_
#define CLOCKPATH_H_


#include "enums.h"
#include "Cell.h"
#include "Net.h"
#include "InputNet.h"
#include "Flipflop.h"
#include "Clockdat.h"

extern queue<Net*> InputClkTable;
void clockPathDelayCalcAux(pin input, clockdat& inclock, Cell* currCell,
		MAXMIN MODE);
void clockPathDelayCalc();
#endif /* CLOCKPATH_H_ */
