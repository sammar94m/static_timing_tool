#ifndef OUTPUTNET_H_
#define OUTPUTNET_H_
#include "enums.h"
#include "Pin.h"
#include "Net.h"
#include "Receiver.h"

class outputNet: public Net {
public:
	PinDat Req;
	load outload;

	outputNet(string name, bool isClk, string REQ_TIME, load outload) :
			Net(name, OUTPUT, isClk), outload(outload) {
		vector<string> tmp;
		boost::split(tmp, REQ_TIME, boost::is_any_of(" \n\r"));
		int _Req = atoi(tmp[0].c_str());
		timetag edgeref;
		if (tmp[1] == "AF") {
			edgeref = AF;
		} else if (tmp[1] == "BF") {
			edgeref = BF;
		} else if (tmp[1] == "AR") {
			edgeref = AR;
		} else if (tmp[1] == "BR") {
			edgeref = BR;
		}
		for (const auto i : { MAX, MIN }) {
			for (const auto j : { FALL, RISE }) {
				Req.tmp_req[i][j].val = _Req;
				Req.tmp_req[i][j].tag = edgeref;
				Req.tmp_TR[i][j] = (j == FALL) ? FALL : RISE;
			}
		}
		Req.updateWC();
	}
	;
	void calcRcvData(receiver* pRcv, const PinDat& Data);
	void CalcDrvReq(const required (&rcvreq)[2][2], Cell* rcv, pin rcvpin);
};

#endif
