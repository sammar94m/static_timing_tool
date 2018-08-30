/*
 * Net.cpp
 *
 *  Created on: Aug 30, 2018
 *      Author: Sammar
 */
#include "Net.h"
#include "Cell.h"


delay Net::getRcvDelay(Cell* rcv, pin rcvpin){
	for(auto RcvIT=this->receivers.begin();RcvIT!=this->receivers.end();RcvIT++){
		if((*RcvIT)->cell==rcv && (*RcvIT)->inPin==rcvpin){
			return (*RcvIT)->netDely;
		}
	}
	return -1;
}
slope Net::getRcvSlope(slope drvSlope, Cell* rcv, pin rcvpin){
	for(auto RcvIT=this->receivers.begin();RcvIT!=this->receivers.end();RcvIT++){
		if((*RcvIT)->cell==rcv && (*RcvIT)->inPin==rcvpin){
			return sqrt((*RcvIT)->slopeDeg*(*RcvIT)->slopeDeg+drvSlope*drvSlope);
		}
	}
	return -1;
}
void Net::set_driver(Cell* cell, pin pin_t) {
	driver = pair<Cell*, pin>(cell, pin_t);
}
void Net::add_receiver(Cell* cell, pin pin_t) {
	receivers.push_front(new receiver(cell, pin_t));
}
void Net::set_clkdat(clockdat& clk) {
	//ClkArtime=clk;
}

//virtual void Net::print() {
//	cout << "name=" << name << " " << "type=" << type << " " << "isClk="
//			<< isClk << endl;
//	cout << "driver = { " << driver.first->name << " , " << driver.second
//			<< endl;
//	cout << "receivers list:" << endl;
//	for (auto iter = receivers.begin(); iter != receivers.end(); ++iter)
//		iter.operator *()->print();
//
////		map<receiver*,clockdat> ClkArtime; //refrence clock rise
//}

