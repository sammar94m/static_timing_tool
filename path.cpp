
#include "Cell.h"
#include "Net.h"
#include "path.h"

void C_NODE::print() {
	cout << "Cell:	" << _cell->name << endl;
}


void N_NODE::print() {
		cout << "Net:	" << _net->name << endl;
	}
