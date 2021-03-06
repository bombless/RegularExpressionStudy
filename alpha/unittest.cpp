﻿#include <iostream>
#include <string>
#include "RegularExpressionStudy.h"
#ifdef _DEBUG
#include <conio.h>
#endif
void expect(const char *expr, bool ex, bool ac){
	const char* e = ex ? "true" : "false";
	const char *a = ac ? "true" : "false";
	if (ex == ac){
		std::cout << "passed: " << expr << " = " << e << std::endl;
	}
	else{
		std::cout << "!!error: expect " << expr << " = " << e <<
		", get " << a << " instead." << std::endl;
	}
}
class Expect{
	bool assert;
public:
	explicit Expect(bool b) :assert(b){}
	bool GetAssert(){ return assert; }
};
class Test{
	bool assert;
	FA1 fa;
public:
	Test(const char *re, Expect e) :fa(re), assert(e.GetAssert()){
		std::cout << "Testing RE /" << re << "/ :" << std::endl;
	}
	Test& operator ,(const char*p){
		if (*p == '\0'){
			expect("<Empty String>", assert, fa << p);
		}
		else{
			expect(p, assert, fa << p);
		}
		return *this;
	}
};
class TestException{
	FA1 fa;
	std::string re;
public:
	TestException(const char *re) :fa(re), re(re){}
	TestException& operator ,(const char *p){
		try{
			fa << p;
		}
		catch (FA1::SyntaxErrorException&){
			std::cout << "passed: expected exception received for RE"  <<
			" /" << re << "/" << std::endl;
			return *this;
		}
		std::cout << "!!error: expected exception was not caught." << std::endl;
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
	Test("a*", Expect(true)), "", "a",
		"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
	Test("a*", Expect(false)), "b", "ab", "ba",
		"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaab";
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
