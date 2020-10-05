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
		return 3;   //�������ͳ˷�ͬʱ����ʱ��Ϊ��֤�����ȷ��Ӧ���ȳ�����˷������ǳ˷����԰��ݡ�
	else return 0;
}

//ת�����沨��
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
				else if (get_priority(read_char) >= get_priority(op.top())) //�Ƚ����ȼ������ȼ���'*','/' > '+','-' > ')','('
					op.push(read_char);
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
			while (input[j]!=')'&&input[j] != ' '&&j<input.length()) { //input[j]!=' '--��Զ�ȡ������ֹ�Ǻ�һ�������ǿգ�
				digit_node += input[j];				 //input[j]!='\0' --��Զ�ȡ����ʱ���һ����������ʱ��ֹ������ܲ��ǿգ���'\0'��
				j++;
			}
			////���ԣ���������
			//cout << digit_node << " ";
			i = j;  //
			if (input[i] != ')') i++;
		}
		
	}
	////���ԣ���������
	//cout << endl << "--�������沨�����е����--" << endl;

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
	//���ԣ���������
	/*stack<string> _stk2=stk;
	while (!_stk2.empty()) {
		cout << _stk2.top() << endl;
		_stk2.pop();
	}
	cout << "--������ת������沨ʽ���--" << endl << endl;*/

	return stk;
}

//ȥ��1. 2. ...
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

//���ַ���ת����Node�����ͣ�����������㣬����ֵ����Node��
Node transfer(std::string val) {
	int up = 0, down = 0,A=0;
	int flag1 = -1, flag2 = -1; //�ֱ����ڼ��� ' �� / ��λ��
	int i = 0;
	std::string label="000"; //�ҵķ������������֣�A'B/C , B/C , C label��λֵ�ʹ����Ÿ�λ�Ƿ���ڡ�
	//�ֱ����ڼ��� ' �� / ��λ��
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

//�沨��ʽ������ ��������string   ����������string���ͷ���
string get_result(std::string val1,std:: string val2, std::string op) {

	Node node1 = transfer(val1);
	Node node2 = transfer(val2);

	//���ԣ������Խ��
	cout << "***   get_result:   ***" << endl << endl;;
	cout << "node1.up= " << node1.up <<" "<< "node1.down= " << node1.down<<endl<<endl;
	cout << "node2.up= " << node2.up <<" "<< "node2.down= " << node2.down << endl<<endl;

	Node _node;
	string _revalue="";
	if (op == "+") _node = node1 + node2;
	else if (op == "-") _node = node1 - node2;
	else if (op == "*") _node = node1 * node2;
	else _node = node1 / node2;
	//���ԣ������Խ��
	cout << "_node.up=" << _node.up << " " << "_node.down=" << _node.down << endl<<endl;
	cout << "--��������������--" << endl<<endl;

	//��� up > down �����
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
	//��� up = down 
	else if (_node.up / _node.down) {
		char B[Width];

		_itoa_s(_node.up, B, 10);
		_revalue += B;

	}
	//��� up < down
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

//�����沨��ʽ ��������string
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
			//���ԣ������Խ��
			cout << val2 << " " << op << " " << val1<<endl;
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

	//�ļ���ʼ���Լ����
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

	//input--���ڶ�ȡexcercisefile������ʽ�� answer--���ڶ�ȡanswerfile�Ĵ�
	std::string input,answer;
	//number--����ţ�_correct--��������ȷ����_wrong--�����������
	int number = 1,_correct=0,_wrong=0;
	//��������װ��ȷ���������
	vector<int> correct;
	vector<int> wrong;

	while (getline(excfile, input)&& getline(ansfile, answer)) {

		//�޸�ȥ��1.2.�ȵ������ı�־������ 1.��3+4��* 6 ---> ��3+4��* 6
		input = getRidOf(input);
		answer = getRidOf(answer);
		//���ԣ������ȥ����Ľ��
	/*	cout << input << endl;
		cout << answer << endl;
		cout << "--��������Ŀ�ʹ�--" << endl;*/

		std::stack<string> _stk = transfrom(input);//ת���沨��ʽ

		//��ת�����沨��ʽ��ʽ�ӽ������㣬��������string��ʽ���ء�
		string _revalue = rpn(_stk);//��ֵ
		////���ԣ���������ֵ
		//cout << "�������ã�" << endl;
		//cout << _revalue << endl;
		//cout << "--------------------------------" << endl;

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

	cout << "�������!" << endl;

	if (excfile.is_open() && ansfile.is_open() && grafile.is_open()) {
		excfile.close();
		ansfile.close();
		grafile.close();
	}
	system("pause");
	return 0;
}