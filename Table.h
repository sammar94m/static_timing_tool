#ifndef TABLE_H_
#define TABLE_H_

#include "enums.h"

//ASSUMPTION TABLE MIN SIZE IS 2x2
template<typename T>
class Table {
public:
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

	T** GetTable(MAXMIN AnlsType, InOutTr Tr) {
		return _16tables[pair<MAXMIN, InOutTr>(AnlsType, Tr)];
	}
	bool TableExists(MAXMIN AnlsType, InOutTr Tr){
		for(auto& it : _16tables){
			if(it.first.first==AnlsType && it.first.second==Tr){
				return true;
			}
		}
		return false;
	}
	T GetTableVal(MAXMIN AnlsType, InOutTr Tr, slope inslope, load outload);
	bool withinBounds(slope inslope, load outload);
	void GetCenter(slope& inslope, load& outload);
	T AverageNear(T** arr,slope inslope, load outload);
	T GetDist(slope x1, load y1,slope x2, load y2);
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
template<typename T>
bool Table<T>::withinBounds(slope inslope, load outload) {
	auto& X = *IN_SLOPE_POINTS_PTR;
	auto& Y = *OUT_LOAD_POINTS_PTR;
	slope max_x = X.back();
	slope min_x = X.front();
	load max_y = Y.back();
	load min_y = Y.front();
	if ((inslope >= min_x && inslope <= max_x)
			&& (outload >= min_y && outload <= max_y)) {
		return true;
	} else {
		return false;
	}
}

template<typename T>
void Table<T>::GetCenter(slope& inslope, load& outload) {
	auto& X = *IN_SLOPE_POINTS_PTR;
	auto& Y = *OUT_LOAD_POINTS_PTR;
	slope max_x = X.back();
	slope min_x = X.front();
	load max_y = Y.back();
	load min_y = Y.front();
	inslope = (max_x + min_x) / 2;
	outload = (max_y + min_y) / 2;
}

template<typename T>
T Table<T>::AverageNear(T** arr, slope inslope, load outload) {
	slope xl, xr;
	load yl, yr;
	int ixl, iyl;
	T wl_y, wr_x, wr_y, wl_x;
	T res;
	for (auto it = IN_SLOPE_POINTS_PTR->begin();
			it != IN_SLOPE_POINTS_PTR->end(); it++) {
		if (inslope <= *(it + 1)) {
			xl = *(it);
			ixl = distance(IN_SLOPE_POINTS_PTR->begin(), it);
			xr = *(it + 1);
			break;
		}
	}
	for (auto it = OUT_LOAD_POINTS_PTR->begin();
			it != OUT_LOAD_POINTS_PTR->end(); it++) {
		if (outload <= *(it + 1)) {
			yl = *(it);
			iyl = distance(OUT_LOAD_POINTS_PTR->begin(), it);
			yr = *(it + 1);
			break;
		}
	}
	wl_x = (T) ((xr - inslope) / (xr - xl));
	wr_x = (T) (1 - wl_x);
	wl_y = (T) ((yr - inslope) / (yr - yl));
	wr_y = (T) (1 - wl_y);
	res = wl_x * wl_y * arr[ixl][iyl] + wl_x * wr_y * arr[ixl][iyl+1]
			+ wr_x * wl_y * arr[ixl+1][iyl] + wr_x * wr_y * arr[ixl+1][iyl+1];
	return res;
}


template<typename T>
T Table<T>::GetDist(slope x1, load y1,slope x2, load y2) {
	return ((T)sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1)));
}

template<typename T>
T Table<T>::GetTableVal(MAXMIN AnlsType, InOutTr Tr, slope inslope, load outload){
	T** arr=GetTable(AnlsType,Tr);
	auto& X = *IN_SLOPE_POINTS_PTR;
	auto& Y = *OUT_LOAD_POINTS_PTR;
	slope max_x = X.back();
	slope min_x = X.front();
	load max_y = Y.back();
	load min_y = Y.front();
	if(withinBounds(inslope,outload)){
		return AverageNear(arr,inslope,outload);
	}else{
		slope xc,x1,x2;
		load yc,y1,y2;
		T zc,z1,r,R;
		x2=inslope;
		y2=outload;
		GetCenter(xc,yc);
		zc=AverageNear(arr,xc,yc);
		R=GetDist(xc,yc,x2,y2);
		/*
		 * set x1,y1
		 */
		if(x2>max_x){
			x1=max_x;
		}else if(x2<min_x){
			x1=min_x;
		}else{
			x1=x2;
		}
		if(y2>max_y){
			y1=max_y;
		}else if(y2<min_y){
			y1=min_y;
		}else{
			y1=y2;
		}
		z1=AverageNear(arr,x1,y1);
		r=GetDist(xc,yc,x1,y1);
		return ((z1-zc)/r)*R+zc;
	}
}
#endif

