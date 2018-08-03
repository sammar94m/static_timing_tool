#ifndef NET_H_
#define NET_H_


#include "enums.h"
//#include "Cell.h"
#include "Receiver.h"


class Cell;

class Net {
private:
	string name;
	netType type;
	bool isClk;
	int low; //clk param
	int high; //clk param
	pair<Cell*, pin> driver; //cell* is the driver , string is the pin
	list<receiver*> receivers; //
public:
	//--------------
	Net(string name, netType type, bool isClk, int low, int high) :
			name(name), type(type), isClk(isClk), low(low), high(high) {
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
};





#endif

