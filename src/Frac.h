#ifndef FRAC_H
#define FRAC_H

typedef long long LL;

LL gcd(LL a, LL b);

// 暂不考虑符号与初始化分母为0的问题
struct Frac {
	LL up, down;
	Frac();
	Frac(LL x, LL y);
	Frac operator + (const Frac& x) const;
	Frac operator - (const Frac& x) const;
	Frac operator * (const Frac& x) const;
	Frac operator / (const Frac& x) const;
	bool operator == (const Frac& x) const;
};

struct FracHash {
	size_t operator() (const Frac& x) const;
};

#endif // !FRAC_H

