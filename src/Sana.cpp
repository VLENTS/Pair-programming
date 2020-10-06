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
		return 4;   //�������ͳ˷�ͬʱ����ʱ��Ϊ��֤�����ȷ��Ӧ���ȳ�����˷������ǳ˷����԰��ݡ�
	else return 0;
}

//ת�����沨��
stack<string> transfrom(string input) {
	stack<string> converted;
	stack<string> op;
	int i = 0;
	string digit_node = "";

	while (i < input.length()) {

		char c = input[i];
		string read_char = "";
		read_char += c;

		//��չʶ�� ���͡�
		if (input[i] == (char)0xC3) {
			if (input[++i] == (char)0x97) read_char = "*";
			else read_char = "/";
		}
		if (is_operator(read_char)) {  //is_operator()���ڼ������Ƿ�Ϊ '+', '-', '*', '/'
			if (digit_node != "") { //ÿ���б��з���Ҫ��ջʱ������֮ǰ�����۵�������ѹ��ջ��
				converted.push(digit_node);
				digit_node = "";
			}
			if (op.empty()) {  //��װ�������ջ�գ�ֱ����ջ
				op.push(read_char);
			}
			else {
				//�������Ϊװ�������ջ��Ϊ�յ����
				if (read_char == "(")  //����������������ţ���ôֱ����ջ
					op.push(read_char); 
				else if (read_char == ")") {  //����������������ţ���Ҫ�� ��opջ������������� ֮����������ջconverted�С�
					while (op.top() != "(") {   
						converted.push(op.top());
						op.pop();
					}
					op.pop();
				}
				else if (get_priority(read_char) > get_priority(op.top())) //�Ƚ����ȼ������ȼ�(��-->��)��( "(",")" ),"+","-","��","��"
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
		else { //���������������
			int j = i;
			while (input[j]!=')'&&input[j] != ' '&&j<input.length()&&input[j]!='=') { //input[j]!=' '--��Զ�ȡ������ֹ�Ǻ�һ�������ǿգ�
				digit_node += input[j];				 //input[j]!='\0' --��Զ�ȡ����ʱ���һ����������ʱ��ֹ������ܲ��ǿգ���'\0'��
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
	//��λ�õ���һ�£������������ 
	stack<string> stk;
	while (!converted.empty()) {
		stk.push(converted.top());
		converted.pop();
	}

	return stk;
}

//ȥ��1. 2. ...
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

//���ַ���ת����Frac�����ͣ�����������㣬����ֵ����Frac��
Frac transfer(string val) {
	LL up = 0, down = 0,A=0;
	int flag1 = -1, flag2 = -1; //�ֱ����ڼ��� ' �� / ��λ��
	int i = 0;
	string label="000"; //�ҵķ������������֣�A'B/C , B/C , C label��λֵ�ʹ����Ÿ�λ�Ƿ���ڡ�
	//�ֱ����ڼ��� ' �� / ��λ��
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

//��LL����ת����string����
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

//�沨��ʽ������ ��������string   ����������string���ͷ���
string get_result(string val1, string val2, string op) {

	Frac node1 = transfer(val1);
	Frac node2 = transfer(val2);

	Frac _node;
	string _revalue="";
	if (op == "+") _node = node1 + node2;
	else if (op == "-") _node = node1 - node2;
	else if (op == "*") _node = node1 * node2;
	else _node = node1 / node2;

	//��� up>down||up<down�����
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

//�����沨��ʽ ��������string
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
			rs.push(get_result(val2, val1, op)); //�ȳ�ջ���ǵڶ�λ�����������Դ�ֵҪ����λ�á�
		}
		else {
			rs.push(_stk.top());
			_stk.pop();
		}
	}
	return rs.top();
}

//�ȽϽ��ֵ
bool Compare(string result, string refer) {

	Frac node1 = transfer(result);
	Frac node2 = transfer(refer);

	if((node2.up==node1.up)&&(node2.down==node1.down)){
		return true;
	}
	return false;
}


void sana_test(string adr_exe,string adr_ans) {

	//�ļ���ʼ���Լ����
	ifstream excfile, ansfile;
	ofstream grafile;
	string str[3];
	excfile.open(adr_exe);
	ansfile.open(adr_ans);
	grafile.open(".\\Grade.txt");

	//input--���ڶ�ȡexcercisefile������ʽ�� answer--���ڶ�ȡanswerfile�Ĵ�
	string input,answer;
	//number--����ţ�_correct--��������ȷ����_wrong--�����������
	int number = 1,_correct=0,_wrong=0;
	//��������װ��ȷ���������
	vector<int> correct;
	vector<int> wrong;

	while (getline(excfile, input)&& getline(ansfile, answer)) {

		//�޸�ȥ��1.2.�ȵ������ı�־������ 1.��3+4��* 6 ---> ��3+4��* 6
		input = getRidOf(input);
		answer = getRidOf(answer);

		stack<string> _stk = transfrom(input);//ת���沨��ʽ
		//��ת�����沨��ʽ��ʽ�ӽ������㣬��������string��ʽ���ء�
		string _revalue = rpn(_stk);//��ֵ

		//���бȽ�
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

	//��У�����д���ļ�
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