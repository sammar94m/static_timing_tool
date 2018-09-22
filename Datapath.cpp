/*
 * Datapath.c
 *
 *  Created on: 9 Aug 2018
 *      Author: Saeed
 */
#include "enums.h"
#include "Cell.h"
#include "Net.h"
#include "Flipflop.h"
#include "Clockdat.h"
extern queue<Net*> InputDataTable;
using namespace std;

string to_string(Transitions TR) {
	if (TR == FALL) {
		return string("F");
	} else if (TR == RISE) {
		return string("R");
	}
	return string("");
}

void pushCelltext(stringstream& S, string cell_name, string cell_template,
		output_pin out_pin, Transitions outTR, delay cell_delay,
		slope drv_slope, delay valid, string net_name) {
	S << valid << "	" << cell_delay << "	" << drv_slope << "	" << cell_name
			<< "	" << cell_template << "	" << out_pin << "	" << to_string(outTR)
			<< "	" << net_name << "	" << "\n";
}

margin dataPathDelayCalcAux(pin input, slope inslope, Cell* currCell,
		queue<Cell*>& Cellqueue, delay cumdelay, Transitions inPossiTr,
		MAXMIN MODE, string S, delay Reftime) {
	stringstream SS;
	SS << S;
	if ((currCell->type == FlIPFlOP && ((FlipFlop*) currCell)->visited == false)
			|| currCell->type == OUTCELL) {
		int skew = (((FlipFlop*) currCell)->getClkdat()->RISE_AR - Reftime)
				* (1 - 2 * MODE == MIN);
		margin tmp = cumdelay
				- ((FlipFlop*) currCell)->getSetup(MODE, inPossiTr);
		pushCelltext(SS, currCell->name, currCell->Template->template_name, "",
				FALL, 0, 0, (delay) tmp, "");
		if (MODE == MAX) {
			MAXpaths[pathidmax] = SS.str();
			SS << "pathid = " << pathidmax << "\n";
			pathidmax++;

		} else {
			MINpaths[pathidmin] = SS.str();
			SS << "pathid = " << pathidmin << "\n";
			pathidmin++;
		}

		return tmp;
	}
	for (auto pinIT = currCell->outMap.begin(); pinIT != currCell->outMap.end();
			++pinIT) {
		delay Celldelay;
		slope drvslope;
		Transitions OutTr;
		Net* currNet = pinIT->second;
		output_pin currCelloutPin = pinIT->first;
		load outload = currCell->getCout(currCelloutPin); // calculate load on the output pin
		for (int i = 1; i < 3; i++/*possible transitions*/) {
			if (!currCell->PossiblTr(input, currCelloutPin,
					(Transitions) (inPossiTr + i)))
				break;
			OutTr = i == 1 ? FALL : RISE;
			drvslope = currCell->getSlope(input, currCelloutPin, MODE,
					(Transitions) (inPossiTr + i), inslope, outload);
			Celldelay = currCell->getDelay(input, currCelloutPin, MODE,
					(Transitions) (inPossiTr + i), inslope, outload);

			for (auto rcvCellIt = currNet->receivers.begin();
					rcvCellIt != currNet->receivers.end(); ++rcvCellIt) {
				pushCelltext(SS, currCell->name,
						currCell->Template->template_name, pinIT->first, OutTr,
						Celldelay, drvslope, cumdelay, currNet->name);
				input_pin recieverpin = (*(rcvCellIt))->inPin;
				slope Netoutslope = currNet->getRcvSlope(drvslope,
						(*(rcvCellIt))->cell, recieverpin);
				delay netdelay = currNet->getRcvDelay((*(rcvCellIt))->cell,
						recieverpin); // calculate Net delay
				delay nextstagedelay = netdelay + cumdelay + Celldelay;
				margin rcvrmargin = dataPathDelayCalcAux(recieverpin,
						Netoutslope, (*(rcvCellIt))->cell,/*report,*/Cellqueue,
						nextstagedelay, OutTr, MODE, SS.str(), Reftime); // Move On
				currCell->updateWCdat(currCelloutPin, rcvrmargin, MODE, OutTr);
				currCell->updateWCdat(input, rcvrmargin, MODE, inPossiTr);
			}
		}
	}
	return currCell->getWCdat(input, MODE);
}

void dataPathDelayCalc() {
	Cell* curr;
	queue<Cell*> FFQueue;
	while (!InputDataTable.empty()) {
		Net* inNetp;
		inNetp =InputDataTable.front();
		InputDataTable.pop();
		for(auto RcvIt=inNetp->receivers.begin();RcvIt!=inNetp->receivers.end();++RcvIt){
		curr=(*RcvIt)->cell;
		dataPathDelayCalcAux("", 0, curr, FFQueue, 0, RISE, MAX, "", 0);
		dataPathDelayCalcAux("", 0, curr, FFQueue, 0, RISE, MIN, "", 0);
		}
	}
	while (!FFQueue.empty()) {
		curr = FFQueue.front();
		InputDataTable.pop();
		clockdat* clkp = ((FlipFlop*) curr)->getClkdat();
		dataPathDelayCalcAux("CLK", clkp->RISE_SLOPE, curr, FFQueue, 0, RISE, MAX,
				"", clkp->RISE_AR);
		dataPathDelayCalcAux("CLK", clkp->RISE_SLOPE, curr, FFQueue, 0, RISE, MIN,
				"", clkp->RISE_AR);
		((FlipFlop*) curr)->visited = true;
	}
}

