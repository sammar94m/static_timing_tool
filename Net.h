#ifndef NET_H_
#define NET_H_


#include "enums.h"
#include "Receiver.h"
#include "Clockdat.h"


class Cell;

class Net {
public:
	string name;
	netType type;
	bool isClk;
	pair<Cell*, pin> driver; //cell* is the driver , string is the pin
	list<receiver*> receivers; //
	map<receiver*,clockdat> ClkArtime; //refrence clock rise
	//--------------
	Net(string name, netType type, bool isClk, int low, int high) :
			name(name), type(type), isClk(isClk) {
	};

	virtual ~Net(){}

	delay getRcvDelay(Cell* rcv, pin rcvpin);
	slope getRcvSlope(slope drvSlope, Cell* rcv, pin rcvpin);

	void set_driver(Cell* cell, pin pin_t) ;
	void add_receiver(Cell* cell, pin pin_t);
	void set_clkdat(clockdat& clk);

	//virtual void print();
};





#endif

