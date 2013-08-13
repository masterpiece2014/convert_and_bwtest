
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
//////      BWTestInternal has been tested on:
//////      GCC 4.7.2 on 64-bit MS Windows and 64-bit Linux
//////      MS Virtual C++ 11(_MSC_VER 1700) on 64-bit Windows
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


///////////     REMOVE!!!    //////////////////////////
//#include <chrono>
/////////////////////////////////////

#include "commen.h"
#include "benchmark.h"
#include "testio.h"
#include "testbase.h"



///////////////////////////////////////////////////////////////////////////////
namespace bwtest {
    class TestBase;
}
#undef _BWTEST_TEST_NAME_DFGRP
#define _BWTEST_TEST_NAME_DFGRP(test_name)\
    BWTestGroup_default__Case_##test_name##_End

#undef TESTCASE
#define TESTCASE(test_name, test_time)\
class _BWTEST_TEST_NAME_DFGRP(test_name) : public bwtest::TestBase\
{\
    void run();\
    public:\
        _BWTEST_TEST_NAME_DFGRP(test_name) ()\
        :   TestBase(test_time, #test_name)  {}\
};\
const bool BWTEST_##test_name##_registered =\
        BWTestInternal::TestRegister::instance()->registerTestDefaultGroup(new _BWTEST_TEST_NAME_DFGRP(test_name) ());\
void _BWTEST_TEST_NAME_DFGRP(test_name)::run()
/////////////////////////////////////
#undef _BWTEST_TEST_NAME
#define _BWTEST_TEST_NAME(test_group, test_name)\
    BWTestGroup_##test_group##__Case_##test_name##_End
///////////////////////////////////////////////////////////////////////////////
#undef TEST
#define TEST(test_group, test_name, test_time)\
class _BWTEST_TEST_NAME(test_group, test_name) : public bwtest::TestBase\
{\
    void run();\
    public:\
        _BWTEST_TEST_NAME(test_group, test_name) ()\
        :   TestBase(test_time, #test_name, #test_group)  {}\
};\
const bool BWTEST_##test_group##__##test_name##_registered =\
bwtest::BWTestInternal::TestRegister::instance()->registerTest(new _BWTEST_TEST_NAME(test_group, test_name) (), #test_group);\
void _BWTEST_TEST_NAME(test_group, test_name)::run()


#undef CONSTRUCT
#define CONSTRUCT(CustomClass)\
    public:\
    CustomClass() : bwtest::TestBase(#CustomClass)

#undef REGISTER
#define REGISTER(CustomClass)\
const bool BWTEST_CUSTOM_##CustomClass##_registered =\
    bwtest::BWTestInternal::TestRegister::instance()->registerTest(new CustomClass(), ::bwtest::CUSTOM_GROUP)
///////////////////////////////////////////////////////////////////////////////

namespace bwtest {

namespace BWTestInternal {

    std::string toString(long long);
    std::string toString(unsigned long long);
    std::string toString(float);

    template <typename Tgt, typename Src>
    int rangeCheck(Src src) {
        Tgt t_max = std::numeric_limits<Tgt>::max();
        Tgt t_min = std::numeric_limits<Tgt>::min();
        if (src < t_min) {
            return -1;
        } else if (src > t_max) {
            return 1;
        } else {
            return 0;
        }
    }
    std::string makeErrorMsg(const char*, const char*, long long);
}
}

#undef RANGE_CHEACK
#define RANGE_CHEACK(val, TypeName)\
{\
    int result = rangeCheck<TypeName>(val);\
    if (1 == result) {\
        throw std::overflow_error(\
                bwtest::BWTestInternal::makeErrorMsg(__FILE__,\
                                    __FUNCTION__, (long long)__LINE__)\
                .append("\n>>>  overflow in converting to "#TypeName).append("\n")\
          );\
    } else if (-1 == result) {\
        throw std::underflow_error(\
                bwtest::BWTestInternal::makeErrorMsg(__FILE__,\
                                    __FUNCTION__, (long long)__LINE__)\
                .append("\n>>>  underflow in converting to "#TypeName).append("\n")\
          );\
    }\
}

#undef assert_throw
#define assert_throw(statement, expected_except)\
        {\
            bool caught = false;\
            try{statement;}\
            catch(expected_except const& e){caught = true;}\
            catch(...){}\
            assert(caught);\
        }


#undef assert_throw_any
#define assert_throw_any(statement)\
        {\
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


#undef LOOP_FUNC
#define LOOP_FUNC(func, loops)\
            {\
                size_t std_except_caught = 0;\
                size_t unknown_except_caught = 0;\
                for(size_t i = 0; i != loops; ++i ) {\
                    try{\
                    func();\
                    }catch(std::exception& e) {\
                        std_except_caught++;\
                        OUT_ONCE(BWTestInternal::makeErrorMsg(__FILE__, __FUNCTION__, (long long)__LINE__));\
                        OUT_ONCE("\n>>>  caught std exception\n");\
                        OUT_ONCE(e.what());\
                        OUT_ONCE("\n");\
                    }\
                    catch(...) {\
                        unknown_except_caught++;\
                        OUT_ONCE(BWTestInternal::makeErrorMsg(__FILE__, __FUNCTION__, (long long)__LINE__));\
                        OUT_ONCE("\n>>>  caught unknown exception");\
                        OUT_ONCE("\n");\
                    }\
                }\
                OUT_ONCE("\n>>  Totally caught: ");\
                OUT_ONCE(std_except_caught + unknown_except_caught);\
                OUT_ONCE("\n>>>    std exception : ");\
                OUT_ONCE(std_except_caught);\
                OUT_ONCE("\n>>>    unknown exception: ");\
                OUT_ONCE(unknown_except_caught);\
                OUT_ONCE("\n");\
            }

namespace bwtest {

namespace BWTestInternal
{

class TestRegister
{
    private:
        TestRegister(){}

        BWTEST_NO_ASSIGN(TestRegister);
        BWTEST_NO_COPY(TestRegister);

    private:
        typedef std::vector<bwtest::TestBase*>          Group;
        typedef std::map<std::string, Group>    GroupMap;
        GroupMap tests_;
        
        unsigned long int time_total_;// duration, in second.

        static TestRegister* class_handler;

    public:
        /// class_handler itself does not  need to be deleted
        ~TestRegister() BW_NOEXCEPT;

        bool registerTestDefaultGroup(::bwtest::TestBase* newTest);

        bool registerTest(::bwtest::TestBase* newTest, const char* group);
        
        int runAllTests();

        int runGroup(const char* group);
        
        int runTest(const char*group, const char*name);
        
 //       int reportAllTests() BW_NOEXCEPT;
        
        static TestRegister* instance();
};


}// namespace BWTestInternal
} // bwtest

////////////////////////////////////////////////////////////
#undef RUN_ALL
#define RUN_ALL \
    bwtest::BWTestInternal::TestRegister::instance()->runAllTests

#undef RUN_GROUP
#define RUN_GROUP(group)\
    bwtest::BWTestInternal::TestRegister::instance()->runGroup(#group)

#undef RUN_TEST
#define RUN_TEST(group, name)\
        bwtest::BWTestInternal::TestRegister::instance()->runTest(#group, #name)
/////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////

namespace bwtest {
namespace BWTestInternal {
    class PrintAux;
}
}
template<typename PrintType>
std::ostream& operator<< (const bwtest::BWTestInternal::PrintAux& expAux, const PrintType& msg) {
    if(expAux.has_failed()) {
        bwtest::getOutputStream() << ">>>  Message: " << msg;
        bwtest::getOutputStream() << std::endl;
        return bwtest::getOutputStream();
    }
    else {
        static bwtest::BWTestInternal::NullOStream nullOS;// smaller than stringstream
        return nullOS;
    }
}

#undef expect_true
#define expect_true(statement)\
        bwtest::BWTestInternal::PrintAux(statement,\
                    true,\
                  #statement,\
                  "  is true",\
                  "   is false",\
                  __FILE__,\
                  __LINE__,\
                  __FUNCTION__)

#undef expect_false
#define expect_false(statement)\
        bwtest::BWTestInternal::PrintAux(statement,\
                    false,\
                  #statement,\
                  "  is false",\
                  "  is true",\
                  __FILE__,\
                  __LINE__,\
                  __FUNCTION__)

#undef expect_eq
#define expect_eq(x, y)\
        bwtest::BWTestInternal::PrintAux( (x == y),\
                    true,\
                    #x,\
                    " == "#y,\
                    " not equal",\
                  __FILE__,\
                  __LINE__,\
                  __FUNCTION__)

#undef expect_eq_at
#define expect_eq_at(x, y, pres)\
        bwtest::BWTestInternal::PrintAux( (fabs(x - y) <= pres),\
                    true,\
                    #x,\
                    " == "#y,\
                    "not equal at "#pres,\
                  __FILE__,\
                  __LINE__,\
                  __FUNCTION__)

#undef expect_nq_at
#define expect_nq_at(x, y, pres)\
        bwtest::BWTestInternal::PrintAux( (fabs(x - y) > pres),\
                    true,\
                    #x,\
                    " != "#y,\
                    " equal at "#pres,\
                  __FILE__,\
                  __LINE__,\
                  __FUNCTION__)

#undef expect_nq
#define expect_nq(x, y)\
        bwtest::BWTestInternal::PrintAux( (x != y),\
                    true,\
                    #x,\
                    " != "#y,\
                    " equals ",\
                  __FILE__,\
                  __LINE__,\
                  __FUNCTION__)

#undef expect_throw
#define expect_throw(statement, exceptType)\
    bwtest::BWTestInternal::PrintAux::BWTEST_bool_caughtExcepttion = false;\
    try{statement;}\
    catch(exceptType e){bwtest::BWTestInternal::PrintAux::BWTEST_bool_caughtExcepttion = true;}\
    catch(...){}\
    bwtest::BWTestInternal::PrintAux(bwtest::BWTestInternal::PrintAux::BWTEST_bool_caughtExcepttion,\
                                    true,\
                                    #statement,\
                                    "  throws "#exceptType,\
                                    "  didn't throw "#exceptType,\
                                    __FILE__,\
                                    __LINE__,\
                                    __FUNCTION__)

#undef expect_throw_any
#define expect_throw_any(statement)\
    bwtest::BWTestInternal::PrintAux::BWTEST_bool_caughtExcepttion = false;\
    try{statement;}\
    catch(...){bwtest::BWTestInternal::PrintAux::BWTEST_bool_caughtExcepttion = true;}\
    bwtest::BWTestInternal::PrintAux(bwtest::BWTestInternal::PrintAux::BWTEST_bool_caughtExcepttion,\
                                    true,\
                                    #statement,\
                                    "  throws any thing",\
                                    "  throws nothing",\
                                    __FILE__,\
                                    __LINE__,\
                                    __FUNCTION__)


#undef OUT_ONCE

//#include "bwtest.cpp"

#endif //_BOWEN_TEST_H_




