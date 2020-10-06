#include "Sana.h"

using namespace std;

bool is_operator(string op) {
	return (op == "+" || op == "-" || op == "*" || op == "/" || op == "(" || op == ")");
}

int get_priority(string op) {
	if (op == "+")
		return 1;
	else if (op == "-")
		return 2;
	else if (op == "*")
		return 3;
	else if (op == "/")
		return 4;   //当除法和乘法同时出现时，为保证结果正确，应该先除法后乘法，但是乘法可以包容。
	else return 0;
}

//转换成逆波兰
stack<string> transfrom(string input) {
	stack<string> converted;
	stack<string> op;
	int i = 0;
	string digit_node = "";

	while (i < input.length()) {

		char c = input[i];
		string read_char = "";
		read_char += c;

		//扩展识别 ×和÷
		if (input[i] == (char)0xC3) {
			if (input[++i] == (char)0x97) read_char = "*";
			else read_char = "/";
		}
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
				else if (get_priority(read_char) > get_priority(op.top())) //比较优先级，优先级(高-->低)：( "(",")" ),"+","-","×","÷"
					op.push(read_char);
				else if(get_priority(read_char) == get_priority(op.top())){
					if (read_char == "-") {
						converted.push(op.top());
						op.pop();
						op.push(read_char);
					}
					else if (read_char == "/") {
						converted.push(op.top());
						op.pop();
						op.push(read_char);
					}
					else{
						op.push(read_char);
					}
				}
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
			while (input[j]!=')'&&input[j] != ' '&&j<input.length()&&input[j]!='=') { //input[j]!=' '--针对读取分数截止是后一个符号是空，
				digit_node += input[j];				 //input[j]!='\0' --针对读取分数时最后一个分数，此时截止情况可能不是空，是'\0'。
				j++;
			}
			i = j;  
			if (input[i] != ')') i++;
		}
		
	}

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

	return stk;
}

//去除1. 2. ...
string getRidOf(string str) {
	int i = 0;
	while (str[i] != '.') {
		i++;
	}
	i++;
	while (str[i] == ' ') {
		i++;
	}
	string _str="";
	for (; str[i] != '\0'; i++) {
		_str += str[i];
	}
	return _str;
}

//将字符串转换成Frac的类型，方便后续运算，返回值类型Frac。
Frac transfer(string val) {
	LL up = 0, down = 0,A=0;
	int flag1 = -1, flag2 = -1; //分别用于记下 ' 和 / 的位置
	int i = 0;
	string label="000"; //我的分数情形有三种：A'B/C , B/C , C label三位值就代表着该位是否存在。
	//分别用于记下 ' 和 / 的位置
	while (i<val.length()) {
		if (val[i] == '\'') 
			flag1 = i;
		if (val[i] == '/') 
			flag2 = i;
		i++;
	}
	i = 0;
	for (; i < val.length(); i++) {
		if (flag1 != -1&&i<flag1) {
			label[0] = 1;
			A = A * 10 + (val[i] - '0');
		}
		else if (flag2 != -1 && i > flag1 &&i < flag2) {
			label[1] = 1;
			up = up * 10 + (val[i] - '0');
		}
		else if(i>flag2){
			label[2] = 1;
			down = down * 10 + (val[i] - '0');
		}
	}

	if (flag1 == -1 && flag2 == -1) {
		return Frac(down, 1);
	}
	else {
		up = A * down + up;
		return Frac(up, down);
	}
}

//将LL类型转换成string类型
string llTostr(LL num) {
	int x;
	string s1 = "", s2 = "";
	if (num == 0) {
		s2 += '0';
		return s2;
	}
	while (num) {
		x = num % 10;
		num /= 10;
		s1 += (x + '0');
	}
	for (int i = s1.length(); i != 0; i--) {
		s2 += s1[i-1];
	}
	return s2;
}

//逆波兰式子运算 返回类型string   将子运算以string类型返回
string get_result(string val1, string val2, string op) {

	Frac node1 = transfer(val1);
	Frac node2 = transfer(val2);

	Frac _node;
	string _revalue="";
	if (op == "+") _node = node1 + node2;
	else if (op == "-") _node = node1 - node2;
	else if (op == "*") _node = node1 * node2;
	else _node = node1 / node2;

	//针对 up>down||up<down的情况
	if (_node.up % _node.down&&_node.up>_node.down) {  
		LL x = _node.up / _node.down;
		_node.up -= x * _node.down;
		_revalue += llTostr(x) + '\'' + llTostr(_node.up) + '/' + llTostr(_node.down);
	}
	else if (_node.up / _node.down) {
		_revalue+=llTostr(_node.up);
	}
	else{
		_revalue += llTostr(_node.up) + '/' + llTostr(_node.down);
	}
	
	return _revalue;
}

//运算逆波兰式 返回类型string
string rpn(stack<string> _stk) {
	stack<string> rs;
	while (!_stk.empty()) {
		if (is_operator(_stk.top())) {
			string val1 = rs.top();
			rs.pop();
			string val2 = rs.top();
			rs.pop();
			string op = _stk.top();
			_stk.pop();
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
bool Compare(string result, string refer) {

	Frac node1 = transfer(result);
	Frac node2 = transfer(refer);

	if((node2.up==node1.up)&&(node2.down==node1.down)){
		return true;
	}
	return false;
}


void sana_test(string adr_exe,string adr_ans) {

	//文件初始化以及检查
	ifstream excfile, ansfile;
	ofstream grafile;
	string str[3];
	excfile.open(adr_exe);
	ansfile.open(adr_ans);
	grafile.open(".\\Grade.txt");

	//input--用于读取excercisefile的运算式， answer--用于读取answerfile的答案
	string input,answer;
	//number--记题号，_correct--记运算正确数，_wrong--记运算错误数
	int number = 1,_correct=0,_wrong=0;
	//容器用于装正确（错误）题号
	vector<int> correct;
	vector<int> wrong;

	while (getline(excfile, input)&& getline(ansfile, answer)) {

		//修改去除1.2.等等这样的标志，例： 1.（3+4）* 6 ---> （3+4）* 6
		input = getRidOf(input);
		answer = getRidOf(answer);

		stack<string> _stk = transfrom(input);//转换逆波兰式
		//将转换成逆波兰式的式子进行运算，并将答案以string形式传回。
		string _revalue = rpn(_stk);//算值

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
		grafile << "Correct: " << _correct << " (";
		vector<int>::iterator iter;
		iter = correct.begin();
		grafile << *iter;
		iter++;
		for (; iter != correct.end(); iter++){
			grafile << ", ";
			grafile << *iter;
		}
		grafile << ")" << endl;
	}

	if (wrong.empty()) {
		grafile << "Wrong: 0" << endl;
	}
	else {
		grafile << "Wrong: " << _wrong << " (";
		vector<int>::iterator iter;
		iter = wrong.begin();
		grafile << *iter;
		iter++;
		for (; iter != wrong.end(); iter++) {
			grafile << ", ";
			grafile << *iter;
		}
		grafile << ")" << endl;
	}

	excfile.close();
	ansfile.close();
	grafile.close();
}