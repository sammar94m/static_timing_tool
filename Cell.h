#ifndef CELL_H_
#define CELL_H_
#include "netfwd.h"
#include "enums.h"
#include "CellTemplate.h"
#include "Table.h"
#include "Pin.h"
#include "path.h"

typedef std::pair<std::pair<MAXMIN, InOutTr>, std::pair<input_pin, output_pin> > DelCacheKey;

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
	map<DelCacheKey, delay> SlpCache;
	time_t visittime;
	int ready_inputs;
	bool visited;
	bool isCrit[2] = { false, false };
	virtual ~Cell();
	Cell();
	Cell(cellType _type, string _name, CellTemplate* _Template);
	virtual load getCin(pin input);
	const map<output_pin, Net*>& getOutMap();

	virtual load getCout(output_pin out);
	string getName();
	virtual delay getDelay(input_pin in, output_pin out, MAXMIN AnlsType,
			InOutTr Tr, slope inslope, load outload);
	virtual slope getSlope(input_pin in, output_pin out, MAXMIN AnlsType,
			InOutTr Tr, slope inslope, load outload);
	bool PossiblTr(input_pin in, output_pin out,MAXMIN M, InOutTr Tr);

	void updateWCdat(pin PIN, margin Margin, MAXMIN mode,
			InOutTr Tr/*FALL OR RISE*/);
	margin getWCdat(input_pin input, MAXMIN MODE);
	virtual bool isReady();
	void print();
	void printpins(MAXMIN M,ofstream& f);
	void CalcOutputData();
	int getnumofinputs();
	void resetReq();
	void CalcInputReq();
	void RecordBS(PriorityQ<branchslack, BRANCHCompare>& Q, MAXMIN M);
	void CalReq(pin in, pin out, required (&inreq)[2][2], Tr (&inTr)[2][2],
			const required (&outreq)[2][2]);
	pin getWCpin(MAXMIN);
};

#endif

