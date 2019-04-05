#include "OutputNet.h"
#include "Cell.h"

void outputNet::CalcDrvReq(const required (&rcvreq)[2][2], Cell* rcv,
		pin rcvpin) {
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 2; j++) {
			driver.first->PinData[driver.second].tmp_req[i][j] = rcvreq[i][j];
		}
	}
	driver.first->PinData[driver.second].updateWC();
}
