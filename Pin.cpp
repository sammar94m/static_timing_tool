/*
 * Pin.cpp
 *
 *  Created on: 16 Apr 2019
 *      Author: saeed
 */
#include "Pin.h"
#include "InputNet.h"

template<class T>
void align(T (&source)[2][2]) {
	for (auto& i : { MAX, MIN }) {
		for (auto& j : { FALL, RISE }) {
			switch (source[i][j].tag) {
			case AF:
				source[i][j].val = source[i][j].val
						+ ((inputNet*) mainClk)->high;
				break;
			case BF:
				source[i][j].val = ((inputNet*) mainClk)->high
						- source[i][j].val;
				break;
			case BR:
				source[i][j].val = -source[i][j].val;
				break;
			default:
				break;
			}
		}
	}
}
template<class T>
T unalign(T source) {
//from AR TO OTHERS
	switch (source.tag) {
	case AF:
		source.val = source.val - ((inputNet*) mainClk)->high;
		break;
	case BF:
		source.val = ((inputNet*) mainClk)->high - source.val;
		break;
	case BR:
		source.val = -source.val;
		break;
	default:
		break;
	}

	return source;
}
required PinDat::getUnalReq(MAXMIN M, Tr tr) {
	return unalign(tmp_req[M][tr]);
}
valid PinDat::getUnalVld(MAXMIN M, Tr tr) {
	return unalign(tmp_vld[M][tr]);
}
void PinDat::CalcTmpMarg() {
	for (auto& i : { FALL, RISE }) {
		if(tmp_req[MAX][i].val==INT_MAX){
			tmp_marg[MAX][i]=INT_MAX;
		}else{
			tmp_marg[MAX][i] = tmp_req[MAX][i].val - tmp_vld[MAX][i].val;
		}

		if(tmp_req[MIN][i].val==INT_MIN){
			tmp_marg[MIN][i]=INT_MAX;
		}else{
			tmp_marg[MIN][i] = tmp_vld[MIN][i].val - tmp_req[MIN][i].val;
		}

	}
}
void PinDat::align_req() {
	align(tmp_req);
}
void PinDat::align_vld() {
	align(tmp_vld);
}
