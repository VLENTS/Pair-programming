#include "Frac.h"

using namespace std;
typedef long long LL;

LL gcd(LL a, LL b) {
	return b ? gcd(b, a % b) : a;
}

Frac::Frac() { up = down = 0; }

Frac::Frac(LL x, LL y) {
	LL g = gcd(x, y);
	up = x / g; down = y / g;
}

Frac Frac::operator + (const Frac& x) const {
	LL _up = up * x.down + x.up * down;
	LL _down = down * x.down;
	LL g = gcd(_up, _down);
	return Frac{ _up / g,_down / g };
}

Frac Frac::operator - (const Frac& x) const {
	LL _up = up * x.down - x.up * down;
	LL _down = down * x.down;
	LL g = gcd(_up, _down);
	return Frac{ _up / g,_down / g };
}

Frac Frac::operator * (const Frac& x) const {
	LL _up = up * x.up;
	LL _down = down * x.down;
	LL g = gcd(_up, _down);
	return Frac{ _up / g,_down / g };
}

Frac Frac::operator / (const Frac& x) const {
	LL _up = up * x.down;
	LL _down = down * x.up;
	LL g = gcd(_up, _down);
	return Frac{ _up / g,_down / g };
}

bool Frac::operator == (const Frac& x) const {
	return up == x.up && down == x.down;
}

size_t FracHash::operator() (const Frac& x) const {
	size_t seed = x.up + 0x9e3779b9;
	return seed ^= x.down + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}
