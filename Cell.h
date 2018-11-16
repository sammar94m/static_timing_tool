#ifndef CELL_H_
#define CELL_H_

#include "enums.h"
#include "Net.h"
#include "Clockdat.h"
#include "CellTemplate.h"
#include "Table.h"

class Cell {
public:
	cellType type;
	string name;
	CellTemplate * Template;
	std::map<input_pin, Net*> inMap;
	map<output_pin, Net*> outMap;
	map<pin, load> pinLoadMap;
	map<pin, margin> WCmarg[2];
	Transitions WCtr[2];
public:
	virtual ~Cell() {
		delete Template;
	}

	Cell() {
		Template = NULL;
		type = UNKNOWN;
	}
	Cell(cellType type, string name, CellTemplate *Template) :
			type(type), name(name), Template(Template) {

	}

	//virtual void saveclkdat(clockdat cd){};

	virtual load getCin(pin input);
	const map<output_pin, Net*>& getOutMap();

	virtual load getCout(output_pin out);
	string getName();
	virtual delay getDelay(input_pin in, output_pin out, MAXMIN AnlsType,
			Transitions Tr, slope inslope, load outload);
	virtual slope getSlope(input_pin in, output_pin out, MAXMIN AnlsType,
			Transitions Tr, slope inslope, load outload);
	bool PossiblTr(input_pin in, output_pin out, Transitions Tr);

	void updateWCdat(pin PIN, margin Margin, MAXMIN mode,
			Transitions Tr/*FALL OR RISE*/);
	margin getWCdat(input_pin input, MAXMIN MODE);
	void print();
};

#endif

