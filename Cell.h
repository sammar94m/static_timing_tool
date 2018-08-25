#ifndef CELL_H_
#define CELL_H_

#include "enums.h"
//#include "Net.h"
#include "Clockdat.h"
#include "CellTemplate.h"
#include "Table.h"


class Net;

class Cell {
public:
	cellType type;
	string name;
	CellTemplate * Template;
	std::map<input_pin, Net* > inMap;
	map<output_pin, Net* > outMap;
	map<pin, load> pinLoadMap;
	map<pin, margin> WCmarg[2];
	Transitions WCtr[2];
public:
	virtual ~Cell(){
		delete Template;
	}

	Cell(){
		Template = NULL;
		type =UNKNOWN;
	}
	Cell(cellType type, string name, CellTemplate *Template) :
			type(type), name(name), Template(NULL) {


	}

	//virtual void saveclkdat(clockdat cd){};

	load getCin(pin input) {
		return Template->temp_pinLoadMap[input];
	}
	const map<output_pin, Net*>& getOutMap() {
		return outMap;
	}
	load getCout(output_pin out);
	string getName() {
		return name;
	}
//	delay getDelay(input_pin in, output_pin out, MAXMIN AnlsType,
//			Transitions Tr, slope inslope, load outload) {
//		Table<delay>* table = &Template->delayTable[pair<input_pin, output_pin>(
//				in, out)];
//		return table->GetTable(AnlsType, Tr, inslope, outload);
//
//	}
//	slope getSlope(input_pin in, output_pin out, MAXMIN AnlsType,
//			Transitions Tr, slope inslope, load outload) {
//		Table<slope>* table = &Template->slopeTable[pair<input_pin, output_pin>(
//				in, out)];
//		return table->GetTable(AnlsType, Tr, inslope, outload);
//	}
//	bool PossiblTr(input_pin in, output_pin out, Transitions Tr){
//		int Poss=this->Template->delayTable[make_pair(in,out)].GetTable(MAX,Tr,0,0);
//		return Poss==-1 ? false : true;
//	}

	void updateWCdat(pin PIN, margin Margin, MAXMIN mode,
			Transitions Tr/*FALL OR RISE*/){
		if(WCmarg[mode].find(PIN)==WCmarg[mode].end()){
			WCmarg[mode][PIN]=Margin;
			WCtr[mode]=Tr;
		}else{
			WCmarg[mode][PIN] = WCmarg[mode][PIN]>Margin ? Margin : WCmarg[mode][PIN];
			WCtr[mode]=WCmarg[mode][PIN]>Margin ? Tr: WCtr[mode];
		}
	}
	margin getWCdat(input_pin input, MAXMIN MODE){
		return WCmarg[MODE][input];
	}
	static void print() {
		static int cnt = 0;
		if (cnt == 0) {
			cout << "bla bla " << endl;
			cnt = 1;
		}
	}
};




#endif

