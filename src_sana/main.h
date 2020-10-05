#ifndef MAIN_H
#define MAIN_H
#include<iostream>
#include<fstream>
#include<io.h>
#include<string>
#include<stack>
#include<sstream>
#include<stdlib.h>
#include<vector>



int gcd(int a, int b);
struct Node {
	int up, down;
	//无参构造 用于创建数组
	Node() {
		up = down = 0;
	}
	//含参构造 自动约分
	Node(int x, int y) {
		int g = gcd(x, y);
		up = x / g; 
		down = y / g;
	}
	//分数加法
	Node operator + (const Node& x) const {
		int _up = up * x.down + x.up * down;
		int _down = down * x.down;
		int g = gcd(_up, _down);
		return Node( _up / g,_down / g );
	}
	//分数减法
	Node operator - (const Node& x) const {
		int _up = up * x.down - x.up * down;
		int _down = down * x.down;
		int g = gcd(_up, _down);
		return Node(_up / g,_down / g );
	}
	//分数乘法
	Node operator * (const Node& x) const {
		int _up = up * x.up;
		int _down = down * x.down;
		int g = gcd(_up, _down);
		return Node(_up / g,_down / g );
	}
	//分数除法
	Node operator / (const Node& x) const {
		int _up = up * x.down;
		int _down = down * x.up;
		int g = gcd(_up, _down);
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

