#ifndef NET_H_
#define NET_H_
#include "cellfwd.h"
#include "enums.h"
#include "Receiver.h"
#include "Clockdat.h"
#include "Pin.h"
#include "path.h"

class Net {
public:
	string name;
	netType type;
	bool isClk;
	pair<Cell*, pin> driver; //cell* is the driver , string is the pin
	list<receiver*> receivers; //
	//--------------
	Net(string _name, netType _type, bool _isClk);

	Net(string name, bool isClk);

	virtual ~Net();
	delay getRcvDelay(Cell* rcv, pin rcvpin);
	slope getRcvSlope(slope drvSlope, Cell* rcv, pin rcvpin);

	void set_driver(Cell* cell, pin pin_t);
	void add_receiver(Cell* cell, pin pin_t);
	virtual PinDat getDrvData(); // return driver PinDat
	virtual void CalcDrvReq(const required (&req)[2][2], Cell* rcv, pin rcvpin);
	/*
	 * calculate Netdelay and slope and update reciever pin
	 * update WC
	 */
	void calcRcvData(receiver* pRcv, const PinDat& Data, pin inPin);
	//virtual void print();
	list<receiver*>::iterator getCritReciever(MAXMIN MODE);
	void RecordBS(_PATH* pPA,path_vec::iterator PA,list<receiver*>::iterator ref,margin refmarg,PriorityQ<branchslack>& BS,MAXMIN MODE,Tr state);
};

#endif

