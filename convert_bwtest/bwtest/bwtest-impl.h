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
//////      BWTest_Names has been tested on:
//////      GCC 4.7.2 on 64-bit MS Windows and 64-bit Linux
//////      MS Virtual C++ 11(_MSC_VER 1700) on 64-bit Windows
//////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

//    C++11 features maybe needed:
//        chrono for new high resolution clock
//        delete default constructors and assign operator
//        nullptr


#ifndef _BOWEN_TEST_H_
#error #error this file is the partial implementation of bwtest\
        it may only be included by bwtest.h\
        and cannot be used separately
#endif


#ifndef __BWTEST_HAS_CX
    template<class T, class E>
    void __BWTEST_RANGE_CHEACK(T val, E) {
        val <= static_cast<T>(std::numeric_limits<E>::max())
        ?   (void)0 : throw std::overflow_error("overflow in convertion");
        val >= static_cast<T>(std::numeric_limits<E>::min())
            ? (void)0 : throw std::underflow_error("underflow in convertion");
    }
#endif



     void bwtest::setOutput(const char* c_str)
    {
#ifdef __BWTEST_HAS_ENOUGH_CXX11__
        if (!strncmp(c_str, "cout", 4))
            outputStream = bwtest::BWTestOutputType::stdCout;
        else if (!strncmp(c_str, "cerr", 4))
            outputStream = bwtest::BWTestOutputType::stdCerr;
        else if (!strncmp(c_str, "clog", 4))
            outputStream = bwtest::BWTestOutputType::stdClog;
        else {
            _fileName = const_cast<char*>(c_str);
            outputStream = bwtest::BWTestOutputType::file;
        }
#else
        if (!strncmp(c_str, "cout", 4))
            outputStream = stdCout;
        else if (!strncmp(c_str, "cerr", 4))
            outputStream = stdCerr;
        else if (!strncmp(c_str, "clog", 4))
            outputStream = stdClog;
        else {
            _fileName = const_cast<char*>(c_str);
            outputStream = file;
        }
#endif
    }



     std::ostream& bwtest::getOutputStream() {
#ifdef __BWTEST_HAS_ENOUGH_CXX11__
        switch(bwtest::outputStream) {
        case bwtest::BWTestOutputType::stdCout : return std::cout;
        case bwtest::BWTestOutputType::stdCerr : return std::cerr;
        case bwtest::BWTestOutputType::stdClog : return std::clog;
        case bwtest::BWTestOutputType::file :
#else
        switch(bwtest::outputStream) {
        case stdCout : return std::cout;
        case stdCerr : return std::cerr;
        case stdClog : return std::clog;
        case file :
#endif
            static std::ofstream bwOStream(_fileName, std::ios::app);
            if (bwOStream.is_open())
                return bwOStream;
            else {
                std::cerr << "cannot open file" << _fileName << std::endl;
                exit(EXIT_FAILURE);
            }
        } // switch
    }



#ifdef __BWTEST_HAS_ENOUGH_CXX11__
     void bwtest::cleanOutputStream() {
        delete bwtest::_fileName;
        ::bwtest::getOutputStream().clear();
        ::bwtest::getOutputStream().flush();
        if (::bwtest::outputStream == ::bwtest::BWTestOutputType::file) {
            static_cast<std::ofstream&>(::bwtest::getOutputStream()
            ).close();
        }
    }

bwtest::BWTestOutputType bwtest::outputStream = bwtest::BWTestOutputType::stdCerr;
#else
     void bwtest::cleanOutputStream() {
        delete bwtest::_fileName;
        if (::bwtest::outputStream == file) {
            static_cast<std::ofstream&>(::bwtest::getOutputStream()
            ).close();
        }
    }

BWTestOutputType bwtest::outputStream = stdCerr;
#endif


char* bwtest::_fileName = __BW_NULL_PTR;


namespace BWTest_Names
{
//// multi-type, multi-arguments printing
////    e.g.
//// print(std::string("sdfds"), 32234, 546.546, "sdgdf", 'c');
////
#ifdef __BWTEST_HAS_ENOUGH_CXX11__
void print(){}
template<typename T, typename... Ts>
void print(const T& obj, const Ts&... others) {
    std::cout << obj << " ";
    print(others...);
}
#endif


    TestBase::
    ~TestBase() __BW_NOEXCEPT {
        delete _testName;
    }


        void TestBase::excute() {
#ifdef __BWTEST_HAS_ENOUGH_CXX11__
            auto t_start = std::chrono::high_resolution_clock::now();
            for(size_t i = 0; i != _excutionNumberOfTimes; ++i) {
                try {
                    testBody();
                }catch(std::exception& e) {
                    _stdExceptionCaught++;
                    COUT_1("\n>>>   caught std exception:\n>>> " << e.what());
                }catch(...) {
                    _otherExceptionCaught++;
                    COUT_1("\n>>>   caught other exception");
                }
            }
            auto t_end = std::chrono::high_resolution_clock::now();
            this->_timePassed = std::chrono::duration_cast<std::chrono::seconds>(t_end - t_start).count();
#else
            std::clock_t c_start = std::clock();
            for(size_t i = 0; i != _excutionNumberOfTimes; ++i) {
                try {
                    testBody();
                }catch(std::exception& e) {
                    _stdExceptionCaught++;
                    COUT_1("\n>>>  caught std exception:\n>>> " << e.what());
                }catch(...) {
                    _otherExceptionCaught++;
                    COUT_1("\n>>>  caught other exception");
                }
            }
            std::clock_t c_end = std::clock();
            this->_timePassed = (c_end - c_start) / CLOCKS_PER_SEC;
#endif
        }

        void TestBase::printReport() __BW_NOEXCEPT {
            ::bwtest::getOutputStream() << "\n>>>  test\"" << _testName
            << "\" excuted " << _excutionNumberOfTimes << "times."
            << "\n>>>  time passed: " << this->_timePassed
            << "s\n>>>  caught std exception " << _stdExceptionCaught << " times."
            << "\n>>>  caught other exception " << _otherExceptionCaught << " times." << std::endl;
        }


        TestRegister::~TestRegister() __BW_NOEXCEPT {
            for(size_t i = 0; i != _testCaseList.size(); ++i) {
                delete _testCaseList[i];
            }
            ::bwtest::cleanOutputStream();
        }
        bool TestRegister::registerTest(TestBase* newTest) {// make sure _handler has been initialized
            TestRegister::instance()->_handler->_testCaseList.push_back(newTest);
            return true;
        }

        TestRegister*
        TestRegister::instance() {
            if(TestRegister::_handler == __BW_NULL_PTR) {
                _handler = new TestRegister();
            }
            return _handler;
        }

        void TestRegister::runAllTests() {
#ifdef __BWTEST_HAS_ENOUGH_CXX11__
            auto t_start = std::chrono::high_resolution_clock::now();
            for(size_t i = 0; i != TestRegister::instance()->_handler->_testCaseList.size(); ++i) {
                _handler->_testCaseList[i]->excute();
            }
            auto t_end = std::chrono::high_resolution_clock::now();
            _handler->_totalTime
                = std::chrono::duration_cast<std::chrono::seconds>(t_end - t_start).count();
#else
            std::clock_t c_start = std::clock();
            for(size_t i = 0; i != TestRegister::instance()->_handler->_testCaseList.size(); ++i) {
                _handler->_testCaseList[i]->excute();
            }
            std::clock_t c_end = std::clock();
            _handler->_totalTime
                = (c_end - c_start) / CLOCKS_PER_SEC;
#endif
        }

        void
        TestRegister::reportAllTests() __BW_NOEXCEPT {
            ::bwtest::getOutputStream() << "\n\n=============================================\n";
            ::bwtest::getOutputStream() << ">>>\t TEST REPORT" << std::endl;
            time_t t_now = std::time(__BW_NULL_PTR);
            ::bwtest::getOutputStream() << ">>>  time: " << std::asctime(std::localtime(&t_now));
            for(size_t i = 0; i != TestRegister::instance()->_handler->_testCaseList.size(); ++i) {

                ::bwtest::getOutputStream() << "--------------------------------------";
                _handler->_testCaseList[i]->printReport();
            }

            ::bwtest::getOutputStream() << "\n>>> Time elapsed: "
                << _handler->_totalTime << " seconds" << std::endl;

            ::bwtest::getOutputStream() << "=============================================\n";
        }

TestRegister* TestRegister::_handler = __BW_NULL_PTR;

}// namespace BWTest_Names



    BWTEST_ExpectAux::BWTEST_ExpectAux(
                bool statement,
                bool expectation,
              const char* expectText0,
              const char* expectText1,
              const char* actualText,
                const char* filePath,
                int lineNumber)
    : _isFalse(statement != expectation) {

        if(_isFalse) {
           ::bwtest::getOutputStream()
                << "\n\n>>>  " << filePath
                << "\n>>>  Line: " << lineNumber
                << "\n>>>  Expect: "
                << expectText0 << expectText1
                << "\n>>>  Actual: " << actualText
                << std::endl;

        //    time_t t_now = std::time(NULL);
//            std::cerr
//             << "\n\n>>>  " << filePath
//             << "\n>>>  line: " << lineNumber
//             << "\n>>>  expect: \" "
//             << expectText0 << expectText1
//             << "\"  expect."
//             << std::endl;
         //    << "\n>>> system time: "
         //    << std::asctime(std::localtime(&t_now));
        }
    }


bool BWTEST_ExpectAux::BWTEST_bool_caughtExcepttion = false;


template<typename PrintType>
std::ostream& operator<< (const BWTEST_ExpectAux& expAux, PrintType& msg) {
    if(expAux._isFalse) {
        ::bwtest::getOutputStream() << ">>>  Message: " << msg;
        return ::bwtest::getOutputStream();
    }
    else {
       // static std::stringstream nullOS;
       // nullOS.clear();
        static BWTest_Names::basicNullOStream nullOS;// smaller than stringstream
        return nullOS;
    };
}







