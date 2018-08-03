#ifndef TABLE_H_
#define TABLE_H_

#include "enums.h"

template<typename T>
class Table {
private:
	T** ptrMat[2][4];
public:
	Table() :
			ptrMat { 0 } {
	}
	;
	void AddTable(T** table, MAXMIN AnlsType, Transitions Tr) {
		ptrMat[AnlsType][Tr] = table;
	}

	int GetTable(MAXMIN AnlsType, Transitions Tr, slope inslope, load outload) {
		T** matrix = ptrMat[AnlsType][Tr];
		if (!matrix)
			return -1;
		return matrix[inslope][outload];
	}

	~Table() {
		for (int i = 0; i < 2; i++) {
			for (int j = 0; j < 4; j++) {
				if (ptrMat[i][j]) {
					free(*(ptrMat[i][j])); //check this shit
				}

			}
		}
	}
void print(){
	cout<<"print the tables"<<endl<<endl;
}
};


#endif

