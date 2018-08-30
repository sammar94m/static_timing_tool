#ifndef TABLE_H_
#define TABLE_H_

#include "enums.h"

//ASSUMPTION TABLE MIN SIZE IS 2x2
template<typename T>
class Table {
private:
	int table_rows;
	int table_cols;
	int tables_num;
	map<pair<MAXMIN, Transitions>, T**> _16tables;

public:
	vector<slope>* IN_SLOPE_POINTS_PTR;
	vector<load>* OUT_LOAD_POINTS_PTR;

	Table() :
			table_rows(-1), table_cols(-1), tables_num(0), IN_SLOPE_POINTS_PTR(NULL),OUT_LOAD_POINTS_PTR(NULL){
	}

	//	~Table() {
	//		for (auto iter = _16tables.begin(); iter != _16tables.end(); ++iter) {
	//			delete
	//		}
	//	}

	void AddTable(int** table, MAXMIN AnlsType, Transitions Tr, int r, int c);

	T** GetTable(MAXMIN AnlsType, Transitions Tr, slope inslope,
			load outload) ;

	T GetTableVal(MAXMIN AnlsType, Transitions Tr, slope inslope,
				load outload);

	T GetTableAV(MAXMIN AnlsType, Transitions Tr, unsigned int inslopeindex,
			unsigned int outloadindex, bool exactld, bool exactSlp) ;

	T GetExtTableVal(MAXMIN AnlsType, Transitions Tr, slope inslope,
			load outload, unsigned int inslopeindex, unsigned int outloadindex,
			bool exactld, bool exactSlp) ;
	T GetColDelta(MAXMIN AnlsType, Transitions Tr, unsigned int Col,
			bool down) ;

	T GetRowDelta(MAXMIN AnlsType, Transitions Tr, unsigned int Row,
			bool right) ;

	void print();
};

#endif

