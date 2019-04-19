#ifndef CELLTEMPLATE_H_
#define CELLTEMPLATE_H_

#include "enums.h"
#include "Table.h"

class CellTemplate {
public:
	map<pair<input_pin, output_pin>, Table<delay>> delayTable;
	map<pair<input_pin, output_pin>, Table<slope>> slopeTable;

	map<pin, load> temp_pinLoadMap;
	vector<slope> IN_SLOPE_POINTS;
	vector<load> OUT_LOAD_POINTS;
	delay setupdata[2][2]; //
	string template_name;
	CellTemplate(string name);
	virtual ~CellTemplate();
	delay getDelay(input_pin in, output_pin out, MAXMIN AnlsType, InOutTr Tr,
			slope inslope, load outload);
	slope getSlope(input_pin in, output_pin out, MAXMIN AnlsType, InOutTr Tr,
			slope inslope, load outload);
	void print();
	bool TableExists(input_pin in, output_pin out, MAXMIN AnlsType, InOutTr Tr);
};

#endif

