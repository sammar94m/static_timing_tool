#ifndef CELL_H_
#define CELL_H_

#include "enums.h"
#include "Net.h"
#include "Clockdat.h"
#include "CellTemplate.h"
#include "Table.h"
#include "Pin.h"
typedef std::pair< std::pair<MAXMIN, InOutTr> , std::pair<input_pin, output_pin> > DelCacheKey;
class Cell {
public:
	cellType type;
	string name;
	CellTemplate * Template;
	map<input_pin, Net*> inMap;
	Net* outNet;
	pin outPin;
	map<pin, load> pinLoadMap;
	map<pin, PinDat> PinData;
	map<DelCacheKey, delay> DelCache;
	time_t visittime;
	int ready_inputs;
	bool visited;
public:
	virtual ~Cell() {
		delete Template;
	}

	Cell() {
		Template = NULL;
		type = UNKNOWN;
		outNet = NULL;
		visittime = time(NULL);
		ready_inputs = 0;
		visited=false;
	}
	Cell(cellType _type, string _name, CellTemplate* _Template) :
			type(_type), name(_name), Template(_Template), outNet(NULL), visittime(
					time(NULL)), ready_inputs(0),visited(false) {
	}

	//virtual void saveclkdat(clockdat cd){};

	virtual load getCin(pin input);
	const map<output_pin, Net*>& getOutMap();

	virtual load getCout(output_pin out);
	string getName();
	virtual delay getDelay(input_pin in, output_pin out, MAXMIN AnlsType,
			InOutTr Tr, slope inslope, load outload);
	virtual slope getSlope(input_pin in, output_pin out, MAXMIN AnlsType,
			InOutTr Tr, slope inslope, load outload);
	bool PossiblTr(input_pin in, output_pin out, InOutTr Tr);

	void updateWCdat(pin PIN, margin Margin, MAXMIN mode,
			InOutTr Tr/*FALL OR RISE*/);
	margin getWCdat(input_pin input, MAXMIN MODE);
	inline bool isReady() {
		return ready_inputs == getnumofinputs();
	}
	void print();
	void CalcOutputData();
	int getnumofinputs() {
		return inMap.size();
	}
	void CalcInputReq();
	void CalReq(pin in, pin out, required (&inreq)[2][2],
			const required (&outreq)[2][2]);
};

#endif

