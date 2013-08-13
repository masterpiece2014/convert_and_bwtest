
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



#ifndef BWTESTIO_H
#define BWTESTIO_H


#include "commen.h"

#include <iostream>
#include <fstream>

#include "nullostream.h"

namespace bwtest
{

    enum BWTestOutputType{
        stdCout,
        stdCerr,
        stdClog,
        file
    };

    static BWTestOutputType outputStream = stdCerr;
    static char* g_fileName  = BW_NULL_PTR;


    void setOutput(const char* c_str);

    std::ostream& getOutputStream();

    void cleanOutputStream();

    //// multi-type, multi-arguments printing
    ////    e.g.
    //// print(std::string("sdfds"), 32234, 546.546, "sdgdf", 'c');
    ////
    #ifdef BWTEST_HAS_CXX11_
    void print();
    
    template<typename T, typename... Ts>
    void print(const T& obj, const Ts&... others);
    #endif
    
    
} // namespace bwtest



#define put_out bwtest::getOutputStream()
#define __MARK put_out << "\n" << __FUNCTION__ << "\t" << __LINE__ << '\n';


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////  enable printing //////////////////////////////
//////////////////      in function loops and testCase.excute() ///////////////
                                #define DO_PRINT
///////////////////////////////////////////////////////////////////////////////

#define OUT_ONCE(obj)

#ifdef DO_PRINT
#undef OUT_ONCE
#define OUT_ONCE(obj) bwtest::getOutputStream() << obj

#endif

namespace bwtest {

namespace  BWTestInternal 
{

class PrintAux
{
    bool expect_failed_;

#ifdef BWTEST_HAS_CXX11_
    PrintAux() = delete;
#else
        PrintAux();
#endif
        
    BWTEST_NO_COPY(PrintAux);
    BWTEST_NO_ASSIGN(PrintAux);

public: // only one ctor is enabled
    explicit PrintAux(
                bool statement,
                bool expectation,
                const char* expectText0,
                const char* expectText1,
                const char* actualText,
                const char* filePath,
                int lineNumber,
                const char* func_name);

    bool has_failed() const BW_NOEXCEPT;

    static bool BWTEST_bool_caughtExcepttion;

};

}
}// bwtest

#include "testio.cpp"


#endif // BWTESTIO_H
