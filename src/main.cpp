#include "main.h"

using namespace std;
typedef long long LL;

const int limit_arg = 100000;

int main(int argc, char* argv[]) {
	srand(time(0));
	if (argc != 5) {
		cout << "argc error!\n"
			<< "correct arguments mode:\n"
			<< "1. project.exe -n [0,100000) -r [0,100000)\n"
			<< "2. project.exe -e <exercisefile>.txt -a <answerfile>.txt\n";
	}
	else if (argv[1][1] == 'n' && argv[3][1] == 'r') {
		int a = 0, b = 0, i;
		i = 0;
		while ('0' <= argv[2][i] && argv[2][i] <= '9') {
			a = a * 10 + argv[2][i] - '0'; i++;
			if (a > limit_arg) {
				cout << "arg too large! argument number2 should between 0 and 100000 but 100000.\n";
				return 0;
			}
		}
		if (argv[2][i]) {
			cout << "arg error! argument number2 should be pure number.\n";
			return 0;
		}
		i = 0;
		while ('0' <= argv[4][i] && argv[4][i] <= '9') {
			b = b * 10 + argv[4][i] - '0'; i++;
			if (b > limit_arg) {
				cout << "arg too large! argument number4 should between 0 and 100000 but 100000.\n";
				return 0;
			}
		}
		if (argv[4][i]) {
			cout << "arg error! argument number4 should be pure number.\n";
			return 0;
		}
		run_gene(a, b - 1);
	}
	else if (argv[1][1] == 'e' && argv[3][1] == 'a') {
		string exe = string(argv[2]), ans = string(argv[4]);
		if (_access(exe.c_str(), 00) == -1 || _access(ans.c_str(), 00 == -1))
			cout << "adress error!\n";
		else sana_test(exe, ans);
	}
	else {
		cout << "mode error!\n"
			<< "correct arguments mode:\n"
			<< "1. project.exe -n [0,100000) -r [0,100000)\n"
			<< "2. project.exe -e <exercisefile>.txt -a <answerfile>.txt\n";
	}
	
	return 0;
}
