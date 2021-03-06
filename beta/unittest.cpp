﻿#include <iostream>
#include <string>
#include "RegularExpressionStudy.h"
#ifdef _DEBUG
#include <conio.h>
#endif
#if defined(_MSC_VER) && (_M_IX86 >= 500 || defined(_M_AMD64)) 
#include <intrin.h>
#pragma intrinsic(__rdtsc)
#define rdtsc __rdtsc
typedef unsigned __int64 rdtsc_t;
#else
#include <ctime>
#endif
bool expect(const char *expr, bool ex, bool ac){
	const char* e = ex ? "true" : "false";
	const char *a = ac ? "true" : "false";
	if (ex == ac){
		std::cout << "passed: " << expr << " = " << e << std::endl;
	}
	else{
		std::cout << "!!error: expect " << expr << " = " << e
		<< ", get " << a << " instead." << std::endl;
	}
	return ex == ac;
}
class TestException{
	FA2 fa;
	std::string re;
public:
	TestException(const char *re) :fa(re), re(re){}
	TestException& operator ,(const char *p){
		using namespace std;
		try{
			fa << p;
		}
		catch (FA1::SyntaxErrorException&){
			cout << "passed:"
			" expected exception received for RE /" << re << "/" << endl;
			return *this;
		}
		cout << "!!error: expected exception was not caught." << endl;
		return *this;
	}
};
class Expect{
	bool assert;
public:
	explicit Expect(bool b) :assert(b){}
	bool GetAssert(){ return assert; }
};
class Test{
	bool assert;
	FA1 faCompare;
	FA2 fa;
public:
	Test(const char *re, Expect e)
 		:fa(re), faCompare(re), assert(e.GetAssert()){
		std::cout << "Testing RE /" << re << "/ :" << std::endl;
	}
	Test& operator ,(const char*p){
		using namespace std;
		bool passed;
		if (*p == '\0'){
			passed = expect("<Empty String>", assert, fa << p);
		}
		else{
			passed = expect(p, assert, fa << p);
		}
		if(!passed)return *this;
#ifdef rdtsc
		unsigned __int64 start;
		start = rdtsc();
		for (int i = 0; i < 9999; ++i){
			faCompare << p;
		}
		rdtsc_t time1 = rdtsc() - start;
		cout << "nfa took " << time1
			<< " clock cycles to calculate for 9999 times" << endl;
		start = rdtsc();
		for (int i = 0; i < 9999; ++i){
			fa << p;
		}
		rdtsc_t time2 = rdtsc() - start;
		cout << "dfa took " << time2 <<
		" clock cycles to calculate for 9999 times" << endl;
		cout << "factor: " << time1 / time2 << endl;
#else
		time_t tm;
		tm = time(0);
		for (int i = 0; i < 9999999; ++i){
			faCompare << p;
		}
		cout << "nfa took " << (time(0) - tm)
		<< " seconds to calculate for " <<
		9999999 << " times" << endl;
		tm = time(0);
		for (int i = 0; i < 9999999; ++i){
			fa << p;
		}
		cout << "dfa took " << (time(0) - tm) <<
		" seconds to calculate for " <<
		9999999 << " times" << endl;
#endif
		return *this;
	}
};
int main(){
	Test("", Expect(true)), "";
	Test("", Expect(false)), "a";
	Test("a", Expect(true)), "a";
	Test("a", Expect(false)), "b", "abc", "";
	Test("ab", Expect(true)), "ab";
	Test("ab", Expect(false)), "", "a", "b", "c";
	Test("a|b", Expect(true)), "a", "b";
	Test("a|b", Expect(false)), "abc", "ab", "";
	Test("ab|c", Expect(true)), "ab", "c";
	Test("ab|c", Expect(false)), "a", "b", "abc";
	Test("ab|a", Expect(true)), "ab", "a";
	Test("ab|a", Expect(false)), "b", "c", "abc";
	Test("a|bc", Expect(true)), "a", "bc";
	Test("a|bc", Expect(false)), "ab", "c", "";
	Test("a*", Expect(true)), "", "a", "aaa";
	Test("a*", Expect(false)), "b", "ab", "ba";
	Test("ab*", Expect(true)), "a", "abb";
	Test("ab*", Expect(false)), "b", "bb", "bc", "abc", "";
	Test("a*b", Expect(true)), "b", "aaab", "ab";
	Test("a*b", Expect(false)), "aa", "ac", "";
	Test("a*ab", Expect(true)), "ab", "aab", "aaaab";
	Test("a*ab", Expect(false)), "a", "b", "c";
	Test("aa*b", Expect(true)), "ab", "aab", "aaaab";
	Test("aa*b", Expect(false)), "a", "b", "c";
	Test("a*|b", Expect(true)), "aaa", "b", "";
	Test("a*|b", Expect(false)), "abc", "ba", "cb", "ab";
	Test("a|b*", Expect(true)), "a", "bb", "";
	Test("a|b*", Expect(false)), "aa", "ab", "bc";
	Test("a|b|c", Expect(true)), "a", "b", "c";
	Test("a|b|c", Expect(false)), "ab", "d", "";
	Test("a|", Expect(true)), "a", "";
	Test("a|", Expect(false)), "b", "aa";
	Test("|a", Expect(true)), "a", "";
	Test("|a", Expect(false)), "b";
	Test("||", Expect(true)), "";
	Test("||", Expect(false)), "a";
	TestException("*"), "", "a", "b";
	TestException("|*"), "", "a", "b";
	TestException("**"), "", "a", "b";
	TestException("*|*"), "", "a", "b";
	TestException("*||*"), "", "a", "b";
	TestException("||*"), "", "a", "b";
#ifdef _DEBUG
#ifdef _MSC_VER
	_CrtCheckMemory();
	_CrtDumpMemoryLeaks();
#endif
	_getch();
#endif
	return 0;
}
