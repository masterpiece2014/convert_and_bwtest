
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
#ifndef COMMENFILES_H
#define COMMENFILES_H


#ifdef __GNUC__

    #if __GNUC__*100 + __GNUC_MINOR__*10 + __GNUC_PATCHLEVEL__ > 470
        #define BWTEST_HAS_CXX11_
    #endif
    #if __GNUC__*100 + __GNUC_MINOR__*10 + __GNUC_PATCHLEVEL__ < 400
        #error "please up data your complier to GCC 4.0.0 and above"
    #endif

    #define TYPENAME_OUT    typename


    #pragma GCC diagnostic ignored "-Wreturn-type"
    #pragma GCC diagnostic ignored "-Wcomment"
    #pragma GCC diagnostic ignored "-Wunused-variable"


#else
#ifdef _MSC_VER
    #if _MSC_VER > 1700
        #define BWTEST_HAS_CXX11_
    #endif
    #if _MSC_VER < 1300
        #error "please up data your complier to VC 7.0 and above"
    #endif

    #define TYPENAME_OUT


    #pragma warning(disable: 4003) //   not enough actual parameters for macro
    #pragma warning(disable: 4996) // 'vsnprintf': This function or variable may be unsafe. Consider using vsnprintf_s instead
    #pragma warning(disable: 4715) // 'bwtest::getOutputStream' : not all control paths return a value

//    #pragma warning(disable: 4625) // nullostream : copy constructor could not be generated because a base class copy constructor is inaccessible
//4626: 'bwtest::BWTestInternal::NullOStream' : assignment operator


#endif
#endif


#ifdef  __linux__
    #define BWTEST_OS_LINUX 1
#else
    #ifdef _WIN32
        #define BWTEST_OS_WIN32 1
    #endif
#endif
//////////////////////////////////////////////////////////////////////////////////////////////
                  //  #undef BWTEST_HAS_CXX11_
//////////////////////////////////////////////////////////////////////////////////////////////





#undef BWTEST_NO_ASSIGN
#undef BWTEST_NO_COPY
#undef BW_NOEXCEPT
#undef BW_NULL_PTR

#ifdef BWTEST_HAS_CXX11_

    #define BW_NOEXCEPT noexcept
    #define BW_NULL_PTR nullptr

    #define BWTEST_NO_ASSIGN(type)\
            type& operator= (const type&) = delete;\
            type& operator= (type&& ) = delete

    #define BWTEST_NO_COPY(type)\
        type(const type&)  = delete;\
        type(type&&) = delete

#else
    #define BW_NOEXCEPT throw()
    #define BW_NULL_PTR 0

    #undef BWTEST_NO_ASSIGN
    #undef BWTEST_NO_COPY

    #define BWTEST_NO_ASSIGN(type)\
        type& operator= (const type&)

    #define BWTEST_NO_COPY(type)\
        type(const type&)

#endif


#endif // COMMENFILES_H
