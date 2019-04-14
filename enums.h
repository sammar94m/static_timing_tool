#ifndef ENUMS_H_
#define ENUMS_H_

#include <list>
#include <map>
#include <string>
#include <unordered_map>
#include <iostream>
#include <stack>
#include <vector>
#include <queue>
#include <limits.h>
#include <limits>
#include <sstream>
#include <iostream>
#include <stdlib.h>
#include <functional>
#include <fstream>
#include <stack>
#include <vector>
#include <string.h>
#include <string>
#include <utility>
#include <math.h>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <time.h>
#include "netfwd.h"

using namespace std;

#define _FALL_ 0
#define _RISE_ 1
typedef vector<string> buffer;
typedef double delay;
typedef double slope; //long int
typedef double margin;
typedef double load;
typedef std::string input_pin;
typedef std::string output_pin;
typedef std::string pin;


typedef enum Timetag_ {
	BF, BR, AF, AR
} timetag;
typedef struct valid_ {
	delay val;
	timetag tag;
} valid;
typedef struct required_ {
	delay val;
	timetag tag;
} required;
enum cellType {
	INCELL, OUTCELL, FlIPFlOP, COMB, UNKNOWN
};

typedef enum _inOutTr {
	FF, RF, FR, RR
} InOutTr;
typedef enum _Tr {
	FALL, RISE
} Tr;
typedef enum _MAXMIN {
	MIN, MAX
} MAXMIN;

enum FlopSetup {
	MAX_RR, MAX_FR, MIN_RR, MIN_FR
};

typedef enum _netType {
	LOCAL, INPUT, OUTPUT
} netType;
InOutTr GetInOut(Tr in, Tr out);

//global
extern map<int, string> MAXpaths;
extern map<int, string> MINpaths;
extern queue<Net*> InputDataTable;
extern vector<Net*> OutputTable;
extern unsigned int numofpaths;
extern time_t resettime;

#endif

