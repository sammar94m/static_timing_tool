#ifndef PIN_H_
#define PIN_H_

#include "enums.h"
template<class T>
void align(T (&source)[2][2]);
template<class T>
T unalign(T source);

class inputNet;
string tagtostring(Timetag_ tag);
class PinDat {

public:
	/*[MODE][FALL/RISE]*/
	margin WC_MARG[2][2] ;
	required WC_REQ[2][2];
	valid WC_VLD[2][2];
	/*Used for analysis*/
	margin tmp_marg[2][2];
	required tmp_req[2][2];
	valid tmp_vld[2][2];
	Tr tmp_TR[2][2];
	slope tmp_slope[2][2];
	PinDat() {
		for (const auto i : { MAX, MIN }) {
			for (const auto j : { FALL, RISE }) {
				WC_MARG[i][j]=INT_MAX;
				tmp_marg[i][j]=INT_MAX;
				WC_REQ[i][j].val = (i == MAX) ? INT_MAX : INT_MIN;
				WC_VLD[i][j].val = (i == MAX) ? INT_MIN : INT_MAX;
				tmp_req[i][j].val = (i == MAX) ? INT_MAX : INT_MIN;
				tmp_vld[i][j].val = (i == MAX) ? INT_MIN : INT_MAX;
				tmp_slope[i][j]= (i == MAX) ? INT_MIN : INT_MAX;
			}
		}
	}
	void updateWC() {
		for (const auto i : { MAX, MIN }) {
			for (const auto j : { FALL, RISE }) {
				if (tmp_marg[i][j] < WC_MARG[i][j]) {
					WC_REQ[i][j] = tmp_req[i][j];
					WC_VLD[i][j] = tmp_vld[i][j];
					WC_MARG[i][j]= tmp_marg[i][j];
				}
			}
		}
	}

	void Print() {
		std::cout << "wc_marg=" << '{' << to_string(WC_MARG[0][0]) << ','
				<< to_string(WC_MARG[0][1]) << ',' << to_string(WC_MARG[1][0])
				<< ',' << to_string(WC_MARG[1][1]) << '}' << endl;
		std::cout << "wc_req=" << '{' << to_string(WC_REQ[0][0].val) << ','
				<< to_string(WC_REQ[0][1].val) << ','
				<< to_string(WC_REQ[1][0].val) << ','
				<< to_string(WC_REQ[1][1].val) << '}' << endl;
		std::cout << "wc_vld=" << '{' << to_string(WC_VLD[0][0].val) << ','
				<< to_string(WC_VLD[0][1].val) << ','
				<< to_string(WC_VLD[1][0].val) << ','
				<< to_string(WC_VLD[1][1].val) << '}' << endl;
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
	void Print(MAXMIN M, ofstream& f);
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
	required getUnalReq(MAXMIN M, Tr tr);
	valid getUnalVld(MAXMIN M, Tr tr);
	required getUnalWCReq(MAXMIN M, Tr tr);
	valid getUnalWCVld(MAXMIN M, Tr tr);
	void align_req(inputNet*);
	void align_vld(inputNet*);

	void CalcTmpMarg();
};

#endif
