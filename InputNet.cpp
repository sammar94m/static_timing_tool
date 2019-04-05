/*
 * InputNet.cpp
 *
 *  Created on: Sep 22, 2018
 *      Author: compm
 */

#include "InputNet.h"
void inputNet::CalcDrvReq(const required (&rcvreq)[2][2], Cell* rcv, pin rcvpin) {
	if (rcv == NULL) {
		cout << "INPUTNET::CALCDRVREQ : error rcv = NULL" << endl;
	}else{
		delay del =getRcvDelay(rcv,rcvpin);
		for(int i=0;i<2;i++){
			for(int j=0; j<2; j++){
				Ariv.tmp_req[i][j]=rcvreq[i][j];
				Ariv.tmp_req[i][j].val-=del;
			}
		}
		Ariv.updateWC();
	}

}
