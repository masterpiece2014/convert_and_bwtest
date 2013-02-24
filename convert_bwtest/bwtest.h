///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
////// Copyright 2013, Cai Bowen
////// All rights reserved.
//////
//////  Author: Cai Bowen
//////      contact/bug report/get new version
 /////           at
 /////       feedback2bowen@outlook.com
 /////
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
//////      bwtest has been tested on:
//////      GCC 4.7.2 on 64-bit MS Windows and 64-bit Linux
//////      MS Virtual C++ 11(_MSC_VER 1700) on 64-bit Windows
//////
////// Last modified: 2013.2.19
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////



#ifndef _BOWEN_TEST_H_
#define _BOWEN_TEST_H_

#ifdef __GNUC__
    #if __GNUC__*100 + __GNUC_MINOR__*10 + __GNUC_PATCHLEVEL__ > 472
        #define __BWTEST_HAS_CXX11__
    #endif
    #if __GNUC__*100 + __GNUC_MINOR__*10 + __GNUC_PATCHLEVEL__ < 400
        #error "please up data your complier to GCC 4.0.0 and above"
    #endif

#else
#ifdef _MSC_VER
    #if _MSC_VER > 1700
        #define __BWTEST_HAS_CXX11__
    #endif
    #if _MSC_VER < 1300
        #error "please up data your complier to VC 7.0 and above"
    #endif
#endif
#endif


#include <iostream>
#include <stdexcept>
#include <vector>
#include <limits>
#include <cstddef>
#include <cmath>
#include <cassert>

#ifdef __BWTEST_HAS_CXX11__
#include <chrono>
#else
#include <ctime>
#endif



#ifdef __BWTEST_HAS_CXX11__

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
    void __BWTEST_RANGE_CHEACK(T val, E) {
        val <= static_cast<T>(std::numeric_limits<E>::max())
        ?   (void)0 : throw std::overflow_error("overflow in convertion");
        val >= static_cast<T>(std::numeric_limits<E>::min())
            ? (void)0 : throw std::underflow_error("underflow in convertion");
    }

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


#define COUT_1(obj)
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////  enable printing //////////////////////////////
//////////////////      in looping functions and testCase.run() ///////////////
                                #define DO_PRINT
///////////////////////////////////////////////////////////////////////////////
#ifdef DO_PRINT
#undef COUT_1
#define COUT_1(obj)  std::cout << obj;
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


namespace bwtest
{

//// multi-type, multi-arguments printing
////    e.g.
//// print(std::string("sdfds"), 32234, 546.546, "sdgdf", 'c');
////
#ifdef __BWTEST_HAS_CXX11__
void print(){}
template<typename T, typename... Ts>
void print(const T& obj, const Ts&... others) {
    std::cout << obj << " ";
    print(others...);
}

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
        size_t _excutionTimes;
        size_t _stdExceptionCaught;
        size_t _otherExceptionCaught;
        const char* _testName;

    virtual void testBody() = 0;

#ifdef __BWTEST_HAS_CXX11__
        TestBase() = delete;
        TestBase(const TestBase&) = delete;
        TestBase& operator= (const TestBase& ) = delete;
#else
    private:
        TestBase() {}
        TestBase(const TestBase&) {}
        TestBase& operator= (const TestBase& ) {}
#endif

  public:
        explicit
        TestBase(size_t tmies, const char* c_str)
        : _excutionTimes(tmies),
          _stdExceptionCaught(0),
          _otherExceptionCaught(0),
          _testName(c_str)  {}

        virtual
        ~TestBase() __BW_NOEXCEPT {
            delete _testName;
        }

        void run() {
            for(size_t i = 0; i != _excutionTimes; ++i) {
                try {
                    testBody();
                }catch(std::exception& e) {
                    _stdExceptionCaught++;
                    COUT_1("\n>>> Caught std exception:\n>>> " << e.what());
                }catch(...) {
                    _otherExceptionCaught++;
                    COUT_1("\n>>> Caught other exception");
                }
            }
        }
        void printReport() __BW_NOEXCEPT {
            std::cout << "\n>>>  test\"" << _testName
            << "\" has been tested for " << _excutionTimes << " times.";
            std::cout << "\n>>> caught std::exception " << _stdExceptionCaught << " times."
            << "\n>>> caught other exception " << _otherExceptionCaught << " times." << std::endl;
        }
};
class TestRegister
{
    private:
        TestRegister(){}
#ifdef __BWTEST_HAS_CXX11__
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

        std::vector<TestBase*> _testCaseList;

        unsigned long int _timeElapsed;// duration, in second.

        static TestRegister* _handler;

    public:
        /// _handler does not  need to be deleted
        ~TestRegister() __BW_NOEXCEPT {
            for(size_t i = 0; i != _testCaseList.size(); ++i) {
                delete _testCaseList[i];
            }
        }
        bool registerTest(TestBase* newTest) {
            _handler->_testCaseList.push_back(newTest);
            return true;
        }

        static TestRegister*
        instance() {
            if(TestRegister::_handler == NULL) {
                _handler = new TestRegister();
            }
            return _handler;
        }

#ifdef __BWTEST_HAS_CXX11__
        static int runAllTests() {
            auto t_start = std::chrono::high_resolution_clock::now();
            for(size_t i = 0; i != _handler->_testCaseList.size(); ++i) {
                _handler->_testCaseList[i]->run();
            }
            auto t_end = std::chrono::high_resolution_clock::now();
            _handler->_timeElapsed
            = std::chrono::duration_cast<std::chrono::seconds>(t_end - t_start).count();
            return 0;
        }
#else
        static void runAllTests() {
            std::clock_t c_start = std::clock();
            for(size_t i = 0; i != _handler->_testCaseList.size(); ++i) {
                _handler->_testCaseList[i]->run();
            }
            std::clock_t c_end = std::clock();
            _handler->_timeElapsed
                = (c_end - c_start) / CLOCKS_PER_SEC;
        }
#endif
        static void
        reportAllTests() __BW_NOEXCEPT {
            std::cout << "\n\n>>>\t TEST REPORT" << std::endl;
            time_t t_now = std::time(NULL);
            std::cout << ">>>time: " << std::asctime(std::localtime(&t_now));
            for(size_t i = 0; i != _handler->_testCaseList.size(); ++i) {
                _handler->_testCaseList[i]->printReport();
            }
            std::cout << "\n>>> Time elapsed: "
                << _handler->_timeElapsed << " seconds" << std::endl;
        }
};
TestRegister* TestRegister::_handler = __BW_NULL_PTR;

}// namespace bwtest

#undef _BWTEST_TEST_NAME
#define _BWTEST_TEST_NAME(test_name)\
    BWTEST_Class_##test_name##_Test


#undef TEST
#define TEST(test_name, test_time)\
class _BWTEST_TEST_NAME(test_name) : public bwtest::TestBase\
{\
    void testBody() __BW_OVERRRIDE;\
    public:\
        _BWTEST_TEST_NAME(test_name) ()\
        :   TestBase(test_time, #test_name)  {}\
};\
const bool BWTEST_bool_##test_name##_registered =\
        bwtest::TestRegister::instance()->registerTest(new _BWTEST_TEST_NAME(test_name));\
void _BWTEST_TEST_NAME(test_name)::testBody() __BW_OVERRRIDE


#undef RUN_ALL
#define RUN_ALL\
    bwtest::TestRegister::instance()->runAllTests

#undef REPORT_ALL
#define REPORT_ALL\
    bwtest::TestRegister::instance()->reportAllTests


class BWTEST_ExpectAux
{
    bool _isFalse;

#ifdef __BWTEST_HAS_CXX11__
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
public: // only this ctor is enabled
    explicit BWTEST_ExpectAux(
                bool statement,
                bool expectation,
              const char* text0,
              const char* text1,
                const char* filePath,
                int lineNumber)
    : _isFalse(statement != expectation) {

        if(_isFalse) {
        //    time_t t_now = std::time(NULL);
            std::cerr << "\n\n>>>  expect: \" "
             << text0 << text1
             << "\"  fell."
             << "\n>>>  at file: " << filePath
             << "\n>>>  line: " << lineNumber
             << std::endl;
         //    << "\n>>> system time: "
         //    << std::asctime(std::localtime(&t_now));
        }
    }

    static bool BWTEST_bool_caughtExcepttion;

    template<typename PrintType>
friend std::ostream& operator<< (const BWTEST_ExpectAux& expAux, PrintType& msg);
};
bool BWTEST_ExpectAux::BWTEST_bool_caughtExcepttion = false;

template<typename PrintType>
std::ostream& operator<< (const BWTEST_ExpectAux& expAux, PrintType& msg) {
    if(expAux._isFalse)
        std::cerr <<"\n>>>  msg: "<< msg;
    return std::cerr;
}


#undef expect_true
#define expect_true(statement)\
        BWTEST_ExpectAux(statement,\
                    true,\
                  #statement,\
                  "  is true",\
                  __FILE__,\
                  __LINE__)

#undef expect_false
#define expect_false(statement)\
        BWTEST_ExpectAux(statement,\
                    false,\
                  #statement,\
                  "  is false",\
                  __FILE__,\
                  __LINE__)

#undef expect_eq
#define expect_eq(x, y)\
        BWTEST_ExpectAux( (fabs(x - y) <= 0.00001),\
                    true,\
                    #x,\
                    " == "#y,\
                  __FILE__,\
                  __LINE__)

#undef expect_eq_at
#define expect_eq_at(x, y, pres)\
        BWTEST_ExpectAux( (fabs(x - y) <= pres),\
                    true,\
                    #x,\
                    " == "#y,\
                  __FILE__,\
                  __LINE__)

#undef expect_nq_at
#define expect_nq_at(x, y, pres)\
        BWTEST_ExpectAux( (fabs(x - y) > pres),\
                    true,\
                    #x,\
                    " != "#y,\
                  __FILE__,\
                  __LINE__)

#undef expect_nq
#define expect_nq(x, y)\
        BWTEST_ExpectAux( (fabs(x - y) > 0.00001),\
                    true,\
                    #x,\
                    " != "#y,\
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
                                    __FILE__,\
                                    __LINE__)


#undef COUT_1
#endif //_BOWEN_TEST_H_




