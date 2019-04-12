
#include "Cell.h"
#include "Net.h"
#include "path.h"


void C_NODE::print() {
	cout << "Cell:	" << _cell->name << endl;
}


void N_NODE::print() {
		cout << "Net:	" << _net->name << endl;
	}
branchslack::branchslack(path_vec::iterator& it, margin diffslack, margin slack, list<receiver*>::iterator& p) :
		_it(it), _slackofpath(slack), _diffslack(diffslack), _prcv(p) {
	cout << "net " << ((N_NODE*)(*it))->_net->name<<": found branch to "<<(*p)->cell->name<<" with diff "<<diffslack<<endl;
}
;
