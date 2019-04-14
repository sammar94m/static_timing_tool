#include "Cell.h"
#include "Net.h"
#include "path.h"
string Trtostring(Tr tr) {
	switch (tr) {
	case FALL:
		return "FALL";
		break;
	default:
		return "RISE";
		break;

	}
}
void C_NODE::print(MAXMIN M) {
	cout << "Cell:	" << _cell->name << "	-	" << _delay << "	-" << endl;
}

void N_NODE::print(MAXMIN M) {
	if (_net->type == INPUT) {
		cout << "Net:	" << _net->name << "	" << _inslope << "	" << _delay
				<< "	-" << endl;
	} else {
		cout << "Net:	" << _net->name << "	-	" << _delay << "	-" << endl;
	}
}
void P_NODE::print(MAXMIN M) {
	cout << "Pin:	" << _pin << "	" << T_DAT.tmp_slope[M][_state] << "	-	"
			<< Trtostring(_state) << endl;
}
branchslack::branchslack(_PATH* PA, path_vec::iterator& it, margin diffslack,
		margin slack, list<receiver*>::iterator& p, Tr state) :
		_PA(PA), _it(it), _slackofpath(slack), _diffslack(diffslack), _prcv(p), _state(
				state) {
	cout << "net " << ((N_NODE*) (*it))->_net->name << ": found branch to "
			<< (*p)->cell->name << " with diff " << diffslack << endl;
}

template<>
margin PriorityQ<branchslack>::GetMIN() {
	if (PQ.empty()) {
		return INT_MAX;
	} else {
		auto& up = PQ.top();
		return up.GetMarg();

	}
}
template<>
margin PriorityQ<_PATH*>::GetMIN() {
	if (PQ.empty()) {
		return INT_MAX;
	} else {
		auto& up = PQ.top();
		return up->GetMarg();

	}
}
template<>
margin PriorityQ<branchslack>::GetMAX() {
	if (PQ.empty()) {
		return INT_MIN;
	} else {
		margin MAX = INT_MIN;
		auto begin = PQ.begin();
		auto end = PQ.end();
		for (auto i = begin; i != end; i++) {
			MAX = max(MAX, (*i).GetMarg());
		}
		return MAX;
	}
}
template<>
margin PriorityQ<_PATH*>::GetMAX() {
	if (PQ.empty()) {
		return INT_MIN;
	} else {
		margin MAX = INT_MIN;
		auto begin = PQ.begin();
		auto end = PQ.end();
		for (auto i = begin; i != end; i++) {
			MAX = max(MAX, (*i)->GetMarg());
		}
		return MAX;
	}
}
template<>
void PriorityQ<branchslack>::Print(MAXMIN M) const {
	auto begin = PQ.begin();
	auto end = PQ.end();
	for (auto i = begin; i != end; i++) {
		(*i).print(M);
	}
}
template<>
void PriorityQ<_PATH*>::Print(MAXMIN M) const {
	auto begin = PQ.begin();
	auto end = PQ.end();
	for (auto i = begin; i != end; i++) {
		(*i)->print(M);
	}
}

