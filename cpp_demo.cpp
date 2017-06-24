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
	//编译错误 error C2338: size of int no more than 4
	//static_assert(sizeof(int) > 4, "size of int no more than 4");

	//编译错误 error C2131: expression did not evaluate to a constant
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

#include <thread>
#include <mutex>

std::mutex m;
unsigned i = 0;

class ThreadFuncObject {
	unsigned thread_index_;
public:
	ThreadFuncObject(unsigned thread_index) : thread_index_{thread_index} {}
	void operator() ()
	{
		unique_lock<mutex> lck{ m };
		cout << "index: " << thread_index_ << endl;
		cout << "id: " << this_thread::get_id() << endl;
		cout << ++i << endl;
	}
};


void cpp_sleep(unsigned time_in_seconds)
{
	this_thread::sleep_for(std::chrono::seconds{ time_in_seconds });
}

void thread_func(unsigned thread_index)
{
	unique_lock<mutex> lck{ m };
	//下面这句cout 和 i 都是多线程不安全的资源
	cout << "index: " << thread_index << endl;
	cout << "id: " << this_thread::get_id() << endl;
	cout << ++i << endl;
}

void thread_example()
{
	unsigned index = 0;
	thread t1{ thread_func, index++ };
	thread t2{ ThreadFuncObject{ index++} };

	t1.join();
	t2.join();
}

mutex m1, m2, m3;

void multi_lock_example()
{
	unique_lock<mutex> lck1{ m1, defer_lock };
	unique_lock<mutex> lck2{ m2, defer_lock };
	unique_lock<mutex> lck3{ m3, defer_lock };

	lock(m1, m2, m3); //可以看作 同时获取三个锁 或者一个都不获取

}

#include <condition_variable>

condition_variable condition;
mutex m4;

void worker()
{
	unsigned i = 0;
	//while (i < 3) {
	unique_lock<mutex> lck{ m4 };
		
	condition.wait(lck); //释放 lck 并等待条件变量
							//被唤醒后会重新获取lck

	cout << "worker" << endl;
	//}
	cout << "thread worker end" << endl;
}

void notify()
{
	unique_lock<mutex> lck{ m4 };
	cout << "input any key to send condition event" << endl;
	char ch;
	getchar();
	condition.notify_one(); //唤醒worker
	//返回后lck也被释放
}

void condition_variable_example()
{
	thread t_worker{ worker };

	notify();

	t_worker.join();
	cout << "condition_variable_example end" << endl;
}

void cpp_sleep_example()
{
	cout << "wait..." << endl;
	auto t1 = chrono::high_resolution_clock::now();
	cpp_sleep(5);
	auto t2 = chrono::high_resolution_clock::now();
	cout << chrono::duration_cast<chrono::seconds>(t2 - t1).count() << "s" << endl;
}

#include <future>

void get_from_future(future<unsigned>& f)
{
	try {
		unsigned value = f.get();
		cout << "get: " << value << endl;
	} catch(exception ep) {
		cout << ep.what() << endl;
	}
}

void set_promise(promise<unsigned>& p)
{
	try {
		//...
		cout << "input a number:" << ends;
		unsigned value;
		cin >> value;
		p.set_value(value);
	} catch(...) {
		p.set_exception(current_exception());
	}
}

void future_promise_example()
{
	promise<unsigned> p;
	thread t_future{ get_from_future , p.get_future()};
	thread t_promise{ set_promise , std::move(p) };

	t_future.join();
	t_promise.join();
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
	thread_example();
	multi_lock_example();
	cpp_sleep_example();
	condition_variable_example();
	future_promise_example();
    return 0;
}

