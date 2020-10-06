#ifndef MAIN_H
#define MAIN_H
#include<iostream>
#include<fstream>
#include<io.h>
#include<string>
#include<stack>
#include<sstream>
#include<vector>
  
typedef long long LL;

LL gcd(LL a, LL b);
struct Node {
	LL up, down;
	//�޲ι��� ���ڴ�������
	Node() {
		up = down = 0;
	}
	//���ι��� �Զ�Լ��
	Node(LL x, LL y) {
		LL g = gcd(x, y);
		up = x / g; 
		down = y / g;
	}
	//�����ӷ�
	Node operator + (const Node& x) const {
		LL _up = up * x.down + x.up * down;
		LL _down = down * x.down;
		LL g = gcd(_up, _down);
		return Node( _up / g,_down / g );
	}
	//��������
	Node operator - (const Node& x) const {
		LL _up = up * x.down - x.up * down;
		LL _down = down * x.down;
		LL g = gcd(_up, _down);
		return Node(_up / g,_down / g );
	}
	//�����˷�
	Node operator * (const Node& x) const {
		LL _up = up * x.up;
		LL _down = down * x.down;
		LL g = gcd(_up, _down);
		return Node(_up / g,_down / g );
	}
	//��������
	Node operator / (const Node& x) const {
		LL _up = up * x.down;
		LL _down = down * x.up;
		LL g = gcd(_up, _down);
		return Node( _up / g,_down / g );
	}
};

bool is_operator(std::string op);
int get_priority(std::string op);
std::stack<std::string> transfrom(std::string input);
std::string getRidOf(std::string str);
Node transfer(std::string val);
std::string get_result(std::string val1, std::string val2, std::string op);
std::string rpn(std::stack<std::string> _stk);
bool Compare(std::string result, std::string refer);

#endif // !MAIN_H

