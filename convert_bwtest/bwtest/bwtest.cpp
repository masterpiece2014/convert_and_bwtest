
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
///////////////////////////////////////////////////////////////////////////////



#include "bwtest.h"

#include <cstdio>
#include <stdarg.h>

namespace bwtest {
namespace BWTestInternal
{
        inline void formatAux(char* ptr, int size, const char* format, ...) {
            va_list args;
            va_start(args, format);
            vsnprintf(ptr, size, format, args);
            va_end(args);
        }
        
        std::string toString(long long val) {
            char buf[4*sizeof(long long)];
            formatAux(buf, 4*sizeof(long long), "%lld", val);
            return std::string(buf);
        }
        std::string toString(unsigned long long val) {
            char buf[4*sizeof(unsigned long long)];
            formatAux(buf, 4*sizeof(unsigned long long), "%llu", val);
            return std::string(buf);
        }
        std::string toString(float val) {
//            float: max exponent10 38
            char buf[48];
            formatAux(buf, 48, "%f", val);
            return std::string(buf);
        }

        std::string makeErrorMsg(const char* file, const char* function, long long line) {
            return std::string("\n>>>  ").append(file)
                        .append("\n>>>  Function: ").append(function)
                        .append("\t Line: ").append(BWTestInternal::toString(line));
        }

        
        
        TestRegister* TestRegister::class_handler = BW_NULL_PTR;

        TestRegister* TestRegister::instance(){
            if(TestRegister::class_handler == BW_NULL_PTR) {
                class_handler = new TestRegister();
            }
            return class_handler;
        }


        bool TestRegister::registerTestDefaultGroup(::bwtest::TestBase* newTest) {
            TestRegister::instance()->tests_[DEFALUT_GROUP].push_back(newTest);
            return true;
        }

        bool TestRegister::registerTest(::bwtest::TestBase* newTest, const char* group) {
            TestRegister::instance()->tests_[group].push_back(newTest);
            return true;
        }

        int  TestRegister::runGroup(const char* group) {
            GroupMap::iterator  grp = TestRegister::instance()->tests_.find(group);
            if (grp != TestRegister::instance()->tests_.end()) {
                for (size_t j = 0; j != grp->second.size(); ++j) {
                    grp->second.at(j)->excute();
                }
                return 0;
            } else {
                put_out << ">>>  empty group: " << group << '\n';
                return 1;
            }
        }

        int TestRegister::runAllTests()  {
            TestRegister* obj = TestRegister::instance();
            for (TYPENAME_OUT GroupMap::iterator i = obj->tests_.begin();
                                        i != obj->tests_.end();
                                        ++i) {
                for (size_t j = 0; j != i->second.size(); ++j) {
                    i->second.at(j)->excute();
                }
            }
            return 0;
        }

        int TestRegister::runTest(const char* g, const char* name) {
            TYPENAME_OUT GroupMap::iterator  grp = TestRegister::instance()->tests_.find(g);
            if (grp != TestRegister::instance()->tests_.end()) {
                for (TYPENAME_OUT Group::iterator i = grp->second.begin();
                                    i != grp->second.end();
                                                        ++i) {
                    if (0 == strcmp(name, (*i)->getTestCaseName())) {
                        (*i)->excute();
                        break;
                    }
                }
                return 0;
            } else {
                put_out << ">>>  empty group: " << g << '\n';
                return 1;
            }
        }

//        void TestRegister::reportAllTests() BW_NOEXCEPT {
//                        ::bwtest::getOutputStream() << "\n\n=============================================\n";
//            ::bwtest::getOutputStream() << ">>>\t TEST REPORT" << std::endl;
//            time_t t_now = std::time(BW_NULL_PTR);
//            ::bwtest::getOutputStream() << ">>>  time: " << std::asctime(std::localtime(&t_now));
//            for(size_t i = 0; i != TestRegister::instance()->class_handler->list_testcases_.size(); ++i) {
//
//                ::bwtest::getOutputStream() << "--------------------------------------";
//                class_handler->list_testcases_[i]->printReport();
//            }
//
//            ::bwtest::getOutputStream() << "\n>>> Time elapsed: "
//                << class_handler->time_total_ << " seconds" << std::endl;
//
//            ::bwtest::getOutputStream() << "=============================================\n";
//        }


/////////////////////////////////////////////////////////////////////////////////
        TestRegister::~TestRegister() BW_NOEXCEPT {
            for (GroupMap::iterator i = tests_.begin(); i != tests_.end(); ++i) {
                for (size_t j = 0; j != i->second.size(); ++j) {
                    delete i->second.at(j);
                }
            }
            ::bwtest::cleanOutputStream();
        }
}// namespace BWTestInternal
}
