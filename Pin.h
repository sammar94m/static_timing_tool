#ifndef PIN_H_
#define PIN_H_


#include "enums.h"
#include "Net.h"

class Pin {
	string name;
	margin WCMarR;
	slope WCSlopeR;
	margin WCMarF;
	slope WCSlopeF;
public:
	Pin(Net* Con, string namestr) :
			name(namestr), WCMarR(), WCSlopeR(INT_MAX), WCMarF(INT_MAX), WCSlopeF(
			INT_MAX), validF(false), validR(false) {
	}
	void updateWC(margin MAR, slope SLOP, Transitions Tr /* Fall or Rise */) {
		if (Tr == FALL) {
			WCMarF = WCMarF < MAR ? WCMarF : MAR;
			WCSlopeF = WCSlopeF > SLOP ? WCSlopeF : SLOP;
			validF = true;
		} else {
			WCMarR = WCMarR < MAR ? WCMarR : MAR;
			WCSlopeR = WCSlopeR > SLOP ? WCSlopeR : SLOP;
			validR = true;
		}
	}
private:
	bool validF;
	bool validR;
};





#endif
