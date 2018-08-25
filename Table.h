#ifndef TABLE_H_
#define TABLE_H_

#include "enums.h"

//ASSUMPTION TABLE MIN SIZE IS 2x2
//template<typename T>
class Table {
private:
	int table_rows;
	int table_cols;
	int tables_num;
	map<pair<MAXMIN, Transitions>, int**> _16tables;

public:
	vector<slope>* IN_SLOPE_POINTS_PTR;
	vector<load>* OUT_LOAD_POINTS_PTR;

	Table() :
			table_rows(-1), table_cols(-1), tables_num(0), IN_SLOPE_POINTS_PTR(NULL),OUT_LOAD_POINTS_PTR(NULL){
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
	T GetTableAV(MAXMIN AnlsType, Transitions Tr, unsigned int inslopeindex,
			unsigned int outloadindex, bool exactld, bool exactSlp) {
		T tmp = 0;
		if (exactSlp && exactld) { //exact
			return this->GetTableVal(AnlsType, Tr, inslopeindex, outloadindex);
		} else { //not exact average nearby
			if (inslopeindex != 0 && outloadindex != 0) {
				tmp += this->GetTableVal(AnlsType, Tr, inslopeindex - 1,
						outloadindex - 1);
				tmp += this->GetTableVal(AnlsType, Tr, inslopeindex - 1,
						outloadindex);
				tmp += this->GetTableVal(AnlsType, Tr, inslopeindex,
						outloadindex - 1);
				tmp += this->GetTableVal(AnlsType, Tr, inslopeindex,
						outloadindex);
				return tmp / 4;
			} else if (inslopeindex == 0) { // one of them is zero - if both the first if would be taken
				tmp += this->GetTableVal(AnlsType, Tr, inslopeindex,
						outloadindex - 1);
				tmp += this->GetTableVal(AnlsType, Tr, inslopeindex,
						outloadindex);
				return tmp / 2;
			} else if (outloadindex == 0) {
				tmp += this->GetTableVal(AnlsType, Tr, inslopeindex,
						outloadindex);
				tmp += this->GetTableVal(AnlsType, Tr, inslopeindex - 1,
						outloadindex);
				return tmp / 2;
			}
		}
		return -1;
	}

	T GetExtTableVal(MAXMIN AnlsType, Transitions Tr, slope inslope,
			load outload, unsigned int inslopeindex, unsigned int outloadindex,
			bool exactld, bool exactSlp) {
		//edge cases
		int ldsize = OUT_LOAD_POINTS_PTR->size();
		int slpsize = IN_SLOPE_POINTS_PTR->size();
		int ldind = outloadindex == -1 ? ldsize - 1 : outloadindex;
		int slpind = inslopeindex == -1 ? slpsize - 1 : inslopeindex;
		T RowDel = GetRowDelta(AnlsType, Tr, slpind, outloadindex == -1);
		T ColDel = GetColDelta(AnlsType, Tr, ldind, inslopeindex -= -1);
		slope yDel = inslope - IN_SLOPE_POINTS_PTR->operator [](slpind);
		delay xDel = outload - OUT_LOAD_POINTS_PTR->operator [](ldind); //TODO: ADD EXACT/NONEXACT CASES
		if ((inslopeindex -= -1 || inslopeindex -= 0)
				&& (outloadindex == -1 || outloadindex == 0)) { //two indexes outside
			return GetTableVal(AnlsType, Tr, slpind, ldind) + xDel * RowDel
					+ yDel * ColDel;
		} else if (inslopeindex == -1 || inslopeindex == 0) {
			return GetTableVal(AnlsType, Tr, slpind, ldind) + yDel * ColDel;
		} else if (outloadindex == -1 || outloadindex == 0) {
			return GetTableVal(AnlsType, Tr, slpind, ldind) + xDel * RowDel;
		}
		return -1;
	}
	T GetColDelta(MAXMIN AnlsType, Transitions Tr, unsigned int Col,
			bool down) {
		T Del;
		int size = IN_SLOPE_POINTS_PTR->size();
		if (!down) {
			Del = GetTableVal(AnlsType, Tr, 0, Col)
					- GetTableVal(AnlsType, Tr, 1, Col);
			return Del
					/ (IN_SLOPE_POINTS_PTR->operator [](0)
							- IN_SLOPE_POINTS_PTR->operator [](1));
		} else {
			Del = GetTableVal(AnlsType, Tr, size - 1, Col)
					- GetTableVal(AnlsType, Tr, size - 2, Col);
			return Del
					/ (IN_SLOPE_POINTS_PTR->operator [](size - 1)
							- IN_SLOPE_POINTS_PTR->operator [](size - 2));

		}
	}

	T GetRowDelta(MAXMIN AnlsType, Transitions Tr, unsigned int Row,
			bool right) {
		T Del;
		int size = OUT_LOAD_POINTS_PTR->size();
		if (!right) {
			Del = GetTableVal(AnlsType, Tr, Row, 0)
					- GetTableVal(AnlsType, Tr, Row, 1);
			return Del
					/ (OUT_LOAD_POINTS_PTR->operator [](0)
							- OUT_LOAD_POINTS_PTR->operator [](1));
		} else {
			Del = GetTableVal(AnlsType, Tr, Row, size - 1)
					- GetTableVal(AnlsType, Tr, Row, size - 2);
			return Del
					/ (OUT_LOAD_POINTS_PTR->operator [](size - 1)
							- OUT_LOAD_POINTS_PTR->operator [](size - 2));

		}
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

