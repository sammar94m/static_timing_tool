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

using namespace std;

#define _FALL_ 0
#define _RISE_ 1
typedef vector<string> buffer;
typedef int delay;
typedef int long slope;
typedef int margin;
typedef int load;
typedef std::string input_pin;
typedef std::string output_pin;
typedef std::string pin;

//global
extern int pathid;
extern map<int,string> MAXpaths;
extern map<int,string> MINpaths;


enum cellType {
	INCELL, OUTCELL, FlIPFlOP, COMB, UNKNOWN
};

typedef enum _Trnsitions {
	/* per 1 wire*/FALL = 0, RISE = 3,/*per in and an out */
	FF = 1, RF = 4, FR = 2, RR = 5
} Transitions;

typedef enum _MAXMIN {
	MIN, MAX
} MAXMIN;


enum FlopSetup {
	MAX_RR, MAX_FR, MIN_RR, MIN_FR
};



typedef enum _netType {
	Local, INPUT, OUTPUT
} netType;



#endif





