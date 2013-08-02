
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//////          Copyright 2013, Cai Bowen
//////              All rights reserved.
//////
//////  Author: Cai Bowen
//////      contact/bug report/get new version
 /////           at
 /////       feedback2bowen@outlook.com
//////           or
//////      https://github.com/xkommando/convert_and_bwtest
//////
//////          last modified : 2013.3.12
//////
//////
////// Redistribution and use in source and binary forms, with or without
////// modification, are permitted provided that the following conditions are
////// met:
//////
//////     * Redistributions of source code must retain the above copyright
////// notice, this list of conditions and the following disclaimer.
//////     * Redistributions in binary form must reproduce the above
////// copyright notice, this list of conditions and the following disclaimer
////// in the documentation and/or other materials provided with the
////// distribution.
//////     * Neither the name of Cai Bowen. nor the names of its
////// contributors may be used to endorse or promote products derived from
////// this software without specific prior written permission.
//////
////// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
////// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
////// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
////// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
////// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
////// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
////// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
////// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
////// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
////// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
////// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//////
////// Acknowledgment: this work borrowed the idea of Google Test
//////      The Google C++ Testing Framework (Google Test)
//////
////// Compatibility:
//////      BWTest_Names has been tested on:
//////      GCC 4.7.2 on 64-bit MS Windows and 64-bit Linux
//////      MS Virtual C++ 11(_MSC_VER 1700) on 64-bit Windows
//////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

//    C++11 features maybe needed:
//        chrono for high resolution clock
//        delete default constructors and assign operator
//        nullptr
//         soped enumeration
//          ...


#ifndef _BOWEN_TEST_H_
#define _BOWEN_TEST_H_

#ifdef __GNUC__
    #if __GNUC__*100 + __GNUC_MINOR__*10 + __GNUC_PATCHLEVEL__ > 470
        #define __BWTEST_HAS_ENOUGH_CXX11__
    #endif
    #if __GNUC__*100 + __GNUC_MINOR__*10 + __GNUC_PATCHLEVEL__ < 400
        #error "please up data your complier to GCC 4.0.0 and above"
    #endif

#else
#ifdef _MSC_VER
    #if _MSC_VER > 1700
        #define __BWTEST_HAS_ENOUGH_CXX11__
    #endif
    #if _MSC_VER < 1300
        #error "please up data your complier to VC 7.0 and above"
    #endif
#endif
#endif

//////////////////////////////////////////////////////////////////////////////////////////////
                    #define __BWTEST_HAS_ENOUGH_CXX11__
//////////////////////////////////////////////////////////////////////////////////////////////


#include <iostream>
#include <fstream>
#include <stdexcept>
#include <map>
#include <string>
#include <vector>
#include <numeric>
#include <limits>

#include <cstdlib>
#include <cstddef>
#include <cmath>
#include <cassert>
#include <cstring>

#ifdef __BWTEST_HAS_ENOUGH_CXX11__
#include <chrono>
#else
#include <ctime>
#endif



// this function help the compiler deduce type, and do the range check.
    template<class T, class E>
    void BWTEST_FUNC_RANGE_CHEACK(T val, E, const char* funcname, const char* line);





#undef RANGE_CHEACK
#define RANGE_CHEACK(val, TypeName)\
        BWTEST_FUNC_RANGE_CHEACK(val, TypeName(), __FUNCTION__, __LINE__);

#undef assert_throw
#define assert_throw(statement, expected_except)\
        if(true){\
            bool caught = false;\
            try{statement;}\
            catch(expected_except const& e){caught = true;}\
            catch(...){}\
            assert(caught);\
        }


#undef assert_throw_any
#define assert_throw_any(statement)\
        if(true){\
            bool caught = false;\
            try{statement;}\
            catch(...){caught = true;}\
            assert(caught);\
        }

#undef assert_eq
#define assert_eq(x, y)\
        assert(fabs(x - y) <= 0.00001);

#undef assert_nq
#define assert_nq(x, y)\
    assert(fabs(x - y) > 0.00001);

#undef assert_eq_at
#define assert_eq_at(x, y, z)\
    assert(fabs(x - y) < z);



#ifdef __BWTEST_HAS_ENOUGH_CXX11__
#define __BW_NOEXCEPT noexcept
#define __BW_NULL_PTR nullptr
#else
#define __BW_NOEXCEPT throw()
#define __BW_NULL_PTR 0
#endif

namespace bwtest
{

    enum BWTestOutputType{
        stdCout,
        stdCerr,
        stdClog,
        file
    };

    static BWTestOutputType outputStream = stdCerr;
    static char* _fileName  =__BW_NULL_PTR;


    void setOutput(const char* c_str);

    std::ostream& getOutputStream();


    void cleanOutputStream();

} // namespace bwtest



#define put_out bwtest::getOutputStream()


#define OUT_ONCE(obj)

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////  enable printing //////////////////////////////
//////////////////      in looping functions and testCase.excute() ///////////////
                                #define DO_PRINT
///////////////////////////////////////////////////////////////////////////////
#ifdef DO_PRINT
#undef OUT_ONCE
#define OUT_ONCE(obj) ::bwtest::getOutputStream() << obj

#endif

#undef LOOP_FUNC
#define LOOP_FUNC(func, loops)\
            if(true) {\
                size_t std_except_caught = 0;\
                size_t unknown_except_caught = 0;\
                for(size_t i = 0; i != loops; ++i ) {\
                    try{\
                    func();\
                    }catch(std::exception& e) {\
                        std_except_caught++;\
                        OUT_ONCE("\n\t caught std exception");\
                        OUT_ONCE("\n");\
                        OUT_ONCE(e.what());\
                        OUT_ONCE("\n");\
                    }\
                    catch(...) {\
                        unknown_except_caught++;\
                        OUT_ONCE("\n\t caught unknown exception");\
                        OUT_ONCE("\n");\
                    }\
                }\
                OUT_ONCE("\n\t Totally caught: ");\
                OUT_ONCE(std_except_caught + unknown_except_caught);\
                OUT_ONCE("\n\t std exception : ");\
                OUT_ONCE(std_except_caught);\
                OUT_ONCE("\n\tunknown exception: ");\
                OUT_ONCE(unknown_except_caught);\
                OUT_ONCE("\n");\
            }

namespace BWTest_Names
{
//// multi-type, multi-arguments printing
////    e.g.
//// print(std::string("sdfds"), 32234, 546.546, "sdgdf", 'c');
////
#ifdef __BWTEST_HAS_ENOUGH_CXX11__
void print();

template<typename T, typename... Ts>
void print(const T& obj, const Ts&... others);
#endif


class TestBase
{
    private:
        const char* name_testgroup;
        const char* name_testcase_;
        size_t num_excutions_;
        size_t num_std_except_;
        size_t num_other_except_;
        unsigned long int time_elapsed_;// duration, in second.

    virtual void testBody() = 0;

#ifdef __BWTEST_HAS_ENOUGH_CXX11__
        TestBase() = delete;
        TestBase(const TestBase&) = delete;
        TestBase(TestBase&&) = delete;
        TestBase& operator= (const TestBase& ) = delete;
#else
    private:
        TestBase(); // no definition
        TestBase(const TestBase&);
        TestBase& operator= (const TestBase& );
#endif

  public:
        explicit
        TestBase(size_t tmies, const char* testcase, const char* testgroup = "default")
        :   name_testgroup(testgroup),
            name_testcase_(testcase),
            num_excutions_(tmies),
            num_std_except_(0),
            num_other_except_(0),
            time_elapsed_(0)  {}

        virtual
        ~TestBase() __BW_NOEXCEPT   {}


#ifdef __BWTEST_HAS_ENOUGH_CXX11__
		void excute() {
			try {
				auto t_start = std::chrono::high_resolution_clock::now();
				for(size_t i = 0; i < num_excutions_; ++i) {
					try {
						testBody();
					}
					catch(std::exception& e) {
						num_std_except_++;
                        OUT_ONCE("\n>>>");
						OUT_ONCE(name_testcase_);
						OUT_ONCE("       caught std exception:\n>>> ");
						OUT_ONCE(e.what());
					}
					catch(...) {
						num_other_except_++;
                        OUT_ONCE("\n>>>");
						OUT_ONCE(name_testcase_);
						OUT_ONCE("       caught other exception");
					}
				}
				auto t_end = std::chrono::high_resolution_clock::now();
				this->time_elapsed_ = std::chrono::duration_cast<std::chrono::seconds>(t_end - t_start).count();
			}
			catch(...) {
			    OUT_ONCE("\n>>> ");
			    OUT_ONCE(name_testcase_);
                OUT_ONCE("  testBody: caught unknown exception. \n");
			}
		}
#else
		void excute() {
			try {
				std::clock_t c_start = std::clock();
				for(size_t i = 0; i != _excutionOfTimes; ++i) {
					try {
						testBody();
					} catch(std::exception& e) {
						num_std_except_++;
                        OUT_ONCE("\n>>>");
						OUT_ONCE(name_testcase_);
						OUT_ONCE("       caught std exception:\n>>> ");
						OUT_ONCE(e.what());
					} catch(...) {
						num_other_except_++;
                        OUT_ONCE("\n>>>");
						OUT_ONCE(name_testcase_);
						OUT_ONCE("       caught other exception");
					}
				}
				std::clock_t c_end = std::clock();
				this->time_elapsed_ = (c_end - c_start) / CLOCKS_PER_SEC;
			}
			catch(...) {
			    OUT_ONCE("\n>>> ");
			    OUT_ONCE(name_testcase_);
                OUT_ONCE("  testBody: caught unknown exception. \n");
			}
		}
#endif

        void printReport() const __BW_NOEXCEPT {
            ::bwtest::getOutputStream() << "\n>>>  test\"" << name_testcase_
            << "\" excuted " << num_excutions_ << "times."
            << "\n>>>  time passed: " << this->time_elapsed_
            << "s\n>>>  caught std exception " << num_std_except_ << " times."
            << "\n>>>  caught other exception " << num_other_except_ << " times." << std::endl;
        }
};

class TestRegister
{
    private:
        TestRegister(){}
#ifdef __BWTEST_HAS_ENOUGH_CXX11__
        TestRegister(const TestRegister& ) = delete;
        TestRegister(TestRegister&&) = delete;
        TestRegister&
        operator= (const TestRegister&)=delete;
        TestRegister&
        operator= (TestRegister&&)=delete;
#else
    private:
        TestRegister(const TestRegister& ){}
        TestRegister&
        operator= (const TestRegister&){return *this;}
#endif

    private:
//    public:
        typedef std::vector<TestBase*>          Group;
        typedef std::map<std::string, Group>    GroupMap;
        GroupMap tests_;

        std::vector<TestBase*> list_testcases_;

        unsigned long int time_total_;// duration, in second.

        static TestRegister* class_handler;

    public:
        /// class_handler itself does not  need to be deleted
        ~TestRegister() __BW_NOEXCEPT;


        bool registerTestDefaultGroup(TestBase* newTest);

        bool registerTest(TestBase* newTest, const char* group);

        static TestRegister* instance();

        static unsigned long runGroup(const char* group);


        static void runAllTests();

        static void reportAllTests() __BW_NOEXCEPT;
};


}// namespace BWTest_Names


////////////////////////////////////////////////////////////
#undef _BWTEST_TEST_NAME_DFGRP
#define _BWTEST_TEST_NAME_DFGRP(test_name)\
    BWTestGroup_default__Case_##test_name##_End

#undef TESTCASE
#define TESTCASE(test_name, test_time)\
class _BWTEST_TEST_NAME_DFGRP(test_name) : public BWTest_Names::TestBase\
{\
    void testBody();\
    public:\
        _BWTEST_TEST_NAME_DFGRP(test_name) ()\
        :   TestBase(test_time, #test_name)  {}\
};\
const bool BWTEST_##test_name##_registered =\
        BWTest_Names::TestRegister::instance()->registerTestDefaultGroup(new _BWTEST_TEST_NAME_DFGRP(test_name) ());\
void _BWTEST_TEST_NAME_DFGRP(test_name)::testBody()
////////////////////////////////////////////////////////////


#undef _BWTEST_TEST_NAME
#define _BWTEST_TEST_NAME(test_group, test_name)\
    BWTestGroup_##test_group##__Case_##test_name##_End
////////////////////////////////////////////////////////////
#undef TEST
#define TEST(test_group, test_name, test_time)\
class _BWTEST_TEST_NAME(test_group, test_name) : public BWTest_Names::TestBase\
{\
    void testBody();\
    public:\
        _BWTEST_TEST_NAME(test_group, test_name) ()\
        :   TestBase(test_time, #test_name, #test_group)  {}\
};\
const bool BWTEST_##test_group##__##test_name##_registered =\
        BWTest_Names::TestRegister::instance()->registerTest(new _BWTEST_TEST_NAME(test_group, test_name) (), #test_group);\
void _BWTEST_TEST_NAME(test_group, test_name)::testBody()

////////////////////////////////////////////////////////////

#undef RUN_ALL
#define RUN_ALL\
    BWTest_Names::TestRegister::instance()->runAllTests

////////////////////////////////////////////////////////////
#undef RUN_GROUP
#define RUN_GROUP(group)\
    BWTest_Names::TestRegister::instance()->runGroup(group)

////////////////////////////////////////////////////////////
#undef REPORT_ALL
#define REPORT_ALL\
    BWTest_Names::TestRegister::instance()->reportAllTests



namespace BWTest_Names
{

#ifdef __GNUC__
///////////////////////////////////////////////////////////////////////////////////////
/////////////  two classes below are adopted from stackoverflow
/////////////   http://stackoverflow.com/questions/760301/implementing-a-no-op-stdostream
/////////////////////////////////////////////////////////////////////////////////////////////
class basicNullBuf: public std::basic_streambuf<char, typename std::char_traits<char> >
{
    typename std::char_traits<char>::int_type overflow(typename std::char_traits<char>::int_type c)
    {
        typedef std::char_traits<char>  nameTmp;
        return nameTmp::not_eof(c);
      //  return  typename template std::char_traits<char>::typename not_eof(c); // indicate success
    }
};
class basicNullOStream: public std::basic_ostream<char, typename std::char_traits<char> >
{
    public:
        basicNullOStream():
        std::basic_ios<char, typename std::char_traits<char> >(&BasicNullOSBuf_),
        std::basic_ostream<char, typename std::char_traits<char> >(&BasicNullOSBuf_)
        {
            std::basic_ostream<char, typename std::char_traits<char> >::init(&BasicNullOSBuf_);
        }

    private:
        basicNullBuf BasicNullOSBuf_;
};
#endif


class BWTEST_Expect_Print_Aux
{
    bool expect_failed_;

#ifdef __BWTEST_HAS_ENOUGH_CXX11__
    BWTEST_Expect_Print_Aux() = delete;
    BWTEST_Expect_Print_Aux(const BWTEST_Expect_Print_Aux&) = delete;
    BWTEST_Expect_Print_Aux(BWTEST_Expect_Print_Aux&&) = delete;
    BWTEST_Expect_Print_Aux&
    operator= (const BWTEST_Expect_Print_Aux&)=delete;
    BWTEST_Expect_Print_Aux&
    operator= (BWTEST_Expect_Print_Aux&&)=delete;
#else
    private:
        BWTEST_Expect_Print_Aux(){}
        BWTEST_Expect_Print_Aux(const BWTEST_Expect_Print_Aux&){}
        BWTEST_Expect_Print_Aux&
        operator= (const BWTEST_Expect_Print_Aux&){return *this;}
#endif

public: // only one ctor is enabled
    explicit BWTEST_Expect_Print_Aux(
                bool statement,
                bool expectation,
                const char* expectText0,
                const char* expectText1,
                const char* actualText,
                const char* filePath,
                int lineNumber,
                const char* func_name);

    bool has_failed() const __BW_NOEXCEPT;

    static bool BWTEST_bool_caughtExcepttion;

};

} // namespece BWTest_Names

template<typename PrintType>
std::ostream& operator<< (const BWTest_Names::BWTEST_Expect_Print_Aux& expAux, const PrintType& msg) {
    if(expAux.has_failed()) {
        ::bwtest::getOutputStream() << ">>>  Message: " << msg;
        ::bwtest::getOutputStream() << std::endl;
        return ::bwtest::getOutputStream();
    }
    else {
#ifdef __GNUC__
        static BWTest_Names::basicNullOStream nullOS;// smaller than stringstream
#else
        static std::stringstream nullOS;
        nullOS.clear();
#endif
        return nullOS;
    }
}

#undef expect_true
#define expect_true(statement)\
        ::BWTest_Names::BWTEST_Expect_Print_Aux(statement,\
                    true,\
                  #statement,\
                  "  is true",\
                  "   is false",\
                  __FILE__,\
                  __LINE__,\
                  __FUNCTION__)

#undef expect_false
#define expect_false(statement)\
        ::BWTest_Names::BWTEST_Expect_Print_Aux(statement,\
                    false,\
                  #statement,\
                  "  is false",\
                  "  is true",\
                  __FILE__,\
                  __LINE__,\
                  __FUNCTION__)

#undef expect_eq
#define expect_eq(x, y)\
        ::BWTest_Names::BWTEST_Expect_Print_Aux( (x == y),\
                    true,\
                    #x,\
                    " == "#y,\
                    " not equal",\
                  __FILE__,\
                  __LINE__,\
                  __FUNCTION__)

#undef expect_eq_at
#define expect_eq_at(x, y, pres)\
        ::BWTest_Names::BWTEST_Expect_Print_Aux( (fabs(x - y) <= pres),\
                    true,\
                    #x,\
                    " == "#y,\
                    "not equal at "#pres,\
                  __FILE__,\
                  __LINE__,\
                  __FUNCTION__)

#undef expect_nq_at
#define expect_nq_at(x, y, pres)\
        ::BWTest_Names::BWTEST_Expect_Print_Aux( (fabs(x - y) > pres),\
                    true,\
                    #x,\
                    " != "#y,\
                    " equal at "#pres,\
                  __FILE__,\
                  __LINE__,\
                  __FUNCTION__)

#undef expect_nq
#define expect_nq(x, y)\
        ::BWTest_Names::BWTEST_Expect_Print_Aux( (x != y),\
                    true,\
                    #x,\
                    " != "#y,\
                    " equals ",\
                  __FILE__,\
                  __LINE__,\
                  __FUNCTION__)

#undef expect_throw
#define expect_throw(statement, exceptType)\
    ::BWTest_Names::BWTEST_Expect_Print_Aux::BWTEST_bool_caughtExcepttion = false;\
    try{statement;}\
    catch(exceptType e){::BWTest_Names::BWTEST_Expect_Print_Aux::BWTEST_bool_caughtExcepttion = true;}\
    catch(...){}\
    ::BWTest_Names::BWTEST_Expect_Print_Aux(::BWTest_Names::BWTEST_Expect_Print_Aux::BWTEST_bool_caughtExcepttion,\
                                    true,\
                                    #statement,\
                                    "  throws "#exceptType,\
                                    "  didn't throw "#exceptType,\
                                    __FILE__,\
                                    __LINE__,\
                                    __FUNCTION__)

#undef expect_throw_any
#define expect_throw_any(statement)\
    ::BWTest_Names::BWTEST_Expect_Print_Aux::BWTEST_bool_caughtExcepttion = false;\
    try{statement;}\
    catch(...){::BWTest_Names::BWTEST_Expect_Print_Aux::BWTEST_bool_caughtExcepttion = true;}\
    ::BWTest_Names::BWTEST_Expect_Print_Aux(::BWTest_Names::BWTEST_Expect_Print_Aux::BWTEST_bool_caughtExcepttion,\
                                    true,\
                                    #statement,\
                                    "  throws any thing",\
                                    "  throws nothing",\
                                    __FILE__,\
                                    __LINE__,\
                                    __FUNCTION__)


#undef OUT_ONCE

#endif //_BOWEN_TEST_H_




