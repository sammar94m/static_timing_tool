// VLSI_PREJECT.cpp : Defines the entry point for the console application.
//
//#include "stdafx.h"
//============================================================================
// Name        : vlsiProject.cpp
// Author      : sammar
// Version     : 1
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

//------------------------------------------------------------
#include "Createnetlist.h"

using namespace std;
using namespace boost;
//------------------------------------------------------------

//const int BUFFZISE = 1000;
//------------------------------ HELP FUNCTIONS -------------------------------------------

int setupdataIndex(string s);

Transitions getTransitions(string tr);

template<typename T>
T** create_table(int n, int m, const vector<string>& vec);

template<typename T>
void decreate_table(T*** arr, int n);

vector<string> readLine(ifstream& myfile);
cellType string_to_cellType(string s);

//--------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------
void LibraryFile(const string& filename) {
	ifstream myfile;
	myfile.open(filename.c_str());
	if (!myfile.is_open()) {
		throw "file was not opened";
		return;
	}
	string s;
	getline(myfile, s, '\r');

	if (s == "Library file:") {
		CellTemplate* cellTemplate = NULL;
		pin PIN;
		load LOAD;
		while (!myfile.eof()) {
			vector<string> vec = readLine(myfile);
//			for (int i = 0; i < vec.size(); i++)
//				cout << " " << vec[i];
//			cout << endl;

			if (vec.empty())
				continue;

			if (vec[0] == "CELL") {
//				cout << "CELL" << endl;
				cellTemplate = new CellTemplate(vec[1]);
				if (!cellTemplate) {
					throw "failed to alloc cellTemplate"; // << endl;
				}
				CellTemplateTable[vec[1]] = cellTemplate;

			} else if (vec[0] == "PIN") {
				PIN = vec[1];
			} else if (vec[0] == "LOAD") {
				LOAD = atoi(vec[1].c_str());
				cellTemplate->temp_pinLoadMap[PIN] = LOAD;
			} else if (vec[0] == "IN_SLOPE_POINTS") {
				for (unsigned int i = 1; i < vec.size(); i++) {
					if (vec[i] != "")
						cellTemplate->IN_SLOPE_POINTS.push_back(
								atoi(vec[i].c_str()));
				}
			} else if (vec[0] == "OUT_LOAD_POINTS") {
				for (unsigned int i = 1; i < vec.size(); i++) {
					if (vec[i] != "")
						cellTemplate->OUT_LOAD_POINTS.push_back(
								atoi(vec[i].c_str()));
				}
			} else if (vec[0] == "ARC") {
//				cout << "ARC" << endl;
				input_pin inpin = vec[1];
				output_pin outpin = vec[2];

				//cellTemplate->delayTable[pair<input_pin, output_pin>(inpin, outpin)] = ;

				//cellTemplate->slopeTable.insert(pair<input_pin, output_pin>(inpin, outpin)) = ;

				while (!myfile.eof()) {
					vec = readLine(myfile);
//					for (int i = 0; i < vec.size(); i++)
//						cout << " " << vec[i];
//					cout << endl;

					if (vec.empty()) {
//						cout << "ARC reading done" << endl;
						break;
					}

					void ** table = NULL;
					int rows = cellTemplate->IN_SLOPE_POINTS.size();
					int cols = cellTemplate->OUT_LOAD_POINTS.size();

					//cout<<rows<<cols<<endl;
					vector<string> tableSpec;
					boost::split(tableSpec, vec[0], boost::is_any_of("_"));

					vector<string> tablevec;
					for (int i = 1; i < vec.size(); i++) {
						if (vec[i] != "")
							tablevec.push_back(vec[i]);
					}

					if (tableSpec[0] == "DELAY") {
						table = (void**) create_table<delay>(rows, cols,
								tablevec);
					} else {
						table = (void**) create_table<slope>(rows, cols,
								tablevec);
					}

					MAXMIN AnlsType;

					if (tableSpec[2] == "MAX")
						AnlsType = MAX;
					else if (tableSpec[2] == "MIN")
						AnlsType = MIN;
					else
						throw("invalid AnlsType");

					Transitions Tr = getTransitions(tableSpec[3]);

					if (tableSpec[0] == "DELAY") {
//						cout << "delay table added" << endl;
						cellTemplate->delayTable[pair<input_pin, output_pin>(
								inpin, outpin)].AddTable((delay**) table,
								AnlsType, Tr, rows, cols);
						table = NULL;

					} else {
//						cout << "slope table added" << endl;
						cellTemplate->slopeTable[pair<input_pin, output_pin>(
								inpin, outpin)].AddTable((slope**) table,
								AnlsType, Tr, rows, cols);
						table = NULL;

					}
//					cout << "  good till now" << endl;
				}

			} else if (vec[0] == "CHECK") {
//				cout << " ---- CHECK" << endl;
				if (cellTemplate != NULL
						&& cellTemplate->template_name == "FF") {
					int i = 4;
					while (i--) {
						vec = readLine(myfile);
//						for (int i = 0; i < vec.size(); i++)
//							cout << " " << vec[i];
//						cout << endl;
						cellTemplate->setupdata[setupdataIndex(vec[0])] = atoi(
								vec[1].c_str());
					}
				}

			}

		}
	}

	myfile.close();
}

//--------------------------------------------------------------------------------------
void DesignConstraintsFile(const string& filename) {

	ifstream myfile;
	myfile.open(filename.c_str());
	if (!myfile.is_open()) {
		throw "file was not opened"; // << endl;
		return;
	}

	string s;
	getline(myfile, s, '\r');

	if (s == "Design constraints file:") {
		Net* net = NULL;
		string name;
		int isOut = 0; // 0 input , 1 output
		bool isClk = 0;
		int SL_RISE = 0, SL_FALL = 0, HIGH = 0, LOW = 0, LOAD = 0;
		string AR_TIME, REQ_TIME;

		while (!myfile.eof()) {
			vector<string> vec = readLine(myfile);
//			for (int i = 0; i < vec.size(); i++)
//				cout << " " << vec[i];
//			cout << endl;

			if (vec.empty()) { //end of cell

				if (isOut == 1) {
					net = new outputNet(name, isClk, REQ_TIME, LOAD);
					NetsTable[name] = net;
					//cout<<"-----"<<name<<"----"<<endl;
				} else if(isOut == 2){
					net = new inputNet(name, isClk, SL_RISE, SL_FALL, AR_TIME, HIGH, LOW);
					//cout<<"-----"<<name<<"----"<<endl;
					NetsTable[name] = net;
					InputTable.push(net);
					if (isClk) {
						InputClkTable.push(net);
						if (name == "CLK") {
							inputNet::setRefclk(&((inputNet*)net)->clk);
						}
					} else {
						InputDataTable.push(net);
					}
				}

				net = NULL;
				continue;
			}

			if (vec[0] == "OUTPUT") {
				name = vec[1];
				isOut = 1;
			} else if (vec[0] == "INPUT") {
				name = vec[1];
				isOut = 2;
			} else if (vec[0] == "TYPE") {
				isClk = vec[1] == "CLOCK";
			} else if (vec[0] == "AR_TIME") {
				for (int i = 0; i < vec.size(); ++i)
				//	cout<< vec[i]<<" | ";
				//cout<<endl;

				AR_TIME = vec[1] + ' ' + vec[2]+' ' + vec[3] + '\0';

			} else if (vec[0] == "SL_RISE") {
				SL_RISE = atoi(vec[1].c_str());
			} else if (vec[0] == "SL_FALL") {
				SL_FALL = atoi(vec[1].c_str());
			} else if (vec[0] == "HIGH") {
				HIGH = atoi(vec[1].c_str());
			} else if (vec[0] == "LOW") {
				LOW = atoi(vec[1].c_str());
			} else if (vec[0] == "REQ_TIME") {
				REQ_TIME = vec[1] + ' ' + vec[2] +' '+ vec[3] + '\0';
			} else if (vec[0] == "LOAD") {
				LOAD = atoi(vec[1].c_str());
			}

			//char nextChar = cin.peek(); // to check if next line is empty

		}
	}

	myfile.close();
}

//--------------------------------------------------------------------------------------

void NetlistFileFormat(const string& filename) {
	ifstream myfile;
	myfile.open(filename.c_str());
	if (!myfile.is_open()) {
		throw "file was not opened"; // << endl;
		return;
	}

	string s;
	getline(myfile, s, '\r');

	if (s == "Netlist file format:") {
		while (!myfile.eof()) {
			vector<string> vec = readLine(myfile);

			if (vec.empty()) {
				continue;
			}
			if (vec[0] == "OCC") {
				CellTemplate* ctemplate = CellTemplateTable.find(vec[2])->second;

				if(!ctemplate){
					cout<<"error:no such template: " +vec[2]<< endl;
					continue;
				}

				Cell* cell = new Cell(string_to_cellType(vec[2]), vec[1],ctemplate);
				CellsTable.insert(pair<string, Cell*>(vec[1], cell));
				while (!myfile.eof()) {
					vec = readLine(myfile);

					if (vec.empty()) { //end of cell
						break;
					}
					if (vec[0] == "IN") {
						Net* driverNet = NULL;
						driverNet = NetsTable.find(vec[2])->second;


						if (!driverNet) {

							driverNet = new Net(vec[2], false);
						}
						driverNet->add_receiver(cell, vec[1]);
						cell->inMap[vec[1]] = driverNet;

					} else if (vec[0] == "OUT") {

						//cout<<"  -----"<<vec[2]<<"-------"<<endl;
						Net* outPutNet = NULL;
						outPutNet = NetsTable.find(vec[2])->second;

						if (!outPutNet) {
							outPutNet = new Net(vec[2], false);
							NetsTable[vec[2]] = outPutNet;
						}

						cell->outMap[vec[1]] = outPutNet;
						outPutNet->set_driver(cell, cell->name);
					}
				}
			}
		}
	}
}

//--------------------------------------------------------------------------------------

void ParasiticsInterconnectFile(const string& filename) {
	ifstream myfile;
	myfile.open(filename.c_str());
	if (!myfile.is_open()) {
		throw "file was not opened"; // << endl;
		return;
	}

	string s;
	getline(myfile, s, '\r');

	Net* net = NULL;

	if (s == "Parasitics interconnect file:") {
		while (!myfile.eof()) {
			vector<string> vec = readLine(myfile);
//			for (int i = 0; i < vec.size(); i++)
//				cout << " " << vec[i];
//			cout << endl;

			if (vec.empty()) {
				net = NULL;
				continue;
			}

			if (vec[0] == "NET") {
				net = NetsTable[vec[2]];
				if (!net) {
					cout << "net " << vec[2] << " not found in NetsTable"
							<< endl;
				}
			} else if (net) {
				if (net->driver.first->name == vec[0] && net->name == vec[1]) {
					for (auto itr = net->receivers.begin();
							itr != net->receivers.end(); ++itr) {

						(*itr)->set_slopeDeg_netDely(vec[2], vec[3],
								atoi(vec[4].c_str()), atoi(vec[5].c_str()));
					}
				}

			}

		}
	}
}
//--------------------------------------------------------------------------------------
void dfsPrintaux(Cell* cell) {
	if (!cell) {
		cout << "null cell" << endl;
		return;
	} else
		cout << "dfs start from cell " << cell->name << endl;
	auto outmap = cell->getOutMap();
	for (auto iter = outmap.begin(); iter != outmap.end(); ++iter) {
		cout << (*iter).first << " --> ";
		auto net = (*iter).second;
		auto rcvCells = net->receivers;
		for (auto cellIter = rcvCells.begin(); cellIter != rcvCells.end();
				++cellIter) {
			receiver* rcv = (*cellIter);
			dfsPrintaux(rcv->cell);
		}
	}
}

void dfsPrint() {
	if (InputTable.empty()) {
		cout << "empty table " << endl;
	} else
		cout << "InputTable size = " << InputTable.size() << endl;

	while (!InputTable.empty()) {
		Net* net = InputTable.front();
		if (!net) {
			cout << "null net" << endl;
			break;
		}

		for (auto iter = net->receivers.begin(); iter != net->receivers.end();
				++iter) {
			dfsPrintaux((*iter)->cell);
		}

		InputTable.pop();
	}
}

int setupdataIndex(string s) { // bug
	if (s == "MAX_RR")
		return FlopSetup::MAX_RR;
	if (s == "MAX_FR")
		return FlopSetup::MAX_FR;
	if (s == " MIN_RR")
		return FlopSetup::MIN_RR;
	else
		return FlopSetup::MIN_FR;

}

Transitions getTransitions(string tr) {
	//cout << "tr=" << tr << endl;
	if (tr == "FF") {
		return Transitions::FF;
	} else if (tr == "FR") {
		return Transitions::FR;
	} else if (tr == "RF") {
		return Transitions::RF;
	} else if (tr == "RR") {
		return Transitions::RR;
	} else
		throw("invalid Transition");
}

template<typename T>
T** create_table(int rows, int cols, const vector<string>& vec) {

	T** matrix = new T*[rows];
	for (int i = 0; i < rows; ++i)
		matrix[i] = new T[cols];

	for (int i = 0; i < vec.size(); i++) {
		if (vec[i] != "") {
			matrix[i / cols][i % cols] = atoi(vec[i].c_str());// (T) stoi(vec[i], 0, 10);
		}
	}

	return matrix;
}

template<typename T>
void decreate_table(T*** arr, int n) {
	for (int i = 0; i < n; i++)
		free((*arr)[i]);
	free(*arr);
}

vector<string> readLine(ifstream & myfile) {
	vector<string> vec, goodvec;
	string line;
	getline(myfile, line);
	string delim = " ():,->";
	boost::split(vec, line, boost::is_any_of(" ():,->\r"));

	for (unsigned int i = 0; i < vec.size(); i++)
		if (vec[i] != "")
			goodvec.push_back(vec[i]);

	return goodvec;
}

cellType string_to_cellType(string s) {
	if (s == "FF")
		return cellType::FlIPFlOP;
	return cellType::COMB;
}
