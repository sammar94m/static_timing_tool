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
	margin tmp_marg[2][2];
	required tmp_req[2][2];
	valid tmp_vld[2][2];
	Tr tmp_TR[2][2];
	slope tmp_slope[2][2];
	PinDat() {
		for (const auto i : { MAX, MIN }) {
			for (const auto j : { FALL, RISE }) {
				WC_REQ[i][j].val = (i == MAX) ? INT_MAX : INT_MIN;
				WC_VLD[i][j].val = (i == MAX) ? INT_MIN : INT_MAX;
				tmp_req[i][j].val = (i == MAX) ? INT_MAX : INT_MIN;
				tmp_vld[i][j].val = (i == MAX) ? INT_MIN : INT_MAX;
			}
		}
	}
	void updateWC() {
	}
	;
	void Print() {
		std::cout << "tmp_marg=" << '{' << to_string(tmp_marg[0][0]) << ','
				<< to_string(tmp_marg[0][1]) << ',' << to_string(tmp_marg[1][0])
				<< ',' << to_string(tmp_marg[1][1]) << '}' << endl;
		std::cout << "tmp_req=" << '{' << to_string(tmp_req[0][0].val) << ','
				<< to_string(tmp_req[0][1].val) << ','
				<< to_string(tmp_req[1][0].val) << ','
				<< to_string(tmp_req[1][1].val) << '}' << endl;
		std::cout << "tmp_vld=" << '{' << to_string(tmp_vld[0][0].val) << ','
				<< to_string(tmp_vld[0][1].val) << ','
				<< to_string(tmp_vld[1][0].val) << ','
				<< to_string(tmp_vld[1][1].val) << '}' << endl;

	}
	void resetReq() {
		for (const auto i : { MAX, MIN }) {
			for (const auto j : { FALL, RISE }) {
				tmp_req[i][j].val = (i == MAX) ? INT_MAX : INT_MIN;
			}
		}
	}
	margin GetWCTmpMarg(MAXMIN M) {
		return min(tmp_marg[M][FALL], tmp_marg[M][RISE]);
	}
	Tr GETWCTrTmp(MAXMIN M) {
		return tmp_marg[M][FALL] > tmp_marg[M][RISE] ? RISE : FALL;
	}
	void CalcTmpMarg() {
		for (auto& i : { FALL, RISE }) {
			tmp_marg[MAX][i] = tmp_req[MAX][i].val - tmp_vld[MAX][i].val;
			tmp_marg[MIN][i] = tmp_vld[MIN][i].val - tmp_req[MIN][i].val;

		}
	}
};

#endif
