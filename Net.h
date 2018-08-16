#ifndef NET_H_
#define NET_H_


#include "enums.h"
//#include "Cell.h"
#include "Receiver.h"


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
	}
	;

	delay getRcvDelay(Cell* rcv, pin rcvpin);
	slope getRcvSlope(int drvSlope, Cell* rcv, pin rcvpin);
	void set_driver(Cell* cell, pin pin_t) {
		driver = pair<Cell*, pin>(cell, pin_t);
	}
	void add_receiver(Cell* cell, pin pin_t) {
		receivers.push_front(new receiver(cell, pin_t));
	}
	void set_clkdat(clockdat& clk){
		//ClkArtime=clk;
	}
};





#endif

