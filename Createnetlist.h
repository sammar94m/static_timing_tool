/*
 * createnetlist.h
 *
 *  Created on: Nov 16, 2018
 *      Author: compm
 */

#ifndef CREATENETLIST_H_
#define CREATENETLIST_H_

#include "Cell.h"
#include "OutputNet.h"
#include "InputNet.h"
#include "Net.h"
#include "enums.h"
#include "Flipflop.h"
#define BUFFZISE 1024
//global
extern int pathidmax;
extern int pathidmin;
extern map<int, string> MAXpaths;
extern map<int, string> MINpaths;

extern vector<Net*> InputTable;
extern vector<Net*> InputDataTable;
extern vector<Net*> InputClkTable;
extern vector<Net*> OutputTable;
extern map<string, Net*> NetsTable;
extern map<string, Cell*> CellsTable;
extern map<string, CellTemplate*> CellTemplateTable;

void ParasiticsInterconnectFile(const string& filename);
void NetlistFileFormat(const string& filename);

void DesignConstraintsFile(const string& filename);

void LibraryFile(const string& filename);

void dfsPrint();
void MarkClks();

#endif /* CREATENETLIST_H_ */
