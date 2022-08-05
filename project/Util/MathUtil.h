
#ifndef ___MATH_UTIL_H_
#define ___MATH_UTIL_H_

#include <math.h>


///// Integer Operations /////

template<class T>
inline T ABS(T a) {
	return (a >= 0 ? a : (-a));
}

template<class T>
inline T MAX(T a, T b) {
	return (a > b ? a : b);
}

template<class T>
inline T MIN(T a, T b) {
	return (a < b ? a : b);
}

/* Greatest common divisor.
 */
int gcd(int a, int b);

/* Least common multiple.
 */
int lcm (int a, int b);


///// Double Operations /////

#define DOUBLE_MIN_VALUE  0.0001

#define ROUND_UP(d)   ( int(ceil(d)) )
#define ROUND_DOWN(d) ( int(floor(d)) )

inline bool EQUALS(double a, double b) {
	return ABS(a - b) <= DOUBLE_MIN_VALUE;
}

inline bool LESS(double a, double b) {
	return (b - a) > DOUBLE_MIN_VALUE;
}

/* Logaritm of a number in the given base (num > 0, base > 1).
 */
double log(double num, double base);


///// Random Numbers /////

/* Returns a pseudo-random integer value. This function loads and
 * saves the seed automatically between sessions.
 */
int randx();


#endif /* ___MATH_UTIL_H_ */
