/*
 * FlipFlop.cpp
 *
 *  Created on: Aug 30, 2018
 *      Author: Sammar
 */

#include "Flipflop.h"
#include "Net.h"
#include "InputNet.h"

pin FlipFlop::getClkPin() {
	for (auto& i : inMap) {
		if (i.second->isClk) {
			return i.first;
		}
	}
	return "";
}

bool FlipFlop::isReady() {
	//TODO: OVERRIDE FOR FLIPFLOP
	return ready_inputs == 1;
}

pin FlipFlop::getDatPin() {
	for (auto& i : inMap) {
		if (!i.second->isClk) {
			return i.first;
		}
	}
	return "";
}

void FlipFlop::initDataReq() {
	pin clk = getClkPin();
	pin dat = getDatPin();
	delay cycletime=((inputNet*)mainClk)->high+((inputNet*)mainClk)->low;
	if (clk != "" && dat != "") {
		PinDat& t_clk=this->PinData[clk];
		PinDat& t_dat=this->PinData[dat];
		//MAX
		for(auto& i: {FALL, RISE}){
			t_dat.tmp_req[MAX][i].val=t_clk.tmp_vld[MAX][RISE].val+cycletime-this->Template->setupdata[MAX][i];
			t_dat.tmp_req[MAX][i].tag=t_clk.tmp_vld[MAX][RISE].tag;
		}
		//MIN
		for(auto& i: {FALL, RISE}){
			t_dat.tmp_req[MIN][i].val=t_clk.tmp_vld[MIN][RISE].val+this->Template->setupdata[MIN][i];
			t_dat.tmp_req[MIN][i].val=t_clk.tmp_vld[MIN][RISE].tag;
		}
		t_dat.CalcTmpMarg();
		t_dat.updateWC();
	}
}
