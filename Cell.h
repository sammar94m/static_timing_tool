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
	// flag = 1 if visited
public:
	virtual ~Cell(){
		delete Template;
	}
//	Cell() {
//		Template = NULL;
//		type = COMB;
//	}
//	;
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
	delay getDelay(input_pin in, output_pin out, MAXMIN AnlsType,
			Transitions Tr, slope inslope, load outload) {
		Table<delay>* table = &Template->delayTable[pair<input_pin, output_pin>(
				in, out)];
		return table->GetTable(AnlsType, Tr, inslope, outload);

	}
	slope getSlope(input_pin in, output_pin out, MAXMIN AnlsType,
			Transitions Tr, slope inslope, load outload) {
		Table<slope>* table = &Template->slopeTable[pair<input_pin, output_pin>(
				in, out)];
		return table->GetTable(AnlsType, Tr, inslope, outload);
	}
	bool PossiblTr(input_pin in, output_pin out, Transitions Tr);
	void updateWCdat(pin PIN, margin Margin, MAXMIN mode,
			Transitions Tr/*FALL OR RISE*/); // if less than
	margin getWCdat(input_pin input, MAXMIN MODE,
			Transitions inPossiTr /*FALL OR RISE*/);
	static void print() {
		static int cnt = 0;
		if (cnt == 0) {
			cout << "bla bla " << endl;
			cnt = 1;
		}
	}
};




#endif

