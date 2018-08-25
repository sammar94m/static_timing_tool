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
#include "Cell.h"
#include "Net.h"
#include "OutputNet.h"
#include "InputNet.h"
#include "enums.h"
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

using namespace std;
using namespace boost;
//------------------------------------------------------------
#define BUFFZISE 1024
//global
int pathidmax = 0;
int pathidmin = 0;

queue<Cell*> InputTable;
queue<Cell*> InputDataTable;
queue<Cell*> InputClkTable;

map<string, Net*> NetsTable;
map<string, Cell*> CellsTable;
map<string, CellTemplate*> CellTemplateTable;

//const int BUFFZISE = 1000;
//------------------------------ HELP FUNCTIONS -------------------------------------------

int setupdataIndex(string s);

Transitions getTransitions(string tr);

const vector<string> splitString(const string& s, const char& c);

template<typename T>
T** create_table(int n, int m, const vector<string>& vec);

template<typename T>
void decreate_table(T*** arr, int n);
//bool endOfTemplate(ifstream& myfile);

//void printNet(Net * net);

//void printMainCell(Cell* cell);
void split(const string &s, const char* delim, vector<string> & v);

vector<string> readLine(ifstream& myfile);
cellType string_to_cellType(string s);

//cellType cellType(string s);
//------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------------------------------
void LibraryFile(const string& filename) {
	ifstream myfile;
	myfile.open(filename.c_str());
	if (!myfile.is_open()) {
		cout << "file was not opened" << endl;
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
			for (int i = 0; i < vec.size(); i++)
				cout << " " << vec[i];
			cout << endl;

			if (vec.empty())
				continue;

			if (vec[0] == "CELL") {
				cout << "CELL" << endl;
				cellTemplate = new CellTemplate(vec[1]);
				if (!cellTemplate) {
					cout << "failed to alloc cellTemplate" << endl;
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
				cout << "ARC" << endl;
				input_pin inpin = vec[1];
				output_pin outpin = vec[2];

				//cellTemplate->delayTable[pair<input_pin, output_pin>(inpin, outpin)] = ;

				//cellTemplate->slopeTable.insert(pair<input_pin, output_pin>(inpin, outpin)) = ;

				while (!myfile.eof()) {
					vec = readLine(myfile);
					for (int i = 0; i < vec.size(); i++)
						cout << " " << vec[i];
					cout << endl;
					if (vec.empty()) {
						cout << "ARC reading done" << endl;
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
					else throw("invalid AnlsType");

					Transitions Tr = getTransitions(tableSpec[3]);

					if (tableSpec[0] == "DELAY") {
						cout << "delay table added" << endl;
						cellTemplate->delayTable[pair<input_pin, output_pin>(
								inpin, outpin)].AddTable((delay**) table,
								AnlsType, Tr, rows, cols);
						table = NULL;

					} else {
						cout << "slope table added" << endl;
						cellTemplate->slopeTable[pair<input_pin, output_pin>(
								inpin, outpin)].AddTable((slope**) table,
								AnlsType, Tr, rows, cols);
						table = NULL;

					}
					cout << "  good till now" << endl;
				}

			} else if (vec[0] == "CHECK") {
				cout << " ---- CHECK" << endl;
				if (cellTemplate != NULL
						&& cellTemplate->template_name == "FF") {
					int i = 4;
					while (i--) {
						vec = readLine(myfile);
						for (int i = 0; i < vec.size(); i++)
							cout << " " << vec[i];
						cout << endl;
						cellTemplate->setupdata[setupdataIndex(vec[0])] = atoi(
								vec[1].c_str());
					}
				}

			}

		}
	}

	myfile.close();
}

//-------------------------------------------------------------------------------------------------------------------------
void DesignConstraintsFile(const string& filename) {

	ifstream myfile;
	myfile.open(filename.c_str());
	if (!myfile.is_open()) {
		cout << "file was not opened" << endl;
		return;
	}

	string s;
	getline(myfile, s, '\r');

	if (s == "Design constraints file:") {
		Net* net = NULL;
		string name;
		bool isOut; // 0 input , 1 output
		bool isClk = 0;
		int SL_RISE, SL_FALL, HIGH, LOW, LOAD;
		string AR_TIME, REQ_TIME;

		while (!myfile.eof()) {
			vector<string> vec = readLine(myfile);
			for (int i = 0; i < vec.size(); i++)
				cout << " " << vec[i];
			cout << endl;

			if (vec.empty()) { //end of cell
				if (net) { // next line is empty.
					if (isOut) {
						net = new outputNet(name, isClk, LOW, HIGH, REQ_TIME,
								LOAD);
						Cell* dummycell = new Cell(OUTCELL,
								string("dummy_") + name,
								NULL);

						dummycell->outMap.insert(
								pair<output_pin, Net*>("dummy", net));

						net->set_driver(dummycell, "dummy");

						NetsTable[name] =  net;
					} else {
						net = new inputNet(name, isClk, LOW, HIGH, SL_RISE,
								SL_FALL, AR_TIME);
						Cell* dummycell = new Cell(INCELL,
								string("dummy_") + name,
								NULL);
						dummycell->inMap.insert(
								pair<input_pin, Net*>("A", net));
						net->add_receiver(dummycell, "dummy");
						InputTable.push(dummycell);
						NetsTable[name]= net;
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
				isOut = 0;
			} else if (vec[0] == "TYPE") {
				isClk = vec[1] == "CLOCK";
			} else if (vec[0] == "AR_TIME") {
				AR_TIME = vec[1] + vec[2] + vec[3];
			} else if (vec[0] == "SL_RISE") {
				SL_RISE = atoi(vec[1].c_str());
			} else if (vec[0] == "SL_FALL") {
				SL_FALL = atoi(vec[1].c_str());
			} else if (vec[0] == "HIGH") {
				HIGH = atoi(vec[1].c_str());
			} else if (vec[0] == "LOW") {
				LOW = atoi(vec[1].c_str());
			} else if (vec[0] == "REQ_TIME") {
				REQ_TIME = vec[1] + vec[2] + vec[3];
			} else if (vec[0] == "LOAD") {
				LOAD = atoi(vec[1].c_str());
			}

			//char nextChar = cin.peek(); // to check if next line is empty

		}
	}

	myfile.close();
}

//-------------------------------------------------------------------------------------------------------------------------

void NetlistFileFormat(const string& filename) {
	ifstream myfile;
	myfile.open(filename.c_str());
	if (!myfile.is_open()) {
		cout << "file was not opened" << endl;
		return;
	}

	string s;
	getline(myfile, s, '\r');

	if (s == "Netlist file format:") {
		while (!myfile.eof()) {
			vector<string> vec = readLine(myfile);
			for (int i = 0; i < vec.size(); i++)
				cout << " " << vec[i];
			cout << endl;
			if (vec.empty()) {
				continue;
			}
			if (vec[0] == "OCC") {
				Cell* cell = new Cell(string_to_cellType(vec[2]), vec[1],
						CellTemplateTable[vec[2]]);
				CellsTable.insert(pair<string, Cell*>(vec[1], cell));
				while (!myfile.eof()) {
					vec = readLine(myfile);
					for (int i = 0; i < vec.size(); i++)
						cout << " " << vec[i];
					cout << endl;
					if (vec.empty()) { //end of cell
						break;
					}
					if (vec[0] == "IN") {
						Net* driverNet = NULL;
						driverNet = NetsTable.find(vec[2])->second;
						if (driverNet != NULL) { // driver exists
						} else {
							// this shoud be fixed
							driverNet = new Net(vec[2], Local, false, 0, 0);
							//cout << "error input net does not exist" << endl;
						}
						driverNet->add_receiver(cell, vec[1]);
						cell->inMap.insert(pair<pin, Net*>(vec[1], driverNet));
					} else if (vec[0] == "OUT") {
						Net* outPutNet = NULL;
						outPutNet = NetsTable.find(vec[2])->second;
						if (outPutNet == NULL) {
							outPutNet = new Net(vec[2], OUTPUT, 0, 0, 0);
							NetsTable.insert(
									pair<string, Net*>(vec[1], outPutNet));
						}
						cell->outMap.insert(
								pair<string, Net*>(vec[1], outPutNet));
						outPutNet->set_driver(cell, cell->name);
					}
				}
			} else {
				//cout << "error in startNewNet   vec[0] =  " << vec[0] << endl;
				return;
			}
		}
	}
}

/* files:
 *  *		LibraryFile.txt
 *		NetlistFileFormat.txt
 *		DesignConstraintsFile.txt

 *		ParasiticsInterconnectFile.txt
 * */

//void dfsPrint() {
//	while (!InputTable.empty()) {
//		Cell* cell = InputTable.front();
//		if (!cell) {
//			cout << "null cell" << endl;
//			break;
//		}
//		dfsPrintaux(cell);
//	}
//}
//
//void dfsPrintaux(Cell* cell) {
//	if (!cell) {
//		cout << "null cell" << endl;
//		return;
//	}
//	auto outmap = cell->getOutMap();
//	for (auto iter = outmap.begin(); iter != outmap.end(); ++iter) {
//		cout << (*iter).first << " --> ";
//		auto net = (*iter).second;
//		auto rcvCells = net->receivers;
//		for (auto cellIter = rcvCells.begin(); cellIter != rcvCells.end();
//				++cellIter) {
//			receiver* rcv = (*cellIter);
//			dfsPrintaux(rcv->cell);
//		}
//	}
//
//}
int main(int argc, char* argv[]) {

//	cout << " reading LibraryFile" << endl;
//	LibraryFile("LibraryFile.txt");
//	cout << " LibraryFile done" << endl;
//	cout
//			<< "-----------------------------------------------------------------------"
//			<< endl;
//
//	for (auto it = CellTemplateTable.begin(); it != CellTemplateTable.end();
//			++it) {
//		cout << it->first << " : " << endl;
//		it->second->print();
//	}

	cout << " reading DesignConstraintsFile" << endl;
	DesignConstraintsFile("DesignConstraintsFile.txt");
	cout << "  DesignConstraintsFile done"<< endl;
	cout << "-----------------------------------------------------------------------" << endl;

		for (auto it = NetsTable.begin(); it != NetsTable.end();++it) {
			cout << it->first << " : " << endl;
			it->second->print();
		}

//	cout << " reading NetlistFileFormat" << endl;
//	NetlistFileFormat("NetlistFileFormat.txt");
//	cout << " NetlistFileFormat done " << endl;

	return 0;
}

//------------------------------------------------------------------------------------------

int setupdataIndex(string s) { // bug
	if (s == "MAX_RR")
		return FlopSetup::MAX_RR;
	if (s == "MAX_FR")
		return FlopSetup::MAX_FR;
	if (s == " MIN_RR")
		return FlopSetup::MIN_RR;
//	if (s == " MIN_FR")
	else
		return FlopSetup::MIN_FR;
//	else throw("invslid FlopSetup")

}

Transitions getTransitions(string tr) {
	cout<<"tr="<<tr<<endl;
	if (tr == "FF"){
		return Transitions::FF;
	}else if (tr == "FR"){
		return Transitions::FR;
	}else if (tr == "RF"){
		return Transitions::RF;
	}else if( tr == "RR"){
		return Transitions::RR;
	}else throw("invalid Transition");
}

template<typename T>
T** create_table(int rows, int cols, const vector<string>& vec) {

	T** matrix = new T*[rows];
	for (int i = 0; i < rows; ++i)
		matrix[i] = new T[cols];

//	cout <<rows<<" x "<<cols<<endl;
	for (int i = 0; i < vec.size(); i++) {
		if (vec[i] != "") {

			//cout << "!!!   " << vec[i] << " to " << i / (rows - 1) << "  "
			//	<< i % (cols - 1) << endl;

			matrix[i / cols][i % cols] = atoi(vec[i].c_str()); // (T) stoi(vec[i], 0, 10);
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

//void printNet(Net * net) {
//	if (!net)
//		return;
//	for (auto rcvIt = net->receivers.begin(); rcvIt != net->receivers.end();
//			++rcvIt) {
//		cout << rcvIt->first->name << endl;
//		for (auto outIt = rcvIt->first->outMap.begin();
//				outIt != rcvIt->first->outMap.end(); ++outIt) {
//			printNet(outIt->second);
//		}
//	}
//}

//void printMainCell(Cell* cell) {
//	for (auto inputNet = cell->inMap.begin(); inputNet != cell->inMap.end();
//			inputNet++) {
//		cout << inputNet->first << "----------------------" << endl << endl;
//		printNet(inputNet->second);
//	}
//}

vector<string> readLine(ifstream& myfile) {
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

