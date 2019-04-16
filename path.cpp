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
string tagtostring(Timetag_ tag) {
	switch (tag) {
	case AF:
		return "AF";
		break;
	case BF:
		return "BF";
		break;
	case BR:
		return "BR";
		break;
	default:
		return "AR";
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
	valid unalvld=T_DAT.getUnalVld(M,_state);
	required unalrq=T_DAT.getUnalReq(M,_state);
	cout << "Pin:	" << _pin
			<< "	" << T_DAT.tmp_slope[M][_state]
			<< "	-	"<< Trtostring(_state)
			<< "	" << unalvld.val <<tagtostring(unalvld.tag)
			<< "	" << unalrq.val <<tagtostring(unalrq.tag)
			<< "	" << endl;
}
branchslack::branchslack(_PATH* PA, _NODE* pN, margin diffslack, margin slack,
		list<receiver*>::iterator& p, Tr state) :
		_PA(PA), _pN(pN), _slackofpath(slack), _diffslack(diffslack), _prcv(p), _state(
				state) {
	cout << "net " << ((N_NODE*) (_pN))->_net->name << ": found branch to "
			<< (*p)->cell->name << " with diff " << diffslack << endl;
}
void branchslack::print(MAXMIN M) {
	cout << "printing branch slack" << endl;
	cout << "net " << ((N_NODE*) (_pN))->_net->name << " branch to "
			<< (*_prcv)->cell->name << " with diff " << _diffslack << endl;
}
template<>
margin PriorityQ<branchslack, BRANCHCompare>::GetMIN() {
	if (PQ.empty()) {
		return INT_MAX;
	} else {
		auto& up = PQ.top();
		return up.GetMarg();

	}
}
template<>
margin PriorityQ<_PATH*, PATHCompare>::GetMIN() {
	if (PQ.empty()) {
		return INT_MAX;
	} else {
		auto& up = PQ.top();
		return up->GetMarg();

	}
}
template<>
margin PriorityQ<branchslack, BRANCHCompare>::GetMAX() {
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
margin PriorityQ<_PATH*, PATHCompare>::GetMAX() {
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
void PriorityQ<branchslack, BRANCHCompare>::Print(MAXMIN M) const {
	auto begin = PQ.begin();
	auto end = PQ.end();
	for (auto i = begin; i != end; i++) {
		auto x = (*i);
		x.print(M);
	}
}
template<>
void PriorityQ<_PATH*, PATHCompare>::Print(MAXMIN M) const {
	auto begin = PQ.begin();
	auto end = PQ.end();
	for (auto i = begin; i != end; i++) {
		(*i)->print(M);
	}
}

