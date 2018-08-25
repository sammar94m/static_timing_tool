#ifndef TABLE_H_
#define TABLE_H_

#include "enums.h"

//template<typename T>
class Table {
private:
	int table_rows;
	int table_cols;
	int tables_num;
	map<pair<MAXMIN, Transitions>, int**> _16tables;

public:
	Table() :
			table_rows(-1), table_cols(-1), tables_num(0) {
	}

	void AddTable(int** table, MAXMIN AnlsType, Transitions Tr, int r, int c) {
		if (table_rows < 0 || table_cols < 0) {
			table_rows = r;
			table_cols = c;
		}
		if (!table) {
			cout << "null  at " << __func__ << endl;
		} else {
			_16tables[pair<MAXMIN, Transitions>(AnlsType, Tr)] = table;
			++tables_num;

			//cout<< AnlsType<<" "<<Tr<<" num="<<tables_num<<endl;
		}
	}

	int** GetTable(MAXMIN AnlsType, Transitions Tr, slope inslope,
			load outload) {

		return _16tables.find(pair<MAXMIN, Transitions>(AnlsType, Tr)).operator *().second;
	}

//	~Table() {
//		for (auto iter = _16tables.begin(); iter != _16tables.end(); ++iter) {
//			delete
//		}
//	}

	void print() {
		for (auto iter = _16tables.begin(); iter != _16tables.end(); ++iter) {
			int** table = iter.operator *().second;
			if (!table) {

				cout << "-------------------shit no table " << endl << endl;

			} else {
				cout<< iter.operator *().first.first<<" " <<iter.operator *().first.second<<endl;

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

