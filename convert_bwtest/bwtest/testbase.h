
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


#ifndef _BOWEN_TESTBASE_H
#define _BOWEN_TESTBASE_H


#include "commen.h"

#include "testio.h"

#include "benchmark.h"

namespace bwtest {


const char* DEFALUT_GROUP  ="default";
const char* CUSTOM_GROUP = "custom";

    //virtual base class
class TestBase
{
    private: // internal fields
        const char* __name_testgroup;
        const char* __name_testcase_;
        size_t __num_excutions_;
        size_t __num_std_except_;
        size_t __num_other_except_;
        unsigned long int __time_elapsed_;// deprecated
        bwtest::Benchmark __benchmark;

#ifdef BWTEST_HAS_CXX11_
//        TestBase() = delete;
        TestBase(const TestBase&) = delete;
        TestBase(TestBase&&) = delete;
        TestBase& operator= (const TestBase& ) = delete;
#else
    private:
//        TestBase(); // no definition
        TestBase(const TestBase&);
        TestBase& operator= (const TestBase& );
#endif

  public:
        
        virtual void run() = 0;
        
        explicit TestBase(const char* testcase)
        :   __name_testgroup(CUSTOM_GROUP),
            __name_testcase_(testcase),
            __num_excutions_(1),
            __num_std_except_(0),
            __num_other_except_(0),
            __time_elapsed_(0),
            __benchmark(bwtest::CountThreadTime) {}

        explicit TestBase(size_t tmies, const char* testcase, const char* testgroup = DEFALUT_GROUP)
        :   __name_testgroup(testgroup),
            __name_testcase_(testcase),
            __num_excutions_(tmies),
            __num_std_except_(0),
            __num_other_except_(0),
            __time_elapsed_(0),
            __benchmark(bwtest::CountThreadTime) {}

        virtual
        ~TestBase() BW_NOEXCEPT   {}
        
        const char* getTestCaseName() const  BW_NOEXCEPT {
            return __name_testcase_;
        }
        const char* getTestGroupName() const BW_NOEXCEPT {
            return __name_testgroup;
        }
        void setExcuTime(size_t t) {
            this->__num_excutions_ = t;
        }

#ifdef BWTEST_HAS_CXX11_
		void excute() {
			try {
//				auto t_start = std::chrono::high_resolution_clock::now();
				for(size_t i = 0; i < __num_excutions_; ++i) {
					try {
						run();
					}
					catch(std::exception& e) {
						__num_std_except_++;
                        OUT_ONCE("\n>>>");
						OUT_ONCE(__name_testcase_);
						OUT_ONCE("       caught std exception:\n>>> ");
						OUT_ONCE(e.what());
					}
					catch(...) {
						__num_other_except_++;
                        OUT_ONCE("\n>>>");
						OUT_ONCE(__name_testcase_);
						OUT_ONCE("       caught other exception");
					}
				}
//				auto t_end = std::chrono::high_resolution_clock::now();
//				this->__time_elapsed_ = std::chrono::duration_cast<std::chrono::seconds>(t_end - t_start).count();
			}
			catch(...) {
			    OUT_ONCE("\n>>> ");
			    OUT_ONCE(__name_testcase_);
                OUT_ONCE("  testBody: caught unknown exception. \n");
			}
		}
#else
		void excute() {
			try {
//				std::clock_t c_start = std::clock();
				for(size_t i = 0; i != __num_excutions_; ++i) {
					try {
						run();
					} catch(std::exception& e) {
						__num_std_except_++;
                        OUT_ONCE("\n>>>");
						OUT_ONCE(__name_testcase_);
						OUT_ONCE("       caught std exception:\n>>> ");
						OUT_ONCE(e.what());
					} catch(...) {
						__num_other_except_++;
                        OUT_ONCE("\n>>>");
						OUT_ONCE(__name_testcase_);
						OUT_ONCE("       caught other exception");
					}
				}
//				std::clock_t c_end = std::clock();
//				this->__time_elapsed_ = (c_end - c_start) / CLOCKS_PER_SEC;
			}
			catch(...) {
			    OUT_ONCE("\n>>> ");
			    OUT_ONCE(__name_testcase_);
                OUT_ONCE("  testBody: caught unknown exception. \n");
			}
		}
#endif

        void printReport() const BW_NOEXCEPT {
            bwtest::getOutputStream() << "\n>>>  test\"" << __name_testcase_
            << "\" excuted " << __num_excutions_ << "times."
            << "\n>>>  time passed: " << this->__time_elapsed_
            << "s\n>>>  caught std exception " << __num_std_except_ << " times."
            << "\n>>>  caught other exception " << __num_other_except_ << " times." << std::endl;
        }
};


}
#endif // _BOWEN_TESTBASE_H
