/*
 * CellTemplate.cpp
 *
 *  Created on: Sep 7, 2018
 *      Author: Sammar
 */
#include "CellTemplate.h"

delay CellTemplate::getDelay(input_pin in, output_pin out, MAXMIN AnlsType,
		Transitions Tr, slope inslope, load outload) {
	int indslp, indld;
	bool exactslp, exactld;
	indslp = getClosestindex(IN_SLOPE_POINTS, inslope, exactslp);
	indld = getClosestindex(OUT_LOAD_POINTS, outload, exactld);
	if (withinboundry(indslp, exactslp) && withinboundry(indld, exactld)) { //WITHIN TABLE BOUNDRIES
		return delayTable[pair<input_pin, output_pin>(in, out)].GetTableAV(
				AnlsType, Tr, indslp, indld, exactld, exactslp);
	} else { //OUTSIDE TABLE BOUNDRIES
		return delayTable[pair<input_pin, output_pin>(in, out)].GetExtTableVal(
				AnlsType, Tr, inslope, outload, indslp, indld, exactld,
				exactslp);
	}
	return -1;
}
delay CellTemplate::getSlope(input_pin in, output_pin out, MAXMIN AnlsType,
		Transitions Tr, slope inslope, load outload) {
	int indslp, indld;
	bool exactslp, exactld;
	indslp = getClosestindex(IN_SLOPE_POINTS, inslope, exactslp);
	indld = getClosestindex(OUT_LOAD_POINTS, outload, exactld);
	if (withinboundry(indslp, exactslp) && withinboundry(indld, exactld)) { //WITHIN TABLE BOUNDRIES
		return slopeTable[pair<input_pin, output_pin>(in, out)].GetTableAV(
				AnlsType, Tr, indslp, indld, exactld, exactslp);
	} else { //OUTSIDE TABLE BOUNDRIES
		return slopeTable[pair<input_pin, output_pin>(in, out)].GetExtTableVal(
				AnlsType, Tr, inslope, outload, indslp, indld, exactld,
				exactslp);

	}
	return -1;
}
bool CellTemplate::withinboundry(int ida, bool exacta) {
	if (ida == 0 && exacta == false) {
		return false;
	} else if (ida == -1) {
		return false;
	} else {
		return true;
	}
}
template<typename T>
int CellTemplate::getClosestindex(vector<T>& vec, int val, bool& exact) {
	for (int i = 0; i < vec.size(); i++) {
		if (vec[i] == val) {
			exact = true;
			return i;
		} else if (vec[i] > val) {
			exact = false;
			return i;
		}
	}
	exact = false;
	return -1;
}

void CellTemplate::print() {
	cout << "name: " << template_name << endl;
	cout << "setupdata: " << setupdata[0] << " " << setupdata[1] << " "
			<< setupdata[2] << " " << setupdata[3] << endl;
	cout << "IN_SLOPE_POINTS: ";
	for (int i = 0; i < (int) IN_SLOPE_POINTS.size(); i++) {
		cout << IN_SLOPE_POINTS[i] << " ";
	}
	cout << endl;
	cout << "OUT_LOAD_POINTS: ";
	for (int i = 0; i < (int) OUT_LOAD_POINTS.size(); i++) {
		cout << OUT_LOAD_POINTS[i] << " ";
	}
	cout << endl;
	cout << "pin->Load Map: " << endl;
	for (auto it = temp_pinLoadMap.begin(); it != temp_pinLoadMap.end(); ++it) {
		cout << it->first << " -> " << it->second << endl;
	}
	cout << "delay table" << endl << "(input_pin , output_pin)-> delay "
			<< endl;
	for (auto it = delayTable.begin(); it != delayTable.end(); ++it) {
		cout << it->first.first << " , " << it->first.second << " : " << endl;
		it->second.print();
	}

	cout << "slop table" << endl << "(input_pin , output_pin) -> slope "
			<< endl;
	for (auto it = slopeTable.begin(); it != slopeTable.end(); ++it) {
		cout << it->first.first << " , " << it->first.second << " : " << endl;
		it->second.print();
	}

}
