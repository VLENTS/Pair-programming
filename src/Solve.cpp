#include "Solve.h"

using namespace std;
typedef long long LL;

const int MAX = 1e5;

Frac num[MAX + 5], _num[MAX + 5];
int sign[MAX + 5], _sign[MAX + 5], brack[MAX + 5][2], nex[MAX + 5];
int itvl[MAX + 5], itvr[MAX + 5];
//当前起始节点为s 往下走n步为终止结点
bool solve(int s, int n, bool is_brack) {
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
		if (!solve(_l, r - l, rand() & 1)) return false;
		itvr[_l] = itvr[_r];
		is_brack = rand() & 1;
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
		while (i < n && sign[temp] == '-') {
			num[temp] = num[temp] - num[nex[temp]];
			if (num[temp].up < 0 || num[temp].down < 0) return false;
			sign[temp] = sign[nex[temp]];
			nex[temp] = nex[nex[temp]];
			n--;
		}
	}

	temp = s;
	for (int i = 0; i < n; i++, temp = nex[temp]) {
		while (i < n) {
			num[temp] = num[temp] + num[nex[temp]];
			sign[temp] = sign[nex[temp]];
			nex[temp] = nex[nex[temp]];
			n--;
		}
	}
	return true;
}

//碰撞上限
const int limit_gene = 10;
unordered_set<Frac, FracHash> used;

//生成长度为n 生成数值x<m
//如果答案碰撞次数超过上限则生成失败 返回 false
bool gene(int n, int m) {
	int cnt = 0;
	while (cnt < limit_gene) {
		for (int i = 1; i <= n; i++) {
			int b = rand() % m;
			if (m > 1)
				while (!b) b = rand() % m;
			else b = 1;
			int a = rand() % (b * m);
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
		if (solve(1, n - 1, rand() & 1) && used.find(num[1]) == used.end()) {
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

const int limit_run_gene = 10000;
//输出乘除号为utf-8编码
bool run_gene(int t, int m) {
	srand(time(0));
	used.clear();
	ofstream ans, out;
	ans.open(".\\Answers.txt");
	out.open(".\\Exercises.txt");
	//输出
	int cnt;
	for (int i = 1; i <= t; i++) {
		cnt = 0;
		int n;

		if (m > 1000) n = 2;
		else if (m > 100) n = (rand() & 1) + 2;
		else n = rand() % 3 + 2;

		while (!gene(n, m)) {
			if (cnt >= limit_run_gene) {
				ans.close();
				out.close();
				return false;
			}
			cnt++;

			if (m > 1000) n = 2;
			else if (m > 100) n = (rand() & 1) + 2;
			else n = rand() % 3 + 2;
		}

		out << i << ".";
		ans << i << ".";

		cnt = demical(i) + 1;
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

			if (j < n) {
				out << " ";
				if (_sign[j] == '*') out << (char)0xC3 << (char)0x97;
				else if (_sign[j] == '/') out << (char)0xC3 << (char)0xB7;
				else out << (char)_sign[j];
				out << " ";
			}
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
	return true;
}
//假设不存在计算过程出现负数和/0的情况
void test(int s) {
	int m = s;
	while (!brack[m][1]) {
		if (brack[m][0]) {
			brack[m][0]--; test(m);
		}
		else m = nex[m];
	}
	int n = 0;
	for (int i = s; i != m; i = nex[i]) n++;
	int temp;

	temp = s;
	for (int i = 0; i < n; i++, temp = nex[temp]) {
		while (i < n && (sign[temp] == '*' || sign[temp] == '/')) {
			if (sign[temp] == '*')
				num[temp] = num[temp] * num[nex[temp]];
			if (sign[temp] == '/')
				num[temp] = num[temp] / num[nex[temp]];

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
			if (sign[temp] == '-')
				num[temp] = num[temp] - num[nex[temp]];
				
			sign[temp] = sign[nex[temp]];
			nex[temp] = nex[nex[temp]];
			n--;
		}
	}
	brack[s][1] = brack[m][1] - 1;
	brack[m][1] = 0;
}

vector<int> correct, wrong;

//假设输入标准 输入乘除号可以为utf-8编码也可以为* /
void run_test(string adr_exe, string adr_ans) {
	ifstream exe, ans; ofstream out;
	exe.open(adr_exe);
	ans.open(adr_ans);
	out.open(".\\Grade.txt");
	string temp;
	correct.clear(); wrong.clear();
	while (getline(exe,temp)) {
		int k = 0, s = 0, cnt = 1;
		while ('0' <= temp[k] && temp[k] <= '9') {
			s = s * 10 + temp[k] - '0';
			k++;
		}
		while (true) {
			while ((temp[k] < '0' || temp[k] > '9') && temp[k] != '(') k++;
			brack[cnt][0] = 0;
			while (temp[k] == '(') {
				brack[cnt][0]++;
				k++;
			}
			while (temp[k] < '0' || temp[k] > '9') k++;
			int up = 0, down = 0;
			while ('0' <= temp[k] && temp[k] <= '9') {
				up = up * 10 + temp[k] - '0';
				k++;
			}
			if (temp[k] == '\'') {
				k++;
				int t = up; up = 0;
				while ('0' <= temp[k] && temp[k] <= '9') {
					up = up * 10 + temp[k] - '0';
					k++;
				}
					
				k++;
				while ('0' <= temp[k] && temp[k] <= '9') {
					down = down * 10 + temp[k] - '0';
					k++;
				}
					
				up += down * t;
			}
			else if (temp[k] == '/') {
				k++;
				while ('0' <= temp[k] && temp[k] <= '9') {
					down = down * 10 + temp[k] - '0';
					k++;
				}
					
			}
			else down = 1;
			num[cnt] = Frac(up, down);
			while (temp[k] == ' ') k++;
			brack[cnt][1] = 0;
			while (temp[k] == ')') {
				brack[cnt][1]++;
				k++;
			}
			while (temp[k] != '=' && temp[k] != '+' && temp[k] != '-'
				&& temp[k] != '*' && temp[k] != '/' && temp[k] != (char)0xC3) {
				k++;
			}
			if (temp[k] == '=') break;
			else {
				if (temp[k] == (char)0xC3) {
					if (temp[++k] == (char)0x97) sign[cnt++] = '*';
					else sign[cnt++] = '/';
				}
				else sign[cnt++] = temp[k];
			}
		}
		brack[cnt][1]++;
		for (int i = 1; i < cnt; i++) nex[i] = i + 1;
		nex[cnt] = 0;
		test(1);
		getline(ans, temp);
		k = 0;
		while (temp[k] != '.') k++;
		while (temp[k] < '0' || temp[k] > '9') k++;
		LL up = 0, down = 0;
		while ('0' <= temp[k] && temp[k] <= '9') {
			up = up * 10 + temp[k] - '0';
			k++;
		}
		if (temp[k] == '\'') {
			k++;

			LL t = up; up = 0;
			while ('0' <= temp[k] && temp[k] <= '9') {
				up = up * 10 + temp[k] - '0';
				k++;
			}
				
			k++;
			while ('0' <= temp[k] && temp[k] <= '9') {
				down = down * 10 + temp[k] - '0';
				k++;
			}
				
			up += down * t;
		}
		else if (temp[k] == '/') {
			k++;
			while ('0' <= temp[k] && temp[k] <= '9') {
				down = down * 10 + temp[k] - '0';
				k++;
			}
				
		}
		else down = 1;
		if (!(num[1] == Frac(up, down))) wrong.push_back(s);
		else correct.push_back(s);
	}

	out << "Correct: " << correct.size() << " (";
	if (correct.size()) {
		out << correct[0];
		for (int i = 1; i < correct.size(); i++) {
			out << ", " << correct[i];
		}
	}
	out << ")\n";

	out << "Wrong: " << wrong.size() << " (";
	if (wrong.size()) {
		out << wrong[0];
		for (int i = 1; i < wrong.size(); i++) {
			out << ", " << wrong[i];
		}
	}
	out << ")\n";

	exe.close();
	ans.close();
	out.close();
}
