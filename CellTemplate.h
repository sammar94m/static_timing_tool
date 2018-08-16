#ifndef CELLTEMPLATE_H_
#define CELLTEMPLATE_H_

#include "enums.h"
#include "Table.h"

class CellTemplate {
public:
	map<pair<input_pin, output_pin>, Table<delay> > delayTable;
	map<pair<input_pin, output_pin>, Table<slope> > slopeTable;

	map<pin, load> temp_pinLoadMap;
	vector<int> IN_SLOPE_POINTS;
	vector<int> OUT_LOAD_POINTS;
	int setupdata[4]; //
	string template_name;

	CellTemplate(string name) :
			template_name(name) {
		for (int i = 0; i < 4; i++) {
			setupdata[i] = -1;
		}

	}
	virtual ~CellTemplate(){}
	void print() {
		cout << "name: " << template_name << endl;
		cout << "setupdata: " << setupdata[0] << " " << setupdata[1] << " "
				<< setupdata[2] << " " << setupdata[3] << endl;
		cout << "IN_SLOPE_POINTS: ";
		for (int i = 0; i < (int)IN_SLOPE_POINTS.size(); i++) {
			cout << IN_SLOPE_POINTS[i] << " ";
		}
		cout << endl;
		cout << "OUT_LOAD_POINTS: ";
		for (int i = 0; i < (int)OUT_LOAD_POINTS.size(); i++) {
			cout << OUT_LOAD_POINTS[i] << " ";
		}
		cout << endl;
		cout << "pin->Load Map: " << endl;
		for (auto it = temp_pinLoadMap.begin(); it != temp_pinLoadMap.end();
				++it) {
			cout << it->first << " -> " << it->second << endl;
		}
		cout << "input_pin , output_pin -> delay " << endl;
		for (auto it = delayTable.begin(); it != delayTable.end(); ++it) {
			cout << it->first.first<<" , "<<it->first.second<<" : "<<endl;
			it->second.print();
		}

		cout << "input_pin , output_pin -> slope " << endl;
		for (auto it = slopeTable.begin(); it != slopeTable.end(); ++it) {
			cout << it->first.first<<" , "<<it->first.second<<" : "<<endl;
			it->second.print();
		}

	}
};



#endif


