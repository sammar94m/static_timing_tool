// VLSI_PREJECT.cpp : Defines the entry point for the console application.
//

using namespace std;

#include "Createnetlist.h"

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

void runShellCommands() {
	string shellCommand;
	while (shellCommand != "done") {
		cin >> shellCommand;
		if (shellCommand == "algo1") {
			//run alogorithm1
			cout << "running algorithm1" << endl;
		} else if (shellCommand == "algo2") {
			//run alogorithm2
			cout << "running algorithm2" << endl;
		} else if (shellCommand == "list-cells") {
			cout << "..." << endl;
			//CellsTable
		} else if (shellCommand == "list-nets") {
			cout << "..." << endl;
			//NetsTable
		} else if (shellCommand == "list-input") {
			cout << "..." << endl;
			//InputTable;
			// InputDataTable;
			// InputClkTable;
		} else if (shellCommand == "list-output") {
			cout << "..." << endl;

		} else if (shellCommand == "print-receivers") {
			cout << "..." << endl;

		} else if (shellCommand == "print-circuit") {
			cout << "..." << endl;

		} else if (shellCommand == "connect") {
			cout << "..." << endl;

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
	runShellCommands();
	return 0;
}
