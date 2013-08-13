
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

#include "testio.h"

namespace bwtest
{

    void setOutput(const char * c_str) {
        if (!strncmp(c_str, "cout", 4)) {
            outputStream = stdCout;
        }
        else if (!strncmp(c_str, "cerr", 4)) {
            outputStream = stdCerr;
        }
        else if (!strncmp(c_str, "clog", 4)) {
            outputStream = stdClog;
        }
        else {
            g_fileName = const_cast<char*>(c_str);
            outputStream = file;
        }
    }

     std::ostream& getOutputStream() {
        switch(outputStream) {
        case stdCout : return std::cout;
        case stdCerr : return std::cerr;
        case stdClog : return std::clog;
        case file :
            static std::ofstream bwOStream(g_fileName, std::ios::app);
            if (bwOStream.is_open())
                return bwOStream;
            else {
                throw std::runtime_error("cannot open file");
            }
        } // switch
    }


    void cleanOutputStream() {
        if (file == outputStream) {
            static_cast<std::ofstream&>(getOutputStream()
            ).close();
        }
    }

    //// multi-type, multi-arguments printing
    ////    e.g.
    //// print(std::string("sdfds"), 32234, 546.546, "sdgdf", 'c');
    ////
#ifdef BWTEST_HAS_CXX11_
    void print(){}
    template<typename T, typename... Ts>
    void print(const T& obj, const Ts&... others) {
        std::cout << obj << " ";
        print(others...);
    }
#endif
    

namespace BWTestInternal {

PrintAux:: PrintAux(
            bool statement,
            bool expectation,
            const char* expectText0,
            const char* expectText1,
            const char* actualText,
            const char* filePath,
            int lineNumber,
            const char* func_name)

        : expect_failed_(statement != expectation) {

    if(expect_failed_) {
       bwtest::getOutputStream()
            << "\n\n>>>  " << filePath
            << "\n>>>  Line: " << lineNumber << "\t function name: " << func_name
            << "\n>>>  Expect: "
            << expectText0 << expectText1
            << "\n>>>  Actual: " << actualText
            << std::endl;
    }
}

bool PrintAux::has_failed() const BW_NOEXCEPT {
    return expect_failed_;
}


bool PrintAux::BWTEST_bool_caughtExcepttion = false;


}
} // namespace bwtest











