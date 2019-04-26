// VLSI_PREJECT.cpp : Defines the entry point for the console application.
//

using namespace std;

#include "Createnetlist.h"
#include "Datapath.h"

#define BUFFZISE 1024
//global
int pathidmax = 0;
int pathidmin = 0;
map<int, string> MAXpaths;
map<int, string> MINpaths;

vector<Net*> InputTable;
vector<Net*> InputDataTable;
vector<Net*> InputClkTable;
vector<Net*> OutputTable;
map<string, Net*> NetsTable;
map<string, Cell*> CellsTable;
map<string, CellTemplate*> CellTemplateTable;

/* files:
 *  *	LibraryFile.txt
 *		NetlistFileFormat.txt
 *		DesignConstraintsFile.txt
 *		ParasiticsInterconnectFile.txt
 * */
void printCircuit() {
	for (auto it = CellsTable.begin(); it != CellsTable.end(); ++it) {
		cout << it->first << " (" << it->second->Template->template_name << ")"
				<< endl;
		for (auto pinItr = it->second->inMap.begin();
				pinItr != it->second->inMap.end(); ++pinItr) {
			cout << "IN " << pinItr->first << "(" << pinItr->second->name << ")"
					<< endl;
		}
		cout << "OUT " << it->second->outPin << "(" << it->second->outNet->name
				<< ")" << endl;

		cout << endl;
	}
}
void list_command(vector<string>& vec) {
	if (vec[1] == "cells") {
		cout << "..." << endl;
		for (auto it = CellsTable.begin(); it != CellsTable.end(); ++it) {
			cout << "name: " << it->first << endl;
		}
	} else if (vec[1] == "nets") {
		cout << "..." << endl;
		cout << "Name " << "Type " << "isClock " << "Driver" << endl;
		for (auto& it : NetsTable) {
			cout << it.first << " ";
			switch (it.second->type) {
			case LOCAL:
				cout << "LOCAL";
				break;
			case INPUT:
				cout << "INPUT";
				break;
			case OUTPUT:
				cout << "OUTPUT";
				break;

			}
			if (it.second->isClk) {
				cout << " CLK";
			} else {
				cout << " DATA";
			}
			if (it.second->driver.first == NULL) {
				cout << " NULL" << endl;
			} else {
				cout << " " << it.second->driver.first->name << endl;
			}

		}
	} else if (vec[1] == "input") {
		cout << "..." << endl;
		//InputTable;
		// InputDataTable;
		// InputClkTable;
	} else if (vec[1] == "output") {
		cout << "..." << endl;
		cout << "Name " << "Type " << "isClock " << "Driver" << endl;
		for (auto& it : OutputTable) {
			cout << it->name << " ";
			switch (it->type) {
			case LOCAL:
				cout << "LOCAL";
				break;
			case INPUT:
				cout << "INPUT";
				break;
			case OUTPUT:
				cout << "OUTPUT";
				break;

			}
			if (it->isClk) {
				cout << " CLK";
			} else {
				cout << " DATA";
			}
			if (it->driver.first == NULL) {
				cout << " NULL" << endl;
			} else {
				cout << " " << it->driver.first->name << endl;
			}
		}

	} else if (vec[1] == "templates") {
		cout << "..." << endl;
		for (auto it = CellTemplateTable.begin(); it != CellTemplateTable.end();
				++it) {
			cout << it->first << "--" << (it->second != NULL) << endl;
		}

	} else if (vec[1] == "clocks") {
		cout << "..." << endl;
		cout << "CLOCK | " << "RECEIVER|    " << "AR TIME | " << "HIGH   | "
				<< "LOW |  " << "REF" << endl;
		for (auto it = NetsTable.begin(); it != NetsTable.end(); ++it) {
			if (!(it->second->isClk))
				continue;
			cout << it->second->name << endl;
		}
	} else if (vec[1] == "receivers") {
		if (vec.size() < 3) {
			cout << "Enter net name" << endl;
		} else {
			auto it = NetsTable.find(vec[2]);
			if (it == NetsTable.end()) {
				cout << "Net not found" << endl;
				return;
			} else {
				cout << it->second->name << ":" << endl << "Receiver " << "Pin"
						<< endl;
				for (auto rcvit = it->second->receivers.begin();
						rcvit != it->second->receivers.end(); rcvit++) {
					cout << (*rcvit)->cell->name << " " << (*rcvit)->inPin
							<< endl;
				}
			}
		}

	} else {
		cout << "what?!" << endl;

	}
}

void print_command(vector<string>& vec) {
	if (vec[1] == "receivers") {
		cout << "..." << endl;

	} else if (vec[1] == "circuit") {
		cout << "..." << endl;
		printCircuit();
	} else if (vec[1] == "cell") {
		cout << "..." << endl;
		if (vec.size() < 3) {
			cout << "Enter cell name" << endl;
		} else {
			Cell* pCell = CellsTable[vec[2]];
			for (auto& i : pCell->PinData) {
				cout << "Pin " << i.first << endl;
				i.second.Print();

			}
		}
	} else if (vec[1] == "template") {
		if (vec.size() < 3) {
			cout << "Enter template name" << endl;
		} else {
			CellTemplate* pTmp = CellTemplateTable[vec[2]];
			cout << pTmp->template_name << endl;
			cout << "IN_SLOPE_POINTS: ";
			for (auto& s : pTmp->IN_SLOPE_POINTS) {
				cout << s << " ";
			}
			cout << endl;
			cout << "OUT_LOAD_POINTS: ";
			for (auto& s : pTmp->OUT_LOAD_POINTS) {
				cout << s << " ";
			}
			cout << endl;
			for (auto& x : pTmp->delayTable) {
				cout << "in " << x.first.first << " out " << x.first.second
						<< endl;
				for (auto& y : x.second._16tables) {
					for (int i = 0; i < pTmp->IN_SLOPE_POINTS.size(); i++) {
						for (int j = 0; j < pTmp->OUT_LOAD_POINTS.size(); j++) {
							cout << y.second[i][j];
						}

					}
					cout << endl;
				}
			}
		}

	} else {
		cout << "what?!" << endl;

	}
}

void run_command(vector<string>& vec) {
	if (vec[1] == "clock") {
		cout << "..." << endl;
//		clockPathDelayCalc();
	} else if (vec[1] == "data") {
		dataPathDelayCalc();
	} else {
		cout << "what?!" << endl;

	}
}
unsigned int numofpaths = INT_MAX;
void startShellCommands() {
	string input;
	/*
	 * init
	 */
	string s;
	unsigned int n;
	cout << "Enter Library File:" << endl;
	cin >> s;
	cout << "Reading Library File" << endl;
	LibraryFile(s);
	cout << "Library File done" << endl;

	cout << "Enter Design Constraints File:" << endl;
	cin >> s;
	cout << "Reading Design Constraints File" << endl;
	DesignConstraintsFile(s);
	cout << "Design Constraints File done" << endl;

	cout << "Enter Netlist File:" << endl;
	cin >> s;
	cout << "Reading Netlist File" << endl;
	NetlistFileFormat(s);
	cout << "Netlist File done " << endl;

	cout << "Enter Parasitics Interconnect File:" << endl;
	cin >> s;
	cout << "Reading Parasitics Interconnect File" << endl;
	ParasiticsInterconnectFile(s);
	cout << "Parasitics Interconnect File done " << endl;
	MarkClks();

	cout << "Enter Num of Requested pathes:" << endl;
	cin >> n;
	numofpaths = n;
	cin>> s;
	cout
			<< "-----------------------------------------------------------------------"
			<< endl;
	while (1) {
		cout << ">>";
		std::getline(std::cin, input);
		vector<string> vec(10);

		boost::split(vec, input, boost::is_any_of(" "));
		string shellCommand = vec[0];

		if (vec[0] == "list") {
			list_command(vec);
		} else if (vec[0] == "print") {
			print_command(vec);
		} else if (vec[0] == "run") {
			run_command(vec);
		} else if (vec[0] == "exit") {
			exit(0);
		} else {
			cout << "what?!" << endl;
		}
	}
}

int main(int argc, char* argv[]) {
	startShellCommands();
	return 0;
}
