#ifndef INPUTNET_H_
#define INPUTNET_H_

#include "enums.h"
#include "Net.h"

using namespace boost;

class inputNet: public Net {

public:
	PinDat Ariv;
	int low; //clk param
	int high; //clk param

	inputNet(string name, bool isClk, slope SL_RISE, slope SL_FALL,
			string AR_TIME, int HIGH = 0, int LOW = 0) :
			Net(name, INPUT, isClk) {
		low = LOW;
		high = HIGH;
		vector<string> tmp;
		boost::split(tmp, AR_TIME, boost::is_any_of(" \n\r"));
		if (tmp.empty())
			cout << "empty vec" << endl;
		cout << "tmp vec:" << tmp[0] << "  " << tmp[1] << endl;
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
				Ariv.tmp_vld[i][j].val = _Ariv;
				Ariv.tmp_vld[i][j].tag = edgeref;
				Ariv.tmp_TR[i][j] = (j == FALL) ? FALL : RISE;
			}
		}
		Ariv.updateWC(); //TODO: WRITE WC

	}
	PinDat getDrvData() {
		return Ariv;
	}

};

#endif

