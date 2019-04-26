/*
 * Pin.cpp
 *
 *  Created on: 16 Apr 2019
 *      Author: saeed
 */
#include "Pin.h"
#include "InputNet.h"
string tagtostring(Timetag_ tag) {
	switch (tag) {
	case AF:
		return "AF";
		break;
	case BF:
		return "BF";
		break;
	case BR:
		return "BR";
		break;
	default:
		return "AR";
	}
}
template<class T>
void align(T (&source)[2][2], inputNet* ref) {
	for (auto& i : { MAX, MIN }) {
		for (auto& j : { FALL, RISE }) {
			switch (source[i][j].tag) {
			case AF:
				source[i][j].val = source[i][j].val
						+ ref->high;
				break;
			case BF:
				source[i][j].val = ref->high
						- source[i][j].val;
				break;
			case BR:
				source[i][j].val = -source[i][j].val;
				break;
			default:
				break;
			}
			source[i][j].val+=ref->Ariv.tmp_vld[i][RISE].val;
			source[i][j].val=fmod(source[i][j].val,((inputNet*) mainClk)->high
					+ ((inputNet*) mainClk)->low);
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
required PinDat::getUnalWCReq(MAXMIN M, Tr tr) {
	return unalign(WC_REQ[M][tr]);
}
valid PinDat::getUnalWCVld(MAXMIN M, Tr tr) {
	return unalign(WC_VLD[M][tr]);
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
void PinDat::align_req(inputNet* ref) {
	align(tmp_req,ref);
}
void PinDat::align_vld(inputNet* ref) {
	align(tmp_vld,ref);
}
void PinDat::Print(MAXMIN M, ofstream& f) {
	f << to_string(WC_MARG[M][FALL])
			<<",	"
			<<getUnalWCVld(M,FALL).val
			<<tagtostring(WC_VLD[M][FALL].tag)
			<<",	"
			<<getUnalWCReq(M,FALL).val
			<<tagtostring(WC_REQ[M][FALL].tag)
			<<",	"
			<< to_string(WC_MARG[M][RISE])
			<<",	"
			<<getUnalWCVld(M,RISE).val
			<<tagtostring(WC_VLD[M][RISE].tag)
			<<",	"
			<<getUnalWCReq(M,RISE).val
			<<tagtostring(WC_REQ[M][RISE].tag);


}
