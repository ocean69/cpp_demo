// cpp_demo.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <string>
#include <iostream>

namespace NameA
{
	
}

namespace NameB = NameA;

class B {
public:
	B()
	{
		printf("B Constructor\n");
	}
	~B()
	{
		printf("B Destructor\n");
	}
};

class C {
	std::string obj1;
	B b;
	char* leak;
public:
	C()
	{
		leak = new char[10000];
		throw "bad construct";
	}
	~C()
	{
		delete[] leak;
		printf("C destroctor\n");
	}
};

void demo1_exception_in_constructor()
{
	C *c = nullptr;
	try {
		c = new C();
	} catch (...) {
		printf("pointer of object after exception in constructor:%x\n", c);
	}
	_CrtDumpMemoryLeaks();
	/*
	Detected memory leaks!
	Dumping objects -> {159} normal block at 0x006322E8, 10000 bytes long.
	Data: <                > CD CD CD CD CD CD CD CD CD CD CD CD CD CD CD CD
	Object dump complete.
	*/
}
#include <vector>
void list_initialization()
{
	using namespace std;
	
	int b = 1.2l;
	//int a{ 1.2l }; //compile error
	//short c{ b }; //compile error

	vector<int> v{ 1, 2, 3, 4, 5, 6 };
	auto v1 = { 1, 2, 3, 4, 5, 6 };
	
	cout << typeid(v1).name() << endl;
}

constexpr int constexpr_function_get_value()
{

	return 3 * 3;
}

void const_expression_bad_example()
{
	using namespace  std;
	constexpr int value = constexpr_function_get_value();
	int build_in_array[value];
	cout << "array length: " << sizeof(build_in_array) / sizeof(build_in_array[0]) << endl;
	switch (1) {
	case value:
		cout << value << endl;
		break;
	default:
		cout << "default" << endl;
	}
}

int main()
{
	demo1_exception_in_constructor();
	list_initialization();
	const_expression_bad_example();
    return 0;
}

