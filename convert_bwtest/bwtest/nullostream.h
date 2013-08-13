
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


#ifndef NULLOSTREAM_H
#define NULLOSTREAM_H

#include "commen.h"

namespace bwtest {

namespace BWTestInternal
{

///////////////////////////////////////////////////////////////////////////////////////
/////////////  two classes below are adopted from stackoverflow
/////////////   http://stackoverflow.com/questions/760301/implementing-a-no-op-stdostream
/////////////////////////////////////////////////////////////////////////////////////////////
class basicNullBuf: public std::basic_streambuf<char, TYPENAME_OUT std::char_traits<char> > {
    TYPENAME_OUT std::char_traits<char>::int_type overflow(TYPENAME_OUT std::char_traits<char>::int_type c) {
        typedef std::char_traits<char>  nameTmp;
        return nameTmp::not_eof(c);
    }
};
class NullOStream: public std::basic_ostream<char, typename std::char_traits<char> > {
    public:
        NullOStream():
        std::basic_ios<char, typename std::char_traits<char> >(&BasicNullOSBuf_),
        std::basic_ostream<char, typename std::char_traits<char> >(&BasicNullOSBuf_) {
            std::basic_ostream<char, typename std::char_traits<char> >::init(&BasicNullOSBuf_);
        }

    private:
        basicNullBuf BasicNullOSBuf_;
};

}
}
#endif // NULLOSTREAM_H
