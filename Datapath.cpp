/*
 * Datapath.c
 *
 *  Created on: 9 Aug 2018
 *      Author: Saeed
 */
#include "Datapath.h"

time_t resettime;
vector<MAX_PATH> MAXP;
vector<MIN_PATH> MINP;
int MAX_PATH::counter = 0;
int MIN_PATH::counter = 0;
string to_string(Tr TR) {
	if (TR == FALL) {
		return string("F");
	} else {
		return string("R");
	}
}

void dataPathDelayCalc() {
	vector<Net*> INNETS;
	vector<_PATH*> MAX_P;
	PriorityQ PQ(4);
	for (Net* O : OutputTable) {
		if (O->driver.first == NULL) {
			cout << O->name << " Net has no driver" << endl;
			return;
		}
		GraphPartition(*O, INNETS);
		ForwardPropagateValid(INNETS);
		BackwardPropagateReq(*O);
		BuildCritandBS(INNETS, MAX_P, MAX, PQ);
	}
}
/*Given a cell backward walk to primary inputs and return them in a Q
 * mark the visited cells with time stamps
 * set ready inputs to zero
 */
void GraphPartition(Net& OutNet, vector<Net*>& PrimeNet) {
	resettime = time(NULL); //reset visited status
	std::cout << "Partioning Graph starting from Output: " << OutNet.name
			<< endl;
	queue<Cell*> CellQ; //used to store the discovered Cells
	CellQ.push(OutNet.driver.first);
	while (!CellQ.empty()) {
		Cell* curr = CellQ.front();
		if (curr == NULL) {
			cout << "NULL pointer to cell" << endl;
			exit(0);
		}
		CellQ.pop();
		std::cout << "Reached Cell: " << curr->name << endl;
		curr->resetReq();
		for (auto& pair : curr->inMap) {
			auto in_Pin = pair.first;
			auto inNet = pair.second;
			//TODO: Flipflop Data invalidation
			if (inNet->type == INPUT) {
				if (!((inputNet*) inNet)->visited) {
					PrimeNet.push_back((inputNet*) inNet);
					((inputNet*) inNet)->visited = true;
					std::cout << "Reached Input: " << inNet->name << endl;
				}
			} else {
				CellQ.push(inNet->driver.first);
			}

		}
		curr->visittime = resettime+1;
		curr->ready_inputs = 0;

	}
}

void ForwardPropagateValid(const vector<Net*>& inputNetVec) {
	/*
	 * copy Nets to NetQ
	 */
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
								<< " #Ready = " << currCell->ready_inputs
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
void PushNet(Net& net, _PATH* PATH, MAXMIN M) {
	N_NODE* pNetNode = new N_NODE(new Net(net));
	PATH->vec.push_back((_NODE*) pNetNode);
}
void PushCellandInOutPin(Cell& cell, pin inp, pin outp, _PATH* PATH, MAXMIN M) {
	P_NODE* pPinNode = new P_NODE(inp, cell.PinData[inp]);
	PATH->vec.push_back((_NODE*) pPinNode);
	C_NODE* pCellNode = new C_NODE(new Cell(cell));
	PATH->vec.push_back((_NODE*) pCellNode);
	pPinNode = new P_NODE(outp, cell.PinData[outp]);
	PATH->vec.push_back((_NODE*) pPinNode);
}
void BackwardPropagateReq(Net& OutNet) {
	cout << "Starting required propagation from: " << OutNet.name << endl;
	queue<Cell*> CellQ; //used to store the discovered Cells
	CellQ.push(OutNet.driver.first);
	OutNet.CalcDrvReq(dynamic_cast<outputNet&>(OutNet).Req.tmp_req, NULL, ""); //TODO: GET RID OF DYNAMIC CAST

	while (!CellQ.empty()) {
		Cell* curr = CellQ.front();
		CellQ.pop();
		curr->CalcInputReq();
		cout << "Calculated required for Cell's " << curr->name << " inputs"
				<< endl;
		for (auto& pair : curr->inMap) {
			auto in_Pin = pair.first;
			auto inNet = pair.second;
			inNet->CalcDrvReq(curr->PinData[in_Pin].tmp_req, curr, in_Pin);
			//TODO: Flipflop Data invalidation
			if (inNet->type == INPUT) {
				std::cout << "Reached Input: " << inNet->name << endl;
			} else {
				CellQ.push(inNet->driver.first);
			}
		}

	}
}
MAXMIN GLOBM = MAX;

bool CompInNets(Net* lhs, Net* rhs) {
	/*implement <
	 *
	 */

	return ((inputNet*) lhs)->Ariv.GetWCTmpMarg(GLOBM)
			< ((inputNet*) rhs)->Ariv.GetWCTmpMarg(GLOBM);
}
void BuildCritandBS(const vector<Net*>& inputNetVec, vector<_PATH*>& pPATHvec,
		MAXMIN M, PriorityQ& BS) {
	GLOBM = M;
	margin maxdiscovered = INT_MIN;
	unsigned int numPath = 0;
	unsigned int numbranch = 0;
	vector<Net*> pNetVec = inputNetVec;
	sort(pNetVec.begin(), pNetVec.end(), CompInNets);
//	for (auto& e : pNetVec) {
//		cout << e->name << " MARG : " << ((inputNet*) e)->Ariv.GetWCTmpMarg(M)
//				<< endl;
//	}
	cout << "Starting Path building " << endl;
	for (auto inNet : pNetVec) {
		queue<Net*> pNetQ;
		if ((((inputNet*) inNet)->Ariv.GetWCTmpMarg(M) > maxdiscovered)
				&& (numofpaths <= numPath + numbranch)) {
			cout << "Skipped input: " << inNet->name << " Margin: "
					<< ((inputNet*) inNet)->Ariv.GetWCTmpMarg(M) << endl;
			return;
		}
		//init critical path
		cout << "		Building Critical Path: " << inNet->name << endl;
		margin PATHMARG = ((inputNet*) inNet)->Ariv.GetWCTmpMarg(M);
		maxdiscovered = max(maxdiscovered, PATHMARG);

		_PATH* PA;
		if (M == MAX) {
			PA = new MAX_PATH(PATHMARG);
		} else {
			PA = new MIN_PATH(PATHMARG);
		}
		numPath++;
		pPATHvec.push_back(PA);
		Net* pN=inNet;
		while (pN!=NULL) {
			PushNet(*pN, PA, M);
			cout << "Pushed Net " <<pN->name<<" to path"<<endl;
			if (pN->type != OUTPUT) {
				auto Rcvit = pN->getCritReciever(M);
				cout << "Critical receiver of net: " << pN->name << " is "<< (*Rcvit)->cell->name << endl;
				Tr tr = (*Rcvit)->cell->PinData[(*Rcvit)->inPin].GETWCTrTmp(M);
				auto vecIT = PA->vec.end();
				vecIT--;
				pN->RecordBS(vecIT, Rcvit, PATHMARG, BS, M, tr);
//				cout<<"CALCED BS OF NET "<<pN->name<<endl;
				PushCellandInOutPin(*((*Rcvit)->cell), (*Rcvit)->inPin,
						(*Rcvit)->cell->outPin, PA, M);
				cout << "Pushed Cell " <<(*Rcvit)->cell->name<<" to path"<<endl;
				numbranch = BS.getSize();
				maxdiscovered = max(maxdiscovered, BS.GetMAX());
				pN=(*Rcvit)->cell->outNet;
			}else{
				cout <<"		MOVING TO NEXT INPUT"<<endl;
				pN=NULL;
			}
		}
	}
}
