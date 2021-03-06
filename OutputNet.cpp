#include "OutputNet.h"
#include "Cell.h"

void outputNet::CalcDrvReq(const required (&rcvreq)[2][2], Cell* rcv,
		pin rcvpin) {
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 2; j++) {
			driver.first->PinData[driver.second].tmp_req[i][j] = rcvreq[i][j];
		}
	}
	cout<<"updating WC MARG for "<<this->driver.first->name<<"%"<<this->driver.second<<endl;
	driver.first->PinData[driver.second].CalcTmpMarg();
	driver.first->PinData[driver.second].updateWC();
}
