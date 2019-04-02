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

queue<Net*> InputTable;
queue<Net*> InputDataTable;
queue<Net*> InputClkTable;

map<string, Net*> NetsTable;
map<string, Cell*> CellsTable;
map<string, CellTemplate*> CellTemplateTable;

/* files:
 *  *		LibraryFile.txt
 *		NetlistFileFormat.txt
 *		DesignConstraintsFile.txt

 *		ParasiticsInterconnectFile.txt
 * */
//void printCircuit() {
//	for (auto it = CellsTable.begin(); it != CellsTable.end(); ++it) {
//		cout << it->first << " (" << it->second->Template->template_name << ")"
//				<< endl;
//		for (auto pinItr = it->second->inMap.begin();
//				pinItr != it->second->inMap.end(); ++pinItr) {
//			cout << "IN " << pinItr->first << "(" << pinItr->second->name << ")"
//					<< endl;
//		}
//		for (auto pinItr = it->second->outMap.begin();
//				pinItr != it->second->outMap.end(); ++pinItr) {
//			cout << "OUT " << pinItr->first << "(" << pinItr->second->name
//					<< ")" << endl;
//		}
//		cout << endl;
//	}
//}
void list_command(vector<string>& vec) {
	if (vec[1] == "cells") {
		cout << "..." << endl;
		for (auto it = CellsTable.begin(); it != CellsTable.end(); ++it) {
			cout << "name: " << it->first << endl;
		}
	} else if (vec[1] == "nets") {
		cout << "..." << endl;
		cout << "Name " << "Type " << "isClock" << endl;
		for (auto it = NetsTable.begin(); it != NetsTable.end(); ++it) {
			cout << it->first << " ";
			switch (it->second->type) {
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
			if (it->second->isClk) {
				cout << " CLK";
			} else {
				cout << " DATA";
			}
			cout << endl;

		}
	} else if (vec[1] == "input") {
		cout << "..." << endl;
		//InputTable;
		// InputDataTable;
		// InputClkTable;
	} else if (vec[1] == "output") {
		cout << "..." << endl;

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
			cout << it->second->name << ':' << endl;
			for (auto rcvit = it->second->ClkArtime.begin();
					rcvit != it->second->ClkArtime.end(); ++rcvit) {
				cout << "	" << rcvit->first->cell->name << "	" << "	"
						<< rcvit->second.RISE_AR << "	" << rcvit->second.high
						<< "	" << rcvit->second.low << "	" << endl;
				;
			}
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
//		printCircuit();
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

void startShellCommands() {
	string input;
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

	cout << " reading LibraryFile" << endl;
	LibraryFile("LibraryFile.txt");
	cout << " LibraryFile done" << endl;
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
	cout << "  DesignConstraintsFile done" << endl;
	cout
			<< "-----------------------------------------------------------------------"
			<< endl;

//		for (auto it = NetsTable.begin(); it != NetsTable.end();++it) {
//			cout << it->first << " : " << endl;
//			it->second->print();
//		}

	cout << " reading NetlistFileFormat" << endl;
	NetlistFileFormat("NetlistFileFormat.txt");
	cout << " NetlistFileFormat done " << endl;

//dfsPrint();
	startShellCommands();
	return 0;
}
