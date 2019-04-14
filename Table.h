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
	map<pair<MAXMIN, InOutTr>, T**> _16tables;

public:
	vector<slope>* IN_SLOPE_POINTS_PTR;
	vector<load>* OUT_LOAD_POINTS_PTR;

	Table() :
			table_rows(-1), table_cols(-1), tables_num(0), IN_SLOPE_POINTS_PTR(
			NULL), OUT_LOAD_POINTS_PTR(NULL) {
	}

	void AddTable(T** table, MAXMIN AnlsType, InOutTr Tr, int r, int c) {
		if (table_rows < 0 || table_cols < 0) {
			table_rows = r;
			table_cols = c;
		}
		if (!table) {
			cout << "null  at " << __func__ << endl;
		} else {
			_16tables[pair<MAXMIN, InOutTr>(AnlsType, Tr)] = table;
			++tables_num;

			//cout<< AnlsType<<" "<<Tr<<" num="<<tables_num<<endl;
		}
	}

	T** GetTable(MAXMIN AnlsType, InOutTr Tr, slope inslope, load outload) {
		return _16tables[pair<MAXMIN, InOutTr>(AnlsType, Tr)];
	}
	T GetTableVal(MAXMIN AnlsType, InOutTr Tr, slope inslope, load outload);
	T GetTableAV(MAXMIN AnlsType, InOutTr Tr, int inslopeindex,
			int outloadindex, bool exactld, bool exactSlp);
	T GetExtTableVal(MAXMIN AnlsType, InOutTr Tr, slope inslope, load outload,
			int inslopeindex, int outloadindex, bool exactld, bool exactSlp);
	T GetColDelta(MAXMIN AnlsType, InOutTr Tr, unsigned int Col, bool down);

	T GetRowDelta(MAXMIN AnlsType, InOutTr Tr, unsigned int Row, bool right);

//	~Table() {
//		for (auto iter = _16tables.begin(); iter != _16tables.end(); ++iter) {
//			delete
//		}
//	}

	void print() {
		for (auto iter = _16tables.begin(); iter != _16tables.end(); ++iter) {
			T** table = iter.operator *().second;
			if (!table) {

				cout << "-------------------shit no table " << endl << endl;

			} else {
				cout << iter.operator *().first.first << " "
						<< iter.operator *().first.second << endl;

				for (int r = 0; r < table_rows; r++) {
					for (int c = 0; c < table_cols; c++) {
						cout << table[r][c] << " | ";
					}
					cout << endl;
				}
			}
		}
		cout << "+++++++++++++++++++++++++++++++++++++" << endl;
	}
};

#endif

