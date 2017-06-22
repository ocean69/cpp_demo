// cpp_demo.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <string>
#include <iostream>

using namespace  std;

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
	constexpr int value = constexpr_function_get_value();
	int build_in_array[value];
	cout << "array length: " << sizeof(build_in_array) / sizeof(build_in_array[0]) << endl;
	switch (1) {
	case value:
		cout << "constexpr can be used in case statement" << endl;
		break;
	default:
		cout << "default" << endl;
	}
}

void range_for()
{
	const string space{ " " };

	int x[10]{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	for (int i : x) {
		cout <<  i << space << ends;
	}
	cout << endl;

	for (int& i : x) {
		++i;
	}

	for (int i : x) {
		cout << i << space << ends;
	}
	cout << endl;

	for (auto i : { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 }) {
		cout << i << space << ends;
	}
	cout << endl;
}



void enum_class_example()
{
	enum class enum_class {
		value1,
		value2,
		value3
	};

	enum class enum_class2 {
		value1,
		value2,
		value3
	};

	enum old_enum {
		value1,
		value2,
		value3
	};
	
	//enum old_enum2 {
	//value1, //error c2365, 'value1': redefinition
	//value2,
	//value3
	//};

	enum_class enum_class_value = enum_class::value1;
	int a = value2; // old enum 
	//int b = enum_class::value2; //enum class is a new type can't assign to int
}

template <bool>
class vector_base_common {
	vector_base_common() {}
	void __throw_length_error() const;
	void __throw_out_of_range() const;
};
template <bool __b>
void
vector_base_common<__b>::__throw_length_error() const
{
	_VSTD::__throw_length_error("vector");
}

template <bool __b>
void
vector_base_common<__b>::__throw_out_of_range() const
{
	_VSTD::__throw_out_of_range("vector");
}

void static_assert_example()
{
	static_assert(sizeof(int) == 4, "size of int is not 4");
	//������� error C2338: size of int no more than 4
	//static_assert(sizeof(int) > 4, "size of int no more than 4");

	//������� error C2131: expression did not evaluate to a constant
	//int a{ 0 };
	//static_assert(a == 0, "error C2131: expression did not evaluate to a constant");
}

template<typename FIRST, typename... REMAIN>
void output(FIRST first, REMAIN... remain)
{
	cout << first << " " << ends;
	output(remain...);
}
void output() { cout << endl; }

void variable_template_example()
{
	output("a", 1, 0.2);
}


int main()
{
	demo1_exception_in_constructor();
	list_initialization();
	const_expression_bad_example();
	range_for();
	enum_class_example();
	static_assert_example();
	variable_template_example();
    return 0;
}

