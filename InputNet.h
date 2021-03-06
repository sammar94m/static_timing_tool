#ifndef INPUTNET_H_
#define INPUTNET_H_

#include "Net.h"
#include "Pin.h"
using namespace boost;

class inputNet: public Net {

public:
	PinDat Ariv;
	inputNet* ref;
	delay low; //clk param
	delay high; //clk param
	inputNet(string name, bool isClk, slope SL_RISE, slope SL_FALL,
			string AR_TIME, delay HIGH = 0, delay LOW = 0) :
			Net(name, INPUT, isClk) {
		low = LOW;
		high = HIGH;
		vector<string> tmp;
		boost::split(tmp, AR_TIME, boost::is_any_of(" \n\r"));
		int _Ariv = atoi(tmp[0].c_str());
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
				Ariv.tmp_slope[i][j] = (j == FALL) ? SL_FALL : SL_RISE;

				if (isClk) {
					if (j == RISE) {
						Ariv.tmp_vld[i][j].val = _Ariv;
					} else {
						Ariv.tmp_vld[i][j].val = _Ariv+high;
					}
				} else {
					Ariv.tmp_vld[i][j].val = _Ariv;
				}
				Ariv.tmp_vld[i][j].tag = edgeref;
				Ariv.tmp_TR[i][j] = (j == FALL) ? FALL : RISE;
			}
		}
		if (name != "CLK") {
			ref = (inputNet*) NetsTable[tmp[2].c_str()];
			if(ref==NULL){
			cout<<"ref "<<tmp[2]<<" not found"<<endl;
			cout<<"using CLK as ref"<<endl;
			ref=(inputNet*)mainClk;
			Ariv.align_vld(ref);
			}else{
			Ariv.align_vld(ref);
			}
		} else {
			ref = this;
		}


	}
	void CalcDrvReq(const required (&rcvreq)[2][2], Cell* rcv, pin rcvpin);
	PinDat getDrvData() {
		return Ariv;
	}

};

#endif

