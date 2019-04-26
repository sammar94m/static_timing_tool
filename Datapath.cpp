/*
 * Datapath.c
 *
 *  Created on: 9 Aug 2018
 *      Author: Saeed
 */
#include "Datapath.h"

time_t resettime = 0;
vector<MAX_PATH> MAXP;
vector<MIN_PATH> MINP;
int MAX_PATH::counter = 0;
int MIN_PATH::counter = 0;
MAXMIN GLOBM = MAX;
void dataPathDelayCalcAux(Net* O, vector<Net*>& INNETS,
		vector<FlipFlop*>& FFvec, vector<_PATH*> (&P)[2],
		PriorityQ<branchslack, BRANCHCompare> (&PQ_BS)[2],
		PriorityQ<_PATH*, PATHCompare> (&PQ_P)[2]) {
	GraphPartition(*O, INNETS, FFvec);
	cout << endl << endl;
	ForwardPropagateValid(INNETS);
	cout << endl << endl;
	BackwardPropagateReq(*O);
	cout << endl << endl;
	for (auto i : { MIN, MAX }) {
		for (auto j : { FALL, RISE }) {
			BuildCritandBS(INNETS, P[i], i, PQ_BS[i], j);
			cout << endl << endl;
			BuildPartialPATHS(P[i], PQ_BS[i], PQ_P[i], i);
			cout << endl << endl;
		}
	}
	INNETS.clear();
}
void dataPathDelayCalc() {
	//TODO: MARK END FLOP + NET
	vector<Net*> INNETS;
	vector<_PATH*> P[2];
	PriorityQ<branchslack, BRANCHCompare> PQ_BS[2];
	PriorityQ<_PATH*, PATHCompare> PQ_P[2];
	vector<FlipFlop*> FFvec;
	for (Net* O : OutputTable) {
		if (O->driver.first == NULL) {
			cout << O->name << " Net has no driver" << endl;
			return;
		}
		dataPathDelayCalcAux(O, INNETS, FFvec, P, PQ_BS, PQ_P);
		for (auto& FF : FFvec) {
			FF->endpoint = true;
			FF->initDataReq();
			Net* pN = FF->inMap[FF->getDatPin()];
			dataPathDelayCalcAux(pN, INNETS, FFvec, P, PQ_BS, PQ_P);
			FF->endpoint = false;
		}
		FFvec.clear();
	}
	cout << "		Outputting results" << endl;
	OutputResults(PQ_P);
	cout << "		Done" << endl;
}
void OutputResults(PriorityQ<_PATH*, PATHCompare> (&PQ_P)[2]) {
	ofstream maxpathfile("max_paths.txt", ofstream::out | ofstream::trunc);
	ofstream minpathfile("min_paths.txt", ofstream::out | ofstream::trunc);
	ofstream maxpinfile("max_pins.txt", ofstream::out | ofstream::trunc);
	ofstream minpinfile("min_pins.txt", ofstream::out | ofstream::trunc);
	//MAX
	maxpathfile << "		MAX PATHS" << endl;
	maxpathfile << "Main CLK high " << ((inputNet*) mainClk)->high << " low "
			<< ((inputNet*) mainClk)->low << endl;
	maxpathfile << endl;
	PQ_P[MAX].Print(MAX, maxpathfile);
	maxpathfile << endl << endl;
	//MIN
	minpathfile << "Main CLK high " << ((inputNet*) mainClk)->high << " low "
			<< ((inputNet*) mainClk)->low << endl;
	minpathfile << endl;
	minpathfile << "		MIN PATHS" << endl;
	PQ_P[MIN].Print(MIN, minpathfile);
	minpathfile << endl << endl;
	// PINS
	maxpinfile<<"NAME	"
			<<"WC_MARG_FALL	"
			<<"WC_VLD_FALL	"
			<<"WC_REQ_FALL	"
			<<"WC_MARG_RISE	"
			<<"WC_VLD_RISE	"
			<<"WC_REQ_RISE	"<<endl;

	minpinfile<<"NAME	"
			<<"WC_MARG_FALL	"
			<<"WC_VLD_FALL	"
			<<"WC_REQ_FALL	"
			<<"WC_MARG_RISE	"
			<<"WC_VLD_RISE	"
			<<"WC_REQ_RISE	"<<endl;
	for(auto& c : CellsTable){
		c.second->printpins(MAX,maxpinfile);
		c.second->printpins(MIN,minpinfile);
	}
	maxpathfile.close();
	minpathfile.close();
	maxpinfile.close();
	minpinfile.close();
}
/*Given a cell backward walk to primary inputs and return them in a Q
 * mark the visited cells with time stamps
 * set ready inputs to zero
 */
void GraphPartition(Net& OutNet, vector<Net*>& PrimeNet,
		vector<FlipFlop*>& FFvec) {
	resettime += 2; //reset visited status
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
			if (curr->type == cellType::FlIPFlOP && inNet->isClk == false) {
				if (curr->visittime < resettime) {
					FFvec.push_back((FlipFlop*) curr);
				}
				continue; //TODO: ADD DATA NETS TO A Q
			}
			if (inNet->type == INPUT) {
				if (inNet->visittime < resettime) {
					PrimeNet.push_back((inputNet*) inNet);
					std::cout << "Reached Input: " << inNet->name << endl;
					inNet->visittime = resettime + 1;
				}
			} else {
				CellQ.push(inNet->driver.first);
			}

		}
		curr->visittime = resettime + 1;
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
			NetQ.pop();
			if (currNet->visited)
				continue;
			cout << "propagating through net: " << currNet->name << endl;
			PinDat Data = currNet->getDrvData(); //fetch Driver Data
			for (auto& rcv : currNet->receivers) { //iterate over receivers
				Cell* currCell = rcv->cell;
				auto inPin = rcv->inPin;
				currNet->calcRcvData(rcv, Data, inPin);
//				if (currCell->visittime < resettime) {
//					continue; // Cell isn't connected to current processing output
//				} else
				if (rcv->cell->type == FlIPFlOP) {
					if (currNet->isClk == false
							&& ((FlipFlop*) currCell)->endpoint == false) {
						continue;
					}
				}
				if (currCell->isReady()) {
					cout << "Cell: " << currCell->name << " is ready"
							<< " #Ready = " << currCell->ready_inputs << endl;
					CellQ.push(currCell);
				}
			}
			currNet->visited = true;
		}

		while (!CellQ.empty()) { //NetQ is empty() CellQ isn't empty
			Cell* currCell = CellQ.front();
			CellQ.pop();
			currCell->CalcOutputData();/*ready_input reset to 0 visited=true*/
			if (currCell->type == FlIPFlOP) {
				if (((FlipFlop*) currCell)->endpoint == true) {
					std::cout << "reached FlipFlop: " << currCell->name << endl;
				} else {
					NetQ.push(currCell->outNet);
				}
			} else if (currCell->outNet->type == OUTPUT) {
				std::cout << "reached output: " << currCell->outNet->name
						<< endl;
			} else {
				NetQ.push(currCell->outNet);
			}
		}
	}

}
void PushNet(Net& net, _PATH* PATH, MAXMIN M, delay del = 0, slope slp = 0) {
	N_NODE* pNetNode = new N_NODE(new Net(net), del, slp);
	PATH->vec.push_back((_NODE*) pNetNode);
}
void PushCellandInOutPin(Cell& cell, pin inp, pin outp, _PATH* PATH, MAXMIN M,
		Tr inTr, Tr outTr, delay del = 0) {
	P_NODE* pPinNode = new P_NODE(inp, cell.PinData[inp], inTr);
	PATH->vec.push_back((_NODE*) pPinNode);
	C_NODE* pCellNode = new C_NODE(new Cell(cell), del);
	PATH->vec.push_back((_NODE*) pCellNode);
	pPinNode = new P_NODE(outp, cell.PinData[outp], outTr);
	PATH->vec.push_back((_NODE*) pPinNode);
}
void BackwardPropagateReq(Net& OutNet) {
	cout << "Starting required propagation from: " << OutNet.name << endl;
	queue<Cell*> CellQ; //used to store the discovered Cells
	if (OutNet.type == OUTPUT) {
		CellQ.push(OutNet.driver.first);
		dynamic_cast<outputNet&>(OutNet).Req.CalcTmpMarg();
		dynamic_cast<outputNet&>(OutNet).Req.updateWC();
		OutNet.CalcDrvReq(dynamic_cast<outputNet&>(OutNet).Req.tmp_req, NULL,
				""); //TODO: GET RID OF DYNAMIC CAST
	} else {
		for (auto& r : OutNet.receivers) {
			if (r->cell->type == FlIPFlOP) {
				if (((FlipFlop*) r->cell)->endpoint) {
					r->cell->PinData[r->inPin].CalcTmpMarg();
					r->cell->PinData[r->inPin].updateWC();
					OutNet.CalcDrvReq(r->cell->PinData[r->inPin].tmp_req,
							r->cell, r->inPin);
					break;
				}
			}
		}
		if (OutNet.type != INPUT) {
			CellQ.push(OutNet.driver.first);
		} else {
			return;
		}
	}
	while (!CellQ.empty()) {
		Cell* curr = CellQ.front();
		CellQ.pop();
		curr->CalcInputReq();
		cout << "Calculated required for Cell's " << curr->name << " inputs"
				<< endl;
		for (auto& pair : curr->inMap) {
			auto in_Pin = pair.first;
			auto inNet = pair.second;
			if (curr->type == cellType::FlIPFlOP && inNet->isClk == false)
				continue;
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
bool CompInNets(Net* lhs, Net* rhs) {
	/*implement <
	 *
	 */

	return ((inputNet*) lhs)->Ariv.GetWCTmpMarg(GLOBM)
			< ((inputNet*) rhs)->Ariv.GetWCTmpMarg(GLOBM);
}
void BuildCritandBS(const vector<Net*>& inputNetVec, vector<_PATH*>& pPATHvec,
		MAXMIN M, PriorityQ<branchslack, BRANCHCompare>& BS, Tr state) {
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
		if ((((inputNet*) inNet)->Ariv.tmp_marg[M][state] > maxdiscovered)
				&& (numofpaths <= numPath + numbranch)) {
			cout << "Skipped input: " << inNet->name << " Margin: "
					<< ((inputNet*) inNet)->Ariv.tmp_marg[M][state] << endl;
			return;
		}
		//init critical path
		margin PATHMARG = ((inputNet*) inNet)->Ariv.tmp_marg[M][state];
		if (PATHMARG >= INT_MAX)
			continue;
		maxdiscovered = max(maxdiscovered, PATHMARG);

		_PATH* PA;
		if (M == MAX) {
			PA = new MAX_PATH(PATHMARG);
		} else {
			PA = new MIN_PATH(PATHMARG);
		}
		cout << "		Building Critical Path: " << inNet->name << " ID:" << PA->id
				<< " MARG:" << PATHMARG << endl;
		numPath++;
		auto Rcvit = inNet->getCritReciever(M, state);
		PA->start = (*Rcvit)->cell->name + "%" + (*Rcvit)->inPin;
		pPATHvec.push_back(PA);
		BSAux(inNet, M, PA, PATHMARG, maxdiscovered, BS, numbranch, true,
				state);

	}
}
void BSAux(Net* inNet, MAXMIN M, _PATH* PA, margin PATHMARG,
		margin& maxdiscovered, PriorityQ<branchslack, BRANCHCompare>& BS,
		unsigned int& numbranch, bool enforcestate, Tr state) {
//TODO: FIX STATE ENFORCEMENT
	Net* pN = inNet;
	while (pN != NULL) {
		if (!pN->isEndNet()) {
			auto Rcvit = pN->getCritReciever(M, state);
			cout << "Critical receiver of net: " << pN->name << " is "
					<< (*Rcvit)->cell->name << endl;
			Tr tr;
			if (enforcestate == false) {
				tr = (*Rcvit)->cell->PinData[(*Rcvit)->inPin].GETWCTrTmp(M);
			} else {
				tr = state;
			}
			slope inslope;
			delay netdelay = pN->getRcvDelay(Rcvit.operator *()->cell,
					Rcvit.operator *()->inPin);
			if (pN->type == INPUT) {
				inslope = ((inputNet*) pN)->Ariv.tmp_slope[M][tr];
			} else {
				inslope =
						pN->driver.first->PinData[pN->driver.second].tmp_slope[M][tr];
			}
			PushNet(*pN, PA, M, netdelay, inslope);
			auto vecIT = PA->vec.end();
			vecIT--;
			Tr outtr;
			if (enforcestate == false) {
				outtr =
						(*Rcvit)->cell->PinData[(*Rcvit)->cell->outPin].tmp_TR[M][tr];
			} else {
				outtr = (*Rcvit)->cell->PinData[(*Rcvit)->inPin].tmp_TR[M][tr];
			}
			cout << "Pushed Net " << pN->name << " to path" << endl;
			pN->RecordBS(PA, vecIT, Rcvit, PATHMARG, BS, M, tr);
			InOutTr j = GetInOut(tr, outtr);
			auto key = DelCacheKey(std::make_pair(M, j),
					std::make_pair((*Rcvit)->inPin, (*Rcvit)->cell->outPin));
			PushCellandInOutPin(*((*Rcvit)->cell), (*Rcvit)->inPin,
					(*Rcvit)->cell->outPin, PA, M, tr, outtr,
					(*Rcvit)->cell->DelCache[key]);
			cout << "Pushed Cell " << (*Rcvit)->cell->name << " to path"
					<< endl;
			numbranch = BS.getSize();
			maxdiscovered = max(maxdiscovered, BS.GetMAX());
			pN = (*Rcvit)->cell->outNet;
		} else {
			PushNet(*pN, PA, M);
			cout << "Pushed Net " << pN->name << " to path" << endl;

			receiver* prcv;
			if ((prcv = pN->GetEnd()) == NULL) {
				PA->end = pN->driver.first->name + "%" + pN->driver.second;
			} else {
				PA->end = prcv->cell->name + "%" + prcv->inPin;
			}
			pN = NULL;
		}
	}
}
void AddMin(PriorityQ<_PATH*, PATHCompare>& PAQ_d,
		PriorityQ<_PATH*, PATHCompare>& PAQ_s,
		PriorityQ<branchslack, BRANCHCompare>& BSQ_s, MAXMIN M) {
	cout << "PAQ_s min is " << PAQ_s.GetMIN() << " BSQ_s min is "
			<< BSQ_s.GetMIN() << endl;
	if (PAQ_s.GetMIN() < BSQ_s.GetMIN()) { //one of them isn't empty
		if (PAQ_s.PQ.empty()) {
			BSQ_s.PQ.pop();
			return;
		}
		_PATH* PA = PAQ_s.PQ.top();
		cout << "PATH " << PA->id << " is critical" << endl;
		PAQ_s.PQ.pop();
		PAQ_d.Add(PA);
	} else { ///build the branch and add to PAQ
		if (BSQ_s.PQ.empty()) {
			PAQ_s.PQ.pop();
			return;
		}
		cout << "	critical branch found - building it " << endl;
		_PATH* PA;
		branchslack bs = BSQ_s.PQ.top();

		BSQ_s.PQ.pop();
		cout << "Poped branchslack Queue" << endl;
		if (bs.GetMarg() >= INT_MAX)
			return;
		bs.print(M);
		if (M == MAX) {
			PA = new MAX_PATH(bs.GetMarg());
		} else {
			PA = new MIN_PATH(bs.GetMarg());
		}
		//copy the start of the path
		PA->start = bs._PA->start;
		auto end = find(bs._PA->vec.begin(), bs._PA->vec.end(), bs._pN);
		end++;
		for (auto i = bs._PA->vec.begin(); i != end; i++) {
			PA->vec.push_back(*(i));
		}
		cout << "copied Path " << bs._PA->id << endl;
		//Add branch point receiver
		auto vecIT = PA->vec.end();
		Tr tr = bs._state;
		vecIT--;
		Tr outtr =
				(*bs._prcv)->cell->PinData[(*bs._prcv)->cell->outPin].tmp_TR[M][tr];
		InOutTr j = GetInOut(tr, outtr);
		auto key = DelCacheKey(std::make_pair(M, j),
				std::make_pair((*bs._prcv)->inPin, (*bs._prcv)->cell->outPin));
		PushCellandInOutPin(*((*bs._prcv)->cell), (*bs._prcv)->inPin,
				(*bs._prcv)->cell->outPin, PA, M, tr, outtr,
				(*bs._prcv)->cell->DelCache[key]);
		cout << "Added branch point " << (*bs._prcv)->cell->name << endl;
		//build the rest of the path
		margin maxdiscovered = 0;
		unsigned int numbranch = 0;
		BSAux((*bs._prcv)->cell->outNet, M, PA, bs.GetMarg(), maxdiscovered,
				BSQ_s, numbranch, true, outtr);
		PAQ_d.Add(PA);
		cout << "	finished building path " << PA->id << " MARG " << PA->marg
				<< endl;
	}
}
void BuildPartialPATHS(vector<_PATH*>& pPATHvec,
		PriorityQ<branchslack, BRANCHCompare>& BSQ,
		PriorityQ<_PATH*, PATHCompare>& PAQ, MAXMIN M) {
	PriorityQ<_PATH*, PATHCompare> PAQ_local;
//copy paths to queue
	for (auto& e : pPATHvec) {
		PAQ_local.Add(e);
	}
	cout << "		Adding worst " << numofpaths << " PATHS TO QUEUE" << endl;
	while (!(PAQ_local.PQ.empty()) || !(BSQ.PQ.empty())) {
		if (PAQ.getSize() >= numofpaths) {
			cout << "PAQ MAX IS " << PAQ.GetMAX() << endl;
			if (min(PAQ_local.GetMIN(), BSQ.GetMIN()) >= PAQ.GetMAX()) { // all the paths have better margin
				break;
			} else {
				//add path with min margin to PAQ
				AddMin(PAQ, PAQ_local, BSQ, M);
			}
		} else {
			//add path with min margin to PAQ
			AddMin(PAQ, PAQ_local, BSQ, M);
		}
	}
//clear results
	cout << "Clearing uncritical paths and branches" << endl;
	BSQ.PQ.clear();
	pPATHvec.clear();
}
