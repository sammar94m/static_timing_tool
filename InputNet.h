#ifndef INPUTNET_H_
#define INPUTNET_H_

#include "enums.h"
#include "Net.h"

using namespace boost;

class inputNet: public Net {

public:
	clockdat clk;
	static clockdat* refclk;
	inputNet(string name, bool isClk, slope SL_RISE, slope SL_FALL,
			string AR_TIME) :
			Net(name, INPUT, isClk) {
		vector<string> tmp;
		boost::split(tmp, AR_TIME, boost::is_any_of(" \n\r"));
		if (tmp.empty())
			cout << "empty vec" << endl;

		cout << "tmp vec:" << tmp[0] << "  " << tmp[1] << endl;
		clk.RISE_SLOPE = SL_RISE;
		clk.FALL_SLOPE = SL_FALL;
		int Ariv = 0;
		if (refclk) {
			if (tmp[1] == "AF") {
				Ariv = atoi(tmp[0].c_str()) + this->refclk->high;
			} else if (tmp[1] == "BF") {
				Ariv = this->refclk->high - atoi(tmp[0].c_str());
			} else if (tmp[1] == "AR") {
				Ariv = atoi(tmp[0].c_str());
			} else if (tmp[1] == "BR") {
				Ariv = this->refclk->high + this->refclk->low
						- atoi(tmp[0].c_str());
			}
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

