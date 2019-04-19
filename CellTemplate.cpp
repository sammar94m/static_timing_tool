/*
 * CellTemplate.cpp
 *
 *  Created on: Sep 7, 2018
 *      Author: Sammar
 */
#include "CellTemplate.h"
#include "Table.h"

CellTemplate::CellTemplate(string name) :
		template_name(name) {
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 2; j++) {
			setupdata[i][j] = -1;
		}

	}
}
CellTemplate::~CellTemplate() {
}

delay CellTemplate::getDelay(input_pin in, output_pin out, MAXMIN AnlsType,
		InOutTr Tr, slope inslope, load outload) {
	delay res=this->delayTable[pair<input_pin, output_pin>(in,out)].GetTableVal(AnlsType,Tr,inslope,outload);
	return res;
}

slope CellTemplate::getSlope(input_pin in, output_pin out, MAXMIN AnlsType,
		InOutTr Tr, slope inslope, load outload) {
	slope res=this->slopeTable[pair<input_pin, output_pin>(in,out)].GetTableVal(AnlsType,Tr,inslope,outload);
	return res;
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

bool CellTemplate::TableExists(input_pin in, output_pin out, MAXMIN AnlsType, InOutTr Tr) {
	for(auto& it : delayTable){
		if(it.first.first==in && it.first.second==out){
			return it.second.TableExists(AnlsType,Tr);
		}
	}
	return false;
}

