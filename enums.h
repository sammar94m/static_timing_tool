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

using namespace std;

#define _FALL_ 0
#define _RISE_ 1
typedef vector<string> buffer;
typedef int delay;
typedef int slope; //long int
typedef int margin;
typedef int load;
typedef std::string input_pin;
typedef std::string output_pin;
typedef std::string pin;

//global
extern int pathidmax, pathidmin;
extern map<int, string> MAXpaths;
extern map<int, string> MINpaths;
typedef enum Timetag_ {
	BF, BR, AF, AR
} timetag;
typedef struct valid_ {
	int val;
	timetag tag;
} valid;
typedef struct required_ {
	int val;
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

#endif

