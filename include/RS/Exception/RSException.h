/*
BSD 2-Clause License

Copyright (c) 2017, Davood Rasti and Alireza Rasti - rastisoft
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#pragma once

#include "RS/Common/CommonTypes.h"
#include <string>
#include <exception>
#include "RS/Common/RSErrorCode.h"

namespace RS::Exception
{
    class RSException : public std::exception
    {
    protected:
        std::string                 mMessage;
        ulong                       mLine;
        std::string                 mFile;
        std::string                 mModule;
        std::string                 mFullMessage;
        RSErrorCode                 mErrorCode;
    public:
                                    RSException(const std::string& message, RSErrorCode errorCode, const std::string& file, i32 line);                         
        virtual                     ~RSException(void) throw();

        virtual const std::string&  getFile();
        virtual ulong               getLine();
        virtual RSErrorCode         getErrorCode();
        virtual const std::string&  getMessage();         
        
        const char*                 what() const throw();
    };
}

//TODO: try inline function instead of macro.
#define THROW_RS_EXCEPTION(message, errorCode) throw RS::Exception::RSException(message, errorCode, __FILE__, __LINE__)