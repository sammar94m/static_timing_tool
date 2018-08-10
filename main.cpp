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
using namespace std;
//------------------------------------------------------------
#define BUFFZISE 1024
//global
int pathidmax=0;
int pathidmin=0;

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
void allocate_table(T*** arr, int n, int m, const vector<string>& vec);

template<typename T>
void deallocate_table(T*** arr, int n);
bool endOfTemplate(ifstream& myfile);

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
		//cout<<"file was not opened"<<endl;
		return;
	}

	while (!myfile.eof()) {
		vector<string> vec;
		char tmp[BUFFZISE];
		myfile.getline(tmp, BUFFZISE);
		string s(tmp);

		if (s == "Library file:") {
			CellTemplate* cellTemplate = NULL;
			while (!myfile.eof()) {
				vec = readLine(myfile);
				if (vec.empty()) {
					break;
				}

				pin PIN;
				load LOAD;
				if (vec[0] == string("CELL")) {
					cellTemplate = new CellTemplate(vec[1]);
					CellTemplateTable[vec[1]] = cellTemplate;

				} else if (vec[0] == "PIN") {
					PIN = vec[1];
				} else if (vec[0] == "LOAD") {
					LOAD = atoi(vec[1].c_str());
					cellTemplate->temp_pinLoadMap[PIN] = LOAD;
				} else if (vec[0] == "IN_SLOPE_POINTS") {
					for (unsigned int i = 1; i < vec.size(); i++) {
						cellTemplate->IN_SLOPE_POINTS[i - 1] = atoi(
								vec[i].c_str());
					}
				} else if (vec[0] == "OUT_LOAD_POINTS") {
					for (unsigned int i = 1; i < vec.size(); i++) {
						cellTemplate->OUT_LOAD_POINTS[i - 1] = atoi(
								vec[i].c_str());
					}
				} else if (vec[0] == "ARC") {
					input_pin inpin = vec[1];
					output_pin outpin = vec[2];

					//cellTemplate->delayTable[pair<input_pin, output_pin>(inpin, outpin)] = ;

					//cellTemplate->slopeTable.insert(pair<input_pin, output_pin>(inpin, outpin)) = ;

					while (!myfile.eof()) {
						myfile.getline(tmp, BUFFZISE);
						string s(tmp);
						vec = readLine(myfile);
						if (vec.empty()) {
							break;
						}

						void ** table = NULL;
						int rows = cellTemplate->IN_SLOPE_POINTS.size();
						int cols = cellTemplate->OUT_LOAD_POINTS.size();
						if (vec[0] == "DELAY") {

							allocate_table<delay>((delay***) (&table), rows,
									cols, vec);
						} else {
							allocate_table<slope>((slope***) (&table), rows,
									cols, vec);
						}

						vector<string> tableSpec = splitString(vec[0], '_');
						MAXMIN AnlsType = tableSpec[2] == "MAX" ? MAX : MIN;
						Transitions Tr = getTransitions(tableSpec[3]);

						if (vec[0] == "DELAY") {
							cellTemplate->delayTable[pair<input_pin, output_pin>(
									inpin, outpin)].AddTable((delay**) table,
									AnlsType, Tr);
						} else {
							cellTemplate->slopeTable[pair<input_pin, output_pin>(
									inpin, outpin)].AddTable((slope**) table,
									AnlsType, Tr);
						}
						if (endOfTemplate(myfile)
								&& cellTemplate->template_name != "FF") {
							break;
						}
					}
				} else if (vec[0] == "CHECK") {
					if (cellTemplate->template_name == "FF") {
						int i = 4;
						while (i--) { // reading: MAX_RR(number)	MAX_FR(number)	MIN_RR(number)	MIN_FR(number)
							vec = readLine(myfile);
							cellTemplate->setupdata[setupdataIndex(vec[0])] =
									atoi(vec[1].c_str());
						}
					}
					if (endOfTemplate(myfile)) {
						break;
					}
				}

			}
		} else {
			cout << "file was not opened" << endl;
		}
	}

	myfile.close();
}
////-------------------------------------------------------------------------------------------------------------------------
void DesignConstraintsFile(const string& filename) {
	ifstream myfile;
	myfile.open(filename.c_str());
	if (!myfile.is_open()) {
		//cout<<"file was not opened"<<endl;
		return;
	}
	while (!myfile.eof()) {
		vector<string> vec;
		char tmp[BUFFZISE];
		myfile.getline(tmp, BUFFZISE);
		string s(tmp);

		if (s == "Design constraints file:") {
			Net* net = NULL;
			string name;
			bool isOut; // 0 input , 1 output
			bool isClk = 0;
			int SL_RISE, SL_FALL, HIGH, LOW, LOAD;
			string AR_TIME, REQ_TIME;

			while (!myfile.eof()) {
				vec = readLine(myfile);
				if (vec.empty()) { //end of cell
					net = NULL;
					break;
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

				char nextChar = cin.peek(); // to check if next line is empty
				if (nextChar == '\n') { // next line is empty.
					if (isOut) {
						net = new outputNet(name, isClk, LOW, HIGH, REQ_TIME,
								LOAD);
						Cell* dummycell = new Cell(OUTCELL,
								string("dummy_") + name, NULL);

						dummycell->outMap.insert(
								pair<output_pin, Net*>("dummy", net));

						net->set_driver(dummycell, "dummy");

						NetsTable.insert(pair<string, Net*>(name, net));
					} else {
						net = new inputNet(name, isClk, LOW, HIGH, SL_RISE,
								SL_FALL, AR_TIME);
						Cell* dummycell = new Cell(INCELL,
								string("dummy_") + name, NULL);
						dummycell->inMap.insert(
								pair<input_pin, Net*>("A", net));
						net->add_receiver(dummycell, "dummy");
						InputTable.push(dummycell);
						NetsTable.insert(pair<string, Net*>(name, net));
					}
				}

			}
		}
	}
	myfile.close();
}
//-------------------------------------------------------------------------------------------------------------------------
void NetlistFileFormat(const string& filename) {
	ifstream myfile;
	myfile.open(filename.c_str());
	if (!myfile.is_open()) {
		//cout<<"file was not opened"<<endl;
		return;
	}
	while (!myfile.eof()) {
		vector<string> vec = readLine(myfile);
		if (vec.empty()) {
			continue;
		}
		if (vec[0] == "OCC") {
			Cell* cell = new Cell(string_to_cellType(vec[2]), vec[1],
					CellTemplateTable[vec[2]]);
			CellsTable.insert(pair<string, Cell*>(vec[1], cell));
			while (!myfile.eof()) {
				vec = readLine(myfile);
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
						NetsTable.insert(pair<string, Net*>(vec[1], outPutNet));
					}
					cell->outMap.insert(pair<string, Net*>(vec[1], outPutNet));
					outPutNet->set_driver(cell, cell->name);
				}
			}
		} else {
			//cout << "error in startNewNet   vec[0] =  " << vec[0] << endl;
			return;
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


//	cout<< argc <<endl;
//	for(int i=0; i<argc;i++)
//		cout<<string(argv[i])<<endl;

	cout << " reading LibraryFile" << endl;
	LibraryFile("LibraryFile.txt");
	cout << " done reading" << endl;

	for(auto it = CellTemplateTable.begin() ; it != CellTemplateTable.end(); ++it ){
		cout<<it->first<<" : "<<endl;
		it->second->print();
	}

//	cout << " reading DesignConstraintsFile" << endl;
//	DesignConstraintsFile("DesignConstraintsFile.txt");
//	cout<<" reading NetlistFileFormat"<<endl;
//	NetlistFileFormat("NetlistFileFormat.txt");

	return 0;
}

//------------------------------------------------------------------------------------------

int setupdataIndex(string s) { //e.g: s = MAX_RR
	if (s == "MAX_RR")
		return MAX_RR;
	if (s == "MAX_FR")
		return MAX_FR;
	if (s == " MIN_RR")
		return MIN_RR;
	return MIN_FR;

}

Transitions getTransitions(string tr) {
	if (tr == "FF")
		return FF;
	if (tr == "FR")
		return FR;
	if (tr == "RF")
		return FR;
	return RR;
}

const vector<string> splitString(const string& s, const char& c) {
	string buff { "" };
	vector<string> v;

	for (auto n : s) {
		if (n != c)
			buff += n;
		else if (n == c && buff != "") {
			v.push_back(buff);
			buff = "";
		}
	}
	if (buff != "")
		v.push_back(buff);

	return v;
}

template<typename T>
void allocate_table(T*** arr, int n, int m, const vector<string>& vec) {
	*arr = (T**) malloc(n * sizeof(T*));
	if (*arr == NULL)
		return;
	for (int i = 0; i < n; i++) {
		(*arr)[i] = (T*) malloc(m * sizeof(T));
		if ((*arr)[i] == NULL) {
			for (int j = 0; j < i; j++) {
				free((*arr)[j]);
			}
			free(*arr);
			return;
		}
	}
	if (arr) {
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < m; j++) {

				*arr[i][j] = (T) stoi(vec[i * m + j], 0, 10);
			}
		}
	} else {
		cout << "table allocation failed" << endl;
	}
}
template<typename T>
void deallocate_table(T*** arr, int n) {
	for (int i = 0; i < n; i++)
		free((*arr)[i]);
	free(*arr);
}

bool endOfTemplate(ifstream& myfile) {
	char CellWord[6];
	int k = 0;
	for (int i = 0; i < 5 && (!myfile.eof()); i++) {
		CellWord[i] = myfile.get();
		k++;
	}
	CellWord[5] = 0;

	bool isCellWord = false;
	if (string(CellWord) == "CELL:") {
		isCellWord = true;
	}
	while (k > 0) {
		myfile.putback(CellWord[k - 1]);
	}
	return isCellWord;
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

void split(const string &s, const char* delim, vector<string> & v) {
// to avoid modifying original string
// first duplicate the original string and return a char pointer then free the memory
	char * dup = strdup(s.c_str());
	char * token = strtok(dup, delim);
	while (token != NULL) {
		v.push_back(string(token));
		// the call is treated as a subsequent calls to strtok:
		// the function continues from where it left in previous invocation
		token = strtok(NULL, delim);
	}
	free(dup);
}

vector<string> readLine(ifstream& myfile) {
	vector<string> vec;
	char tmp[BUFFZISE];
	myfile.getline(tmp, BUFFZISE);
	string s(tmp);
	string delim = " ():,->";
	split(s, delim.c_str(), vec);
	for (unsigned int i = 0; i < vec.size(); i++) {
		if (*(vec[i].end()) == ',') {
			vec[i].erase(vec[i].end());
		}
	}
	return vec;
}

cellType string_to_cellType(string s) {
	if (s == "FF")
		return FlIPFlOP;
	return COMB;
}





