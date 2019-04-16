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
#include <boost/heap/priority_queue.hpp>

string Trtostring(Tr tr);
class _NODE {
public:
	_NODE() = default;
	virtual void print(MAXMIN M)=0;
	virtual ~_NODE() = default;
};
class C_NODE: public _NODE {
public:
	Cell* _cell;
	delay _delay;
	C_NODE(Cell* pCell,delay delay) :
			_cell(pCell),_delay(delay) {

	}
	void print(MAXMIN M);
};
class P_NODE: public _NODE {
public:
	pin _pin;
	PinDat T_DAT;
	Tr _state;
	P_NODE(pin p, PinDat& d,Tr state ) :
			_pin(p), T_DAT(d),_state(state) {

	}
	void print(MAXMIN M);
};
class N_NODE: public _NODE {
public:
	Net* _net;
	delay _delay;
	slope _inslope;
	N_NODE(Net* n,delay del,slope slp=0) :
			_net(n),_delay(del),_inslope(slp){

	}
	void print(MAXMIN M);
};
typedef vector<_NODE*> path_vec;

class _PATH {
public:
	int id;
	margin marg;
	path_vec vec;
	_PATH(margin _marg) :
		id(0),marg(_marg) {

	}
	bool operator <(const _PATH& rhs) const {
		return this->marg < rhs.marg;
	}
	virtual void print()=0;
	void print(MAXMIN M) {
		cout << "Path id: " << id << endl;
		cout << "MARGIN: " << marg << endl;
		cout << "OBJ	"
		<<"NAME	"
		<<"SLOPE	"
		<<"DELAY	"
		<<"STATE	"
		<<"WC_VLD	"
		<<"WC_REQ	"<<endl;
		for (auto i : vec) {
			i->print(M);
		}
	}
	virtual ~_PATH()=default;
	margin GetMarg(){
		return this->marg;
	}
};
class MAX_PATH: public _PATH {
public:
	static int counter;
	MAX_PATH(margin _marg) :
			_PATH(_marg) {
		id = ++counter;
	}
	void print(){
		_PATH::print(MAX);
	}
};

class MIN_PATH: public _PATH {
public:
	static int counter;
	MIN_PATH(margin _marg) :
			_PATH(_marg) {
		id = ++counter;
	}
	void print(){
		_PATH::print(MIN);
	}
};
struct PATHCompare
{
    bool operator()(const _PATH* e1, const _PATH* e2) const
    {
        return e1->marg > e2->marg;
    }
};
class branchslack {
public:
	_PATH* _PA;
	_NODE* _pN;
	margin _slackofpath;
	margin _diffslack;
	list<receiver*>::iterator _prcv;
	Tr _state;
	branchslack(_PATH* PA,_NODE* pN, margin diffslack, margin slack,
			list<receiver*>::iterator& p,Tr state);

	bool operator <(const branchslack& rhs) const {
		return this->_slackofpath - this->_diffslack
				< rhs._slackofpath - rhs._diffslack; // max heap TODO: CHECK IF MAX HEAP
	}
	margin GetMarg() const{
		return this->_slackofpath - this->_diffslack;
	}
	void print(MAXMIN M);

};
struct BRANCHCompare
{
    bool operator()(const branchslack &e1, const branchslack &e2) const
    {
        return e1.GetMarg() > e2.GetMarg();
    }
};
template<class T, typename C>
class PriorityQ {
public:
	unsigned int MAX_SIZE;
	boost::heap::priority_queue<T,boost::heap::compare<C>> PQ;
	PriorityQ(unsigned int _MAX_SIZE) :
			MAX_SIZE(_MAX_SIZE) {
	}
	void Add(T& obj) {
		if (PQ.size() == MAX_SIZE) {
			PQ.push(obj);
			PQ.pop();
		} else {
			PQ.push(obj);
		}
	}
	margin GetMIN();
	margin GetMAX();
	int getSize() {
		return PQ.size();
	}
	bool isFull(){
		return PQ.size()==MAX_SIZE;
	}
	void Print(MAXMIN M) const;
};

#endif /* PATH_H_ */
