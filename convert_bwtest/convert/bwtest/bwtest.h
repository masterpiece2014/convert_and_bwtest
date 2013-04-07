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
                    #undef __BWTEST_HAS_ENOUGH_CXX11__
//////////////////////////////////////////////////////////////////////////////////////////////


#include <iostream>
#include <fstream>
#include <stdexcept>
#include <vector>
#include <limits>
#include <cstddef>
#include <cstdlib>
#include <cmath>
#include <cassert>
#include <cstring>

#ifdef __BWTEST_HAS_ENOUGH_CXX11__
#include <chrono>
#else
#include <ctime>
#endif


/// to_string may not be supported
/// so break this
#ifdef __BWTEST_HAS_CX
#include <string>
#undef ERROR_MSG
#define ERROR_MSG(message)\
(__FILE__ "(" + std::to_string((long long int) __LINE__) + "): " + (message)\).c_str()

#undef RANGE_CHEACK
#define RANGE_CHEACK(val, TypeName)\
        val <= static_cast<decltype(val)>(std::numeric_limits<TypeName>::max())\
            ?   (void)0 : throw std::overflow_error(ERROR_MSG("overflow in convertion"));\
        val >= static_cast<decltype(val)>(std::numeric_limits<TypeName>::min())\
                ? (void)0 : throw std::underflow_error(ERROR_MSG("underflow in convertion"))
#else

    template<class T, class E>
    void __BWTEST_RANGE_CHEACK(T val, E);

#undef RANGE_CHEACK
#define RANGE_CHEACK(val, TypeName)\
        __BWTEST_RANGE_CHEACK(val, TypeName());
#endif

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
#define __BW_OVERRRIDE override
#else
#define __BW_NOEXCEPT throw()
#define __BW_NULL_PTR NULL
#define __BW_OVERRRIDE
#endif

///!<  scoped enumeration is not supported in c++99 or c++03
///!< what a pity !
#ifndef __BWTEST_HAS_ENOUGH_CXX11__
    enum BWTestOutputType {
        stdCout,
        stdCerr,
        stdClog,
        file
    };
#endif


class bwtest
{
public:
#ifdef __BWTEST_HAS_ENOUGH_CXX11__
    enum BWTestOutputType {
        stdCout,
        stdCerr,
        stdClog,
        file
    };
    bwtest() = delete;
    static BWTestOutputType outputStream;
#else
private:
    bwtest() {}
    static BWTestOutputType outputStream;
#endif
private:
    static char* _fileName;

public:

    static void setOutput(const char* c_str);
    static std::ostream& getOutputStream();
    static void cleanOutputStream();
};



#define COUT_1(obj)

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////  enable printing //////////////////////////////
//////////////////      in looping functions and testCase.excute() ///////////////
                                #define DO_PRINT
///////////////////////////////////////////////////////////////////////////////
#ifdef DO_PRINT
#undef COUT_1
#define COUT_1(obj) ::bwtest::getOutputStream() << obj

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
                        COUT_1("\n\t caught std exception");\
                        COUT_1("\n");\
                        COUT_1(e.what());\
                        COUT_1("\n");\
                    }\
                    catch(...) {\
                        unknown_except_caught++;\
                        COUT_1("\n\t caught unknown exception");\
                        COUT_1("\n");\
                    }\
                }\
                COUT_1("\n\t Totally caught: ");\
                COUT_1(std_except_caught + unknown_except_caught);\
                COUT_1("\n\t std exception : ");\
                COUT_1(std_except_caught);\
                COUT_1("\n\tunknown exception: ");\
                COUT_1(unknown_except_caught);\
                COUT_1("\n");\
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

#define __BW_NOEXCEPT noexcept
#define __BW_NULL_PTR nullptr
#define __BW_OVERRRIDE override
#else
#define __BW_NOEXCEPT throw()
#define __BW_NULL_PTR NULL
#define __BW_OVERRRIDE
#endif


class TestBase
{
    private:
        size_t _excutionNumberOfTimes;
        size_t _stdExceptionCaught;
        size_t _otherExceptionCaught;
        unsigned long int _timePassed;// duration, in second.
        const char* _testName;

    virtual void testBody() = 0;

#ifdef __BWTEST_HAS_ENOUGH_CXX11__
        TestBase() = delete;
        TestBase(const TestBase&) = delete;
        TestBase& operator= (const TestBase& ) = delete;
#else
    private:
        TestBase() {}
        TestBase(const TestBase&) {}
        TestBase& operator= (const TestBase& ) { return *this;}
#endif

  public:
        explicit
        TestBase(size_t tmies, const char* c_str)
        : _excutionNumberOfTimes(tmies),
          _stdExceptionCaught(0),
          _otherExceptionCaught(0),
          _testName(c_str)  {}

        virtual
        ~TestBase() __BW_NOEXCEPT;
        void excute();

        void printReport() __BW_NOEXCEPT;
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
        std::vector<TestBase*> _testCaseList;

        unsigned long int _totalTime;// duration, in second.

        static TestRegister* _handler;

    public:
        /// _handler itself does not  need to be deleted
        ~TestRegister() __BW_NOEXCEPT;
        bool registerTest(TestBase* newTest);

        static TestRegister* instance();

        static void runAllTests();

        static void
        reportAllTests() __BW_NOEXCEPT;
};

//////////////////////////////////////////////////////////////////////////////////////////////
/////////////       copyright
/////////////
///////////// following two classes are adopted from stackoverflow
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
        std::basic_ios<char, typename std::char_traits<char> >(&m_sbuf),
        std::basic_ostream<char, typename std::char_traits<char> >(&m_sbuf)
        {
            std::basic_ostream<char, typename std::char_traits<char> >::init(&m_sbuf);
        }

    private:
        basicNullBuf m_sbuf;
};
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
}// namespace BWTest_Names



#undef _BWTEST_TEST_NAME
#define _BWTEST_TEST_NAME(test_name)\
    BWTEST_Class_##test_name##_Test


#undef TEST
#define TEST(test_name, test_time)\
class _BWTEST_TEST_NAME(test_name) : public BWTest_Names::TestBase\
{\
    void testBody() __BW_OVERRRIDE;\
    public:\
        _BWTEST_TEST_NAME(test_name) ()\
        :   TestBase(test_time, #test_name)  {}\
};\
const bool BWTEST_bool_##test_name##_registered =\
        BWTest_Names::TestRegister::instance()->registerTest(new _BWTEST_TEST_NAME(test_name));\
void _BWTEST_TEST_NAME(test_name)::testBody()


#undef RUN_ALL
#define RUN_ALL\
    BWTest_Names::TestRegister::instance()->runAllTests

#undef REPORT_ALL
#define REPORT_ALL\
    BWTest_Names::TestRegister::instance()->reportAllTests


class BWTEST_ExpectAux
{
    bool _isFalse;

#ifdef __BWTEST_HAS_ENOUGH_CXX11__
    BWTEST_ExpectAux() = delete;
    BWTEST_ExpectAux(const BWTEST_ExpectAux&) = delete;
    BWTEST_ExpectAux(BWTEST_ExpectAux&&) = delete;
    BWTEST_ExpectAux&
    operator= (const BWTEST_ExpectAux&)=delete;
    BWTEST_ExpectAux&
    operator= (BWTEST_ExpectAux&&)=delete;
#else
    private:
        BWTEST_ExpectAux(){}
        BWTEST_ExpectAux(const BWTEST_ExpectAux&){}
        BWTEST_ExpectAux&
        operator= (const BWTEST_ExpectAux&){return *this;}
#endif
public: // only one ctor is enabled
    explicit BWTEST_ExpectAux(
                bool statement,
                bool expectation,
              const char* expectText0,
              const char* expectText1,
              const char* actualText,
                const char* filePath,
                int lineNumber);

    static bool BWTEST_bool_caughtExcepttion;

    template<typename PrintType>
friend std::ostream& operator<< (const BWTEST_ExpectAux& expAux, PrintType& msg);
};

template<typename PrintType>
std::ostream& operator<< (const BWTEST_ExpectAux& expAux, PrintType& msg);


#undef expect_true
#define expect_true(statement)\
        BWTEST_ExpectAux(statement,\
                    true,\
                  #statement,\
                  "  is true",\
                  "   is false",\
                  __FILE__,\
                  __LINE__)

#undef expect_false
#define expect_false(statement)\
        BWTEST_ExpectAux(statement,\
                    false,\
                  #statement,\
                  "  is false",\
                  "  is true",\
                  __FILE__,\
                  __LINE__)

#undef expect_eq
#define expect_eq(x, y)\
        BWTEST_ExpectAux( (bool)(x == y),\
                    true,\
                    #x,\
                    " == "#y,\
                    " not equal",\
                  __FILE__,\
                  __LINE__)

#undef expect_eq_at
#define expect_eq_at(x, y, pres)\
        BWTEST_ExpectAux( (fabs(x - y) <= pres),\
                    true,\
                    #x,\
                    " == "#y,\
                    "not equal at "#pres,\
                  __FILE__,\
                  __LINE__)

#undef expect_nq_at
#define expect_nq_at(x, y, pres)\
        BWTEST_ExpectAux( (fabs(x - y) > pres),\
                    true,\
                    #x,\
                    " != "#y,\
                    " equal at "#pres,\
                  __FILE__,\
                  __LINE__)

#undef expect_nq
#define expect_nq(x, y)\
        BWTEST_ExpectAux( (x != y),\
                    true,\
                    #x,\
                    " != "#y,\
                    " equals ",\
                  __FILE__,\
                  __LINE__)

#undef expect_throw
#define expect_throw(statement, exceptType)\
    BWTEST_ExpectAux::BWTEST_bool_caughtExcepttion = false;\
    try{statement;}\
    catch(exceptType e){BWTEST_ExpectAux::BWTEST_bool_caughtExcepttion = true;}\
    catch(...){}\
    BWTEST_ExpectAux(BWTEST_ExpectAux::BWTEST_bool_caughtExcepttion,\
                                    true,\
                                    #statement,\
                                    "  throws "#exceptType,\
                                    "  didn't throw "#exceptType,\
                                    __FILE__,\
                                    __LINE__)

#undef expect_throw_any
#define expect_throw_any(statement)\
    BWTEST_ExpectAux::BWTEST_bool_caughtExcepttion = false;\
    try{statement;}\
    catch(...){BWTEST_ExpectAux::BWTEST_bool_caughtExcepttion = true;}\
    BWTEST_ExpectAux(BWTEST_ExpectAux::BWTEST_bool_caughtExcepttion,\
                                    true,\
                                    #statement,\
                                    "  throws any thing",\
                                    "  throws nothing",\
                                    __FILE__,\
                                    __LINE__)

#include "bwtest-impl.h"

#define put_out bwtest::getOutputStream()

#undef COUT_1
#endif //_BOWEN_TEST_H_




