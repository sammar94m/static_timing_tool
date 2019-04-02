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
	int setupdata[4]; //
	string template_name;

	CellTemplate(string name) :
			template_name(name) {
		for (int i = 0; i < 4; i++) {
			setupdata[i] = -1;
		}

	}
	virtual ~CellTemplate() {
	}

	delay getDelay(input_pin in, output_pin out, MAXMIN AnlsType, InOutTr Tr,
			slope inslope, load outload);
	delay getSlope(input_pin in, output_pin out, MAXMIN AnlsType, InOutTr Tr,
			slope inslope, load outload);
	bool withinboundry(int ida, bool exacta);
	template<typename T>
	int getClosestindex(vector<T>& vec, int val, bool& exact);

	void print();
};

#endif

