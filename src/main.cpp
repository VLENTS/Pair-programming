#include "main.h"

using namespace std;
typedef long long LL;

const int limit_n = 100000;
const int limit_r = 1000;

int main(int argc, char* argv[]) {
	clock_t st, en;
	st = clock();
	srand(time(0));
	if (argc != 5) {
		cout << "argc error!\n"
			<< "correct arguments mode:\n"
			<< "1. project.exe -n (0,100000] -r (0,1000]\n"
			<< "2. project.exe -e <exercisefile>.txt -a <answerfile>.txt\n";
	}
	else if (argv[1][1] == 'n' && argv[3][1] == 'r') {
		int a = 0, b = 0, i;
		i = 0;
		while ('0' <= argv[2][i] && argv[2][i] <= '9') {
			a = a * 10 + argv[2][i] - '0'; i++;
		}
		if (a > limit_n || a <= 0) {
			cout << "argument number after \"-n\" should between 1 and 100000.\n";
			return 0;
		}
		if (argv[2][i]) {
			cout << "argument number after \"-n\" should be pure number.\n";
			return 0;
		}
		i = 0;
		while ('0' <= argv[4][i] && argv[4][i] <= '9') {
			b = b * 10 + argv[4][i] - '0'; i++;
		}
		if (b > limit_r || b <= 0) {
			cout << "argument number after \"-r\" should between 1 and 1000.\n";
			return 0;
		}
		if (argv[4][i]) {
			cout << "argument number after \"-r\" should be pure number.\n";
			return 0;
		}
		if (!run_gene(a, b)) cout << "generate failure! you can reduce n or enlarge r.\n";
		else cout << "generate success!\n";
	}
	else if (argv[1][1] == 'e' && argv[3][1] == 'a') {
		string exe = string(argv[2]), ans = string(argv[4]);
		if (_access(exe.c_str(), 00) == -1 || _access(ans.c_str(), 00 == -1))
			cout << "adress error!\n";
		else {
			sana_test(exe, ans);
			cout << "proofreading success!\n";
		}
	}
	else {
		cout << "mode error!\n"
			<< "correct arguments mode:\n"
			<< "1. project.exe -n (0,100000] -r (0,1000]\n"
			<< "2. project.exe -e <exercisefile>.txt -a <answerfile>.txt\n";
	}
	en = clock();
	printf("Running Time: %.3f\n", (double)(en - st) / CLOCKS_PER_SEC);
	return 0;
}
