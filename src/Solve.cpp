#include "Solve.h"
#include "Frac.h"

using namespace std;
typedef long long LL;

const int MAX = 1e5;

int is_brack;
Frac num[MAX + 5], _num[MAX + 5];
int sign[MAX + 5], _sign[MAX + 5], brack[MAX + 5][2], nex[MAX + 5];
int itvl[MAX + 5], itvr[MAX + 5];
//当前起始节点为s 往下走n步为终止结点
bool solve(int s, int n) {
	while (n > 1 && is_brack) {
		int l = rand() % n;
		int r = rand() % n + 1;
		while (l >= r || r == n && l == 0) r = rand() % n + 1;
		int _l = s;
		for (int i = 0; i < l; i++) _l = nex[_l];
		int _r = s;
		for (int i = 0; i < r; i++) _r = nex[_r];
		brack[itvl[_l]][0]++;
		brack[itvr[_r]][1]++;
		n -= r - l;
		if (!solve(_l, r - l)) return false;
		itvr[_l] = itvr[_r];
	}
	int temp;

	temp = s;
	for (int i = 0; i < n; i++, temp = nex[temp]) {
		while (i < n && (sign[temp] == '*' || sign[temp] == '/')) {
			if (sign[temp] == '*')
				num[temp] = num[temp] * num[nex[temp]];
			if (sign[temp] == '/') {
				if (num[nex[temp]].up == 0) return false;
				num[temp] = num[temp] / num[nex[temp]];
			}
			sign[temp] = sign[nex[temp]];
			nex[temp] = nex[nex[temp]];
			n--;
		}
	}
	temp = s;
	for (int i = 0; i < n; i++, temp = nex[temp]) {
		while (i < n && (sign[temp] == '+' || sign[temp] == '-')) {
			if (sign[temp] == '+')
				num[temp] = num[temp] + num[nex[temp]];
			if (sign[temp] == '-') {
				num[temp] = num[temp] - num[nex[temp]];
				if (num[temp].up < 0 || num[temp].down < 0) return false;
			}
			sign[temp] = sign[nex[temp]];
			nex[temp] = nex[nex[temp]];
			n--;
		}
	}
	return true;
}

//碰撞上限
const int limit = 10;
unordered_set<Frac, FracHash> used;

//不考虑生成0
//生成长度为n 生成数值x<=m
//如果答案碰撞次数超过上限则生成失败 返回 false
bool gene(int n, int m) {
	int cnt = 0;
	while (cnt < limit) {
		for (int i = 1; i <= n; i++) {
			int a = rand() % m + 1;
			int b = rand() % m + 1;
			num[i] = _num[i] = Frac(a, b);
			itvl[i] = itvr[i] = i;
			if (i < n) {
				nex[i] = i + 1;
				int temp = rand() % 4 + 1; char c;
				if (temp == 1) c = '+';
				else if (temp == 2) c = '-';
				else if (temp == 3) c = '*';
				else c = '/';
				sign[i] = _sign[i] = c;
			}
			else nex[i] = 0;
		}
		for (int i = 1; i <= n; i++)
			brack[i][0] = brack[i][1] = 0;
		is_brack = rand() & 1;
		if (solve(1, n - 1) && used.find(num[1]) == used.end()) {
			used.insert(num[1]); return true;
		}
		cnt++;
	}
	return false;
}

//计算整数有多少位 0算一位
int demical(int x) {
	int cnt = 0;
	while (x) {
		x /= 10;
		cnt++;
	}
	return max(1, cnt);
}

void run_gene(int t, int m) {
	used.clear();
	ofstream ans, out;
	ans.open(".\\Answers.txt");
	out.open(".\\Exercises.txt");
	//输出
	for (int i = 1; i <= t; i++) {
		int n = (rand() & 1) + 2;
		while (!gene(n, m)) {
			n = 3;
		}

		out << i << ".";
		ans << i << ".";

		int cnt = demical(i) + 1;
		while (cnt < 6) {
			out << ' ';
			ans << ' ';
			cnt++;
		}

		for (int j = 1; j <= n; j++) {
			while (brack[j][0]--) out << "(";
			bool is;
			is = false;
			if (_num[j].up >= _num[j].down) {
				out << _num[j].up / _num[j].down; is = true;
				if (_num[j].up % _num[j].down) {
					if (is) out << "'";
					if (_num[j].down == 1) out << _num[j].up % _num[j].down;
					else out << _num[j].up % _num[j].down << "/" << _num[j].down;
				}
			}
			else if (_num[j].up == 0) out << "0";
			else out << _num[j].up << "/" << _num[j].down;

			while (brack[j][1]--) out << ")";

			if (j < n) out << " " << (char)_sign[j] << " ";
			else {
				out << " = ";
				is = false;
				if (num[1].up >= num[1].down) {
					ans << num[1].up / num[1].down; is = true;
					if (num[1].up % num[1].down) {
						if (is) ans << "'";
						if (num[1].down == 1) ans << num[1].up % num[1].down;
						else ans << num[1].up % num[1].down << "/" << num[1].down;
					}
				}
				else if (num[1].up == 0) ans << "0";
				else ans << num[1].up << "/" << num[1].down;
				ans << endl; out << endl;
			}
		}
	}
	ans.close(); out.close();
}