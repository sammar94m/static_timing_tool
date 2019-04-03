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
	} else {
		return string("R");
	}
}

void dataPathDelayCalc() {

}
/*Given a cell backward walk to primary inputs and return them in a Q
 * mark the visited cells with time stamps
 * set ready inputs to zero
 */
void GraphPartition(Net& OutNet, vector<Net*>& PrimeNet) {
	resettime = time(NULL); //reset visited status
	std::cout << "Partioning Graph starting from Output: "
			<< OutNet.name << endl;
	queue<Cell*> CellQ; //used to store the discovered Cells
	CellQ.push(OutNet.driver.first);
	while (!CellQ.empty()) {
		Cell* curr = CellQ.front();
		CellQ.pop();
		for (auto& pair : curr->inMap) {
			auto in_Pin = pair.first;
			auto inNet = pair.second;
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
	cout << "Starting valid propagation" << endl;
	queue<Net*> NetQ;
	queue<Cell*> CellQ; //ready to process
	for (const auto& pNet : inputNetVec) {
		NetQ.push(pNet);
	}
	while (!NetQ.empty() || !CellQ.empty()) {
		while (!NetQ.empty()) {
			Net* currNet = NetQ.front(); //pop NetQ
			cout << "propagating through net: " << currNet->name << endl;
			NetQ.pop();
			PinDat Data = currNet->getDrvData(); //fetch Driver Data
			for (auto& rcv : currNet->receivers) { //iterate over receivers
				Cell* currCell = rcv->cell;
				auto inPin = rcv->inPin;
				currNet->calcRcvData(rcv, Data, inPin);
				if (rcv->cell->visittime < resettime) {
					continue; // Cell isn't connected to current processing output
				} else {
					if (currCell->isReady()) {
						cout << "Cell: " << currCell->name << " is ready"
								<< endl;
						CellQ.push(currCell);
					}
				}
			}
		}
		while (!CellQ.empty()) { //NetQ is empty() CellQ isn't empty
			Cell* currCell = CellQ.front();
			CellQ.pop();
			currCell->CalcOutputData();/*ready_input reset to 0 visited=true*/
			if (currCell->outNet->type == OUTPUT) {
				std::cout << "reached output: " << currCell->outNet->name
						<< endl;
			} else {
				NetQ.push(currCell->outNet);
			}
		}
	}

}
void BackwardPropagateReq(Net& OutNet) {
	cout << "Starting required propagation from: "<<OutNet.name << endl;
	queue<Cell*> CellQ; //used to store the discovered Cells
	CellQ.push(OutNet.driver.first);
	while (!CellQ.empty()) {
		Cell* curr = CellQ.front();
		CellQ.pop();
		curr->CalcInputReq();
		for (auto& pair : curr->inMap) {
			auto in_Pin = pair.first;
			auto inNet = pair.second;
			inNet->CalcDrvReq();
			//TODO: Flipflop Data invalidation
			if (inNet->type == INPUT) {
				std::cout << "Reached Input: " << inNet->name << endl;
			} else {
				CellQ.push(inNet->driver.first);
			}
		}

	}
}
