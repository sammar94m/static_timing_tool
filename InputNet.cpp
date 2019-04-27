/*
 * InputNet.cpp
 *
 *  Created on: Sep 22, 2018
 *      Author: compm
 */

#include "InputNet.h"
void inputNet::CalcDrvReq(const required (&rcvreq)[2][2], Cell* rcv,
		pin rcvpin) {
	if (rcv == NULL) {
		cout << "INPUTNET::CALCDRVREQ : error rcv = NULL" << endl;
	} else {
		delay del = getRcvDelay(rcv, rcvpin);
		delay tmp[2];
		cout << "INPUTNET::"<<name<< " updating req" << endl;
		for (int j = 0; j < 2; j++) {
			tmp[MAX] = Ariv.tmp_req[MAX][j].val;
			tmp[MIN] = Ariv.tmp_req[MIN][j].val;

			Ariv.tmp_req[MAX][j].val = min(tmp[MAX], rcvreq[MAX][j].val - del);
			Ariv.tmp_req[MIN][j].val = max(tmp[MIN], rcvreq[MIN][j].val - del);
		}
		for(auto& i : {MIN, MAX}){
			for(auto& j : {FALL, RISE}){
				cout<<Ariv.tmp_req[i][j].val<<",";
			}
			cout<<endl;
		}
		Ariv.CalcTmpMarg();
		Ariv.updateWC();
	}

}
