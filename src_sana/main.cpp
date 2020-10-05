#include "main.h"

using namespace std;

const int Width = 6;


int gcd(int a, int b) {
	return b ? gcd(b, a % b) : a;
}

bool is_operator(std::string op) {
	return (op == "+" || op == "-" || op == "*" || op == "/" || op == "(" || op == ")");
}

int get_priority(std::string op) {
	if (op == "+" || op == "-")
		return 1;
	if (op == "*")
		return 2;
	if (op == "/")
		return 3;   //当除法和乘法同时出现时，为保证结果正确，应该先除法后乘法，但是乘法可以包容。
	else return 0;
}

//转换成逆波兰
std::stack<string> transfrom(std::string input) {
	std::stack<string> converted;
	std::stack<string> op;
	size_t i = 0;
	std::string digit_node = "";

	//cout << "///transform: ///" << endl;

	while (i < input.length()) {

		char c = input[i];
		std::string read_char = "";
		read_char += c;
		if (is_operator(read_char)) {  //is_operator()用于检查符号是否为 '+', '-', '*', '/'
			if (digit_node != "") { //每次判别有符号要入栈时，都把之前所积累的运算数压入栈。
				converted.push(digit_node);
				digit_node = "";
			}
			if (op.empty()) {  //若装运算符的栈空，直接入栈
				op.push(read_char);
			}
			else {
				//此种情况为装运算符的栈不为空的情况
				if (read_char == "(")  //若读入符号是左括号，那么直接入栈
					op.push(read_char); 
				else if (read_char == ")") {  //若读入符号是右括号，需要把 与op栈内最近的左括号 之间的运算符入栈converted中。
					while (op.top() != "(") {   
						converted.push(op.top());
						op.pop();
					}
					op.pop();
				}
				else if (get_priority(read_char) >= get_priority(op.top())) //比较优先级，优先级：'*','/' > '+','-' > ')','('
					op.push(read_char);
				else {
					converted.push(op.top());
					op.pop();
					op.push(read_char);
				}
			}
			i++;
		}
		else { //若读入符号是数字
			int j = i;
			while (input[j]!=')'&&input[j] != ' '&&j<input.length()) { //input[j]!=' '--针对读取分数截止是后一个符号是空，
				digit_node += input[j];				 //input[j]!='\0' --针对读取分数时最后一个分数，此时截止情况可能不是空，是'\0'。
				j++;
			}
			////测试：：输出结果
			//cout << digit_node << " ";
			i = j;  //
			if (input[i] != ')') i++;
		}
		
	}
	////测试：：输出结果
	//cout << endl << "--以上是逆波过程中的输出--" << endl;

	if (digit_node != "") { 
		converted.push(digit_node);
	}
	while (!op.empty()) {
		converted.push(op.top());
			op.pop();
	}
	//将位置倒置一下，方便后期运算 
	stack<string> stk;
	while (!converted.empty()) {
		stk.push(converted.top());
		converted.pop();
	}
	//测试：：输出结果
	/*stack<string> _stk2=stk;
	while (!_stk2.empty()) {
		cout << _stk2.top() << endl;
		_stk2.pop();
	}
	cout << "--以上是转换后的逆波式结果--" << endl << endl;*/

	return stk;
}

//去除1. 2. ...
string getRidOf(string str) {
	int i = 0;
	while (str[i] != '.') {
		i++;
	}
	i++;
	std::string _str="";
	for (; str[i] != '\0'; i++) {
		_str += str[i];
	}
	return _str;
}

//将字符串转换成Node的类型，方便后续运算，返回值类型Node。
Node transfer(std::string val) {
	int up = 0, down = 0,A=0;
	int flag1 = -1, flag2 = -1; //分别用于记下 ' 和 / 的位置
	int i = 0;
	std::string label="000"; //我的分数情形有三种：A'B/C , B/C , C label三位值就代表着该位是否存在。
	//分别用于记下 ' 和 / 的位置
	while (i<val.length()) {
		if (val[i] == '\'') 
			flag1 = i;
		if (val[i] == '/') 
			flag2 = i;
		i++;
	}

	/*cout << "***********************" << endl;
	cout << "val= " << val << " " << "flag1= " << flag1 << " " << "flag2= " << flag2 << endl;
	cout << "***********************" << endl;*/

	i = 0;
	for (; i < val.length(); i++) {
		if (flag1 != -1&&i<flag1) {
			label[0] = 1;
			A = A * 10 + (val[i] - '0');

			//cout << "***************" << endl;
			//cout << "A= " << A << endl;
			//cout << "***************" << endl<<endl;

		}
		else if (flag2 != -1 && i > flag1 &&i < flag2) {
			label[1] = 1;
			up = up * 10 + (val[i] - '0');

			//cout << "***************" << endl;
			//cout << "up= "<< up << endl;
			//cout << "***************" << endl << endl;;
		}
		else if(i>flag2){
			label[2] = 1;
			down = down * 10 + (val[i] - '0');

		/*	cout << "***************" << endl;
			cout << "down= "<<down << endl;
			cout << "***************" << endl << endl;*/

		}
	}

	if (flag1 == -1 && flag2 == -1) {
		return Node(down, 1);
	}
	else {
		up = A * down + up;
		return Node(up, down);
	}
}

//逆波兰式子运算 返回类型string   将子运算以string类型返回
string get_result(std::string val1,std:: string val2, std::string op) {

	Node node1 = transfer(val1);
	Node node2 = transfer(val2);

	//测试：：测试结果
	cout << "***   get_result:   ***" << endl << endl;;
	cout << "node1.up= " << node1.up <<" "<< "node1.down= " << node1.down<<endl<<endl;
	cout << "node2.up= " << node2.up <<" "<< "node2.down= " << node2.down << endl<<endl;

	Node _node;
	string _revalue="";
	if (op == "+") _node = node1 + node2;
	else if (op == "-") _node = node1 - node2;
	else if (op == "*") _node = node1 * node2;
	else _node = node1 / node2;
	//测试：：测试结果
	cout << "_node.up=" << _node.up << " " << "_node.down=" << _node.down << endl<<endl;
	cout << "--以上是子运算结果--" << endl<<endl;

	//针对 up > down 的情况
	if (_node.up % _node.down) {  
		int x = _node.up / _node.down;
		_node.up -= x * _node.down;
		char A[Width];

		_itoa_s(x,A,10);
		_revalue += A;
		_revalue += '\'';

		_itoa_s(_node.up, A, 10);
		_revalue += A;
		_revalue += '/';

		_itoa_s(_node.down, A, 10);
		_revalue += A;
	}
	//针对 up = down 
	else if (_node.up / _node.down) {
		char B[Width];

		_itoa_s(_node.up, B, 10);
		_revalue += B;

	}
	//针对 up < down
	else {
		char C[Width];

		_itoa_s(_node.up, C, 10);
		_revalue += C;
		_revalue += '/';

		_itoa_s(_node.down, C, 10);
		_revalue += C;

	}
	return _revalue;
}

//运算逆波兰式 返回类型string
string rpn(std::stack<string> _stk) {
	std::stack<string> rs;
	while (!_stk.empty()) {
		if (is_operator(_stk.top())) {
			std::string val1 = rs.top();
			rs.pop();
			std::string val2 = rs.top();
			rs.pop();
			std::string op = _stk.top();
			_stk.pop();
			//测试：：测试结果
			cout << val2 << " " << op << " " << val1<<endl;
			rs.push(get_result(val2, val1, op)); //先出栈的是第二位运算数，所以传值要换个位置。
		}
		else {
			rs.push(_stk.top());
			_stk.pop();
		}
	}
	return rs.top();
}

//比较结果值
bool Compare(std::string result, std::string refer) {

	Node node1 = transfer(result);
	Node node2 = transfer(refer);

	//cout << "$$$$$$ COMPARE $$$$$$$" << endl;
	//cout << "node1:  " << node1.up << " / " << node1.down<<endl;
	//cout << "node2:  " << node2.up << " / " << node2.down<<endl;
	//cout << "$$$$$$$$$$$$$$$$$$$$" << endl << endl;

	if((node2.up==node1.up)&&(node2.down==node1.down)){
		return true;
	}
	
	return false;
}


int main(int argc, char* argv[]) {

	//文件初始化以及检查
	ifstream excfile, ansfile;
	ofstream grafile;
	std::string str[3];
	excfile.open(argv[1]);
	ansfile.open(argv[2]);
	grafile.open(argv[3],ios::app);
	if (!excfile.is_open() && !ansfile.is_open() &&!grafile.is_open()){
		cout << "FILE ERROR!!" << endl;
		system("pause");
		return 0;
	}

	//input--用于读取excercisefile的运算式， answer--用于读取answerfile的答案
	std::string input,answer;
	//number--记题号，_correct--记运算正确数，_wrong--记运算错误数
	int number = 1,_correct=0,_wrong=0;
	//容器用于装正确（错误）题号
	vector<int> correct;
	vector<int> wrong;

	while (getline(excfile, input)&& getline(ansfile, answer)) {

		//修改去除1.2.等等这样的标志，例： 1.（3+4）* 6 ---> （3+4）* 6
		input = getRidOf(input);
		answer = getRidOf(answer);
		//测试：：检查去除后的结果
	/*	cout << input << endl;
		cout << answer << endl;
		cout << "--以上是题目和答案--" << endl;*/

		std::stack<string> _stk = transfrom(input);//转换逆波兰式

		//将转换成逆波兰式的式子进行运算，并将答案以string形式传回。
		string _revalue = rpn(_stk);//算值
		////测试：：查最终值
		//cout << "运算结果得：" << endl;
		//cout << _revalue << endl;
		//cout << "--------------------------------" << endl;

		//进行比较
		if (Compare(_revalue, answer)) {
			_correct++;
			correct.push_back(number);
		}
		else {
			_wrong++;
			wrong.push_back(number);
		}
		number++;

	}

	//将校正结果写入文件
	if (correct.empty()) {
		grafile << "Correct: 0" << endl;
	}
	else {
		grafile << "Correct: " << _correct << "(";
		vector<int>::iterator iter;
		iter = correct.begin();
		grafile << *iter;
		iter++;
		for (; iter != correct.end(); iter++){
			grafile << ",";
			grafile << *iter;
		}
		grafile << ")" << endl;
	}

	if (wrong.empty()) {
		grafile << "Wrong: 0" << endl;
	}
	else {
		grafile << "Wrong: " << _wrong << "(";
		vector<int>::iterator iter;
		iter = wrong.begin();
		grafile << *iter;
		iter++;
		for (; iter != wrong.end(); iter++) {
			grafile << ",";
			grafile << *iter;
		}
		grafile << ")" << endl;
	}

	cout << "输入完成!" << endl;

	if (excfile.is_open() && ansfile.is_open() && grafile.is_open()) {
		excfile.close();
		ansfile.close();
		grafile.close();
	}
	system("pause");
	return 0;
}