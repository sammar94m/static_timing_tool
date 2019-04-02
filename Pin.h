#ifndef PIN_H_
#define PIN_H_

#include "enums.h"

class PinDat {

public:
	/*[MODE][FALL/RISE]*/
	margin WC_MARG[2][2] = { INT_MAX };
	required WC_REQ[2][2];
	valid WC_VLD[2][2];
	Tr WC_TR[2][2];
	slope WC_slope[2][2] = { INT_MIN };
	/*Used for analysis*/
	margin tmp[2][2];
	required tmp_req[2][2];
	valid tmp_vld[2][2];
	Tr tmp_TR[2][2];
	slope tmp_slope[2][2];
	PinDat() {
		for (const auto i : { MAX, MIN }) {
			for (const auto j : { FALL, RISE }) {
				WC_REQ[i][j].val = (i == MAX) ? INT_MAX : INT_MIN;
				WC_VLD[i][j].val = (i == MAX) ? INT_MIN : INT_MAX;
			}
		}

	}
	void updateWC() {

	}
	;
};

#endif
