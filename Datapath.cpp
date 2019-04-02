/*
 * Datapath.c
 *
 *  Created on: 9 Aug 2018
 *      Author: Saeed
 */
#include "Datapath.h"
time_t resettime;

string to_string(Tr TR) {
	if (TR == FALL) {
		return string("F");
	} else if (TR == RISE) {
		return string("R");
	}
	return string("");
}

void dataPathDelayCalc() {

}
/*Given a cell backward walk to primary inputs and return them in a Q
 * mark the visited cells with time stamps
 * set ready inputs to zero
 */
void GraphPartition(Cell& OutDriver, vector<Net*>& PrimeNet) {
	resettime = time(NULL); //reset visited status
	std::cout << "Partioning Graph starting from Cell: " << OutDriver.name
			<< endl;
	queue<Cell*> CellQ; //used to store the discovered Cells
	CellQ.push(&OutDriver);
	while (!CellQ.empty()) {
		Cell* curr = CellQ.front();
		CellQ.pop();
		for (auto& pair : curr->inMap) {
			auto in_Pin=pair.first;
			auto inNet=pair.second;
			//TODO: Flipflop Data invalidation
			if (inNet->type == INPUT) {
				PrimeNet.push_back(inNet);
				std::cout << "Reached Input: " << inNet->name << endl;
			} else {
				CellQ.push(inNet->driver.first);
			}
		}
		curr->visittime = time(NULL);
		curr->ready_inputs = 0;
	}
}

void ForwardPropagateValid(const vector<Net*>& inputNetVec) {
	/*
	 * copy Nets to NetQ
	 */
	//TODO: check if FIFO
	queue<Net*> NetQ;
	queue<Cell*> CellQ; //ready to process
	for (const auto& pNet : inputNetVec) {
		NetQ.push(pNet);
	}
	while (!NetQ.empty() || !CellQ.empty()) {
		while (!NetQ.empty()) {
			Net* currNet = NetQ.front(); //pop NetQ
			NetQ.pop();
			PinDat Data = currNet->getDrvData(); //fetch Driver Data
			for (auto& rcv : currNet->receivers) { //iterate over receivers
				if (rcv->cell->visittime < resettime) {
					continue; // Cell isn't connected to current processing output
				} else {
					Cell* currCell = rcv->cell;
					auto inPin = rcv->inPin;
					currNet->calcRcvData(rcv, Data, inPin);
					if (currCell->isReady()) {
						CellQ.push(currCell);
					}
				}
			}
		}
		while (!CellQ.empty()) { //NetQ is empty() CellQ isn't empty
			Cell* currCell = CellQ.front();
			CellQ.pop();
			currCell->CalcOutputData();/*ready_input reset to 0*/
			if (currCell->outNet->type == OUTPUT) {
				std::cout << "reached output: " << currCell->outNet->name
						<< endl;
			} else {
				NetQ.push(currCell->outNet);
			}
		}
	}

}

