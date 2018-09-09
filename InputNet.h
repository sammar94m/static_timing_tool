#ifndef INPUTNET_H_
#define INPUTNET_H_

#include "enums.h"
#include "Net.h"

class inputNet: public Net {
	clockdat clk;
	static clockdat* refclk;
public:
	inputNet(string name, bool isClk,slope SL_RISE,
			slope SL_FALL,string AR_TIME,int low=0, int high=0):Net(name, INPUT, isClk) {
		vector<string> tmp;
		boost:split(AR_TIME," \n",tmp);
		int Ariv;
		switch(tmp[2]){
		case "AF":
			Ariv=atoi(tmp[1].c_str())+refclk->high;
			break;
		case "BF":
			Ariv=refclk->high-atoi(tmp[1].c_str());
			break;
		case "AR":
			Ariv=atoi(tmp[1].c_str());
			break;
		case "BR":
			Ariv=refclk->high+refclk->low-atoi(tmp[1].c_str());
			break;

		}

	}
	load getCin(pin input) {
		return 0;
	}
	virtual load getCout(output_pin out) {
		return 0;
	}
	virtual delay getDelay(input_pin in, output_pin out, MAXMIN AnlsType,
			Transitions Tr, slope inslope, load outload) {
		if (Tr == RR || Tr == FR) {
			return 0;
		} else {
			return 0;
		}
	}
	virtual slope getSlope(input_pin in, output_pin out, MAXMIN AnlsType,
			Transitions Tr, slope inslope, load outload) {
		if (Tr == RR || Tr == FR) {
			return 0;
		} else {
			return 0;
		}
	}

};

#endif

