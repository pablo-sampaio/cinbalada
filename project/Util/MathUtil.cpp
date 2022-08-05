
#include "MathUtil.h"

#include <iostream>
#include <fstream>

using namespace std;


/* Auxiliary class. Loads and saves the seed.
 */
class __SmartSeed {
private:
	ifstream* in;
public:
	int lastRandomValue;

	__SmartSeed() {
    	/*try {
        	in = new ifstream("randx-seed.cfg", ios_base::in |
					              ios_base::out); // | ios_base::trunc);
			in->seekg(0);
			(*in) >> lastRandomValue;
        } catch (...) {
			lastRandomValue = 1;
        }

	    srand(lastRandomValue);*/
    }

	virtual ~__SmartSeed() {
    	try {
			/*in->seekg(0);
			ostream out(in->rdbuf());
			out << lastRandomValue;
            in->close();
            delete in;*/
        } catch (...) {
			// faz nada...
        }
    }
};

static __SmartSeed saver;


int gcd (int num1, int num2)
{
    int t;
    if (num1 > num2) {
        t = num2;
        num2 = num1;
        num1 = t;
    }

	int remainder = num2 % num1;

	if ( remainder != 0 )
		return gcd( remainder, num1 );

	return num1;
}

int lcm (int a, int b) {
    return (a * b) / gcd(a,b);
}

double log(double num, double base) {
	return log(num) / log(base);
}

int randx() {
    saver.lastRandomValue = rand();
    return saver.lastRandomValue;
}

