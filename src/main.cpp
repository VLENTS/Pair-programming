#include "Frac.h"
#include "Solve.h"

using namespace std;
typedef long long LL;

int main(int argc, char* argv[]) {
	srand(time(0));
	if (argc != 5) cout << "error!\n";
	else if (argv[1][1] == 'n' && argv[3][1] == 'r') {
		int a = 0, b = 0, i;
		i = 0;
		while ('0' <= argv[2][i] && argv[2][i] <= '9') {
			a = a * 10 + argv[2][i] - '0'; i++;
		}
		if (argv[2][i]) cout << "error!\n";
		i = 0;
		while ('0' <= argv[4][i] && argv[4][i] <= '9') {
			b = b * 10 + argv[4][i] - '0'; i++;
		}
		if (argv[4][i]) cout << "error!\n";
		run_gene(a, b - 1);
	}
	else cout << "error!\n";
	
	return 0;
}
