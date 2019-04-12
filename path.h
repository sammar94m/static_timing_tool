/*
 * path.h
 *
 *  Created on: 8 Apr 2019
 *      Author: saeed
 */

#ifndef PATH_H_
#define PATH_H_
#include "enums.h"
#include "cellfwd.h"
#include "netfwd.h"
#include "Receiver.h"
class _NODE {
public:
	_NODE() = default;
	virtual void print()=0;
	virtual ~_NODE() = default;
};
class C_NODE: public _NODE {
public:
	Cell* _cell;
	C_NODE(Cell* pCell) :
			_cell(pCell) {

	}
	void print();
};
class P_NODE: public _NODE {
public:
	pin _pin;
	PinDat T_DAT;
	P_NODE(pin p, PinDat& d) :
			_pin(p), T_DAT(d) {

	}
	void print() {
		cout << "Pin:	" << _pin << endl;
	}
};
class N_NODE: public _NODE {
public:
	Net* _net;
	N_NODE(Net* n) :
			_net(n) {

	}
	void print();
};
typedef vector<_NODE*> path_vec;

class _PATH {
public:
	int id;
	margin marg;
	path_vec vec;
	_PATH(margin _marg) :
			marg(_marg) {

	}
	void print() {
		cout << "Path id: " << id << endl;
		cout << "MARGIN: " << marg << endl;
		for (auto i : vec) {
			i->print();
		}
	}
};
class MAX_PATH: public _PATH {
public:
	static int counter;
	MAX_PATH(margin _marg) :
			_PATH(_marg) {
		id = ++counter;
	}

};
class MIN_PATH: public _PATH {
public:
	static int counter;
	MIN_PATH(margin _marg) :
			_PATH(_marg) {
		id = ++counter;
	}
};
class branchslack {
public:
	path_vec::iterator _it;
	margin _slackofpath;
	margin _diffslack;
	list<receiver*>::iterator _prcv;
	branchslack(path_vec::iterator& it, margin diffslack, margin slack,
			list<receiver*>::iterator& p);
	bool operator <(const branchslack& rhs) const {
		return this->_slackofpath - this->_diffslack
				< rhs._slackofpath - rhs._diffslack; // max heap
	}

};

class PriorityQ {
public:
	unsigned int MAX_SIZE;
	priority_queue<branchslack> PQ;
	PriorityQ(unsigned int _MAX_SIZE) :
			MAX_SIZE(_MAX_SIZE) {
	}
	void Add(branchslack& bs) {
		if (PQ.size() == MAX_SIZE) {
			PQ.push(bs);
			PQ.pop();
		} else {
			PQ.push(bs);
		}
	}
	margin GetMAX() {
		if (PQ.empty()) {
			return INT_MIN;
		} else {
			auto& up = PQ.top();
			return up._slackofpath - up._diffslack;

		}
	}
	int getSize() {
		return PQ.size();
	}
};

#endif /* PATH_H_ */
