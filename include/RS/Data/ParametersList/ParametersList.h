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

#include <string>
#include <unordered_map>
#include <sstream>
#include "RS/Exception/RSException.h"
#include "RS/Common/CommonTypes.h"

#include <iostream>

namespace RS::Data
{
    typedef std::unordered_map<std::string, std::string> ListType;
    class ParametersList
    {
    protected:
        ListType        mParametersList;

    public:
        /**
            @description: ParametersList class default constructor.
            @return
        */
                        ParametersList(void) = default;
        /**
            @description: ParametersList class constructor.
            @param plfFile: plf file that the parameters should be extracted from.
            @return
        */
                        ParametersList(const std::string_view& plfFile);

                        ~ParametersList(void);
        /**
            @description: Extracts the parameters from a plf file.
            @param plfFile: PLF file that the parameters should be extracted from.
            @return: void.
        */
        void            loadFromFile(const std::string_view& plfFile);                

        /**
            @description: Adds new parameter/Updates existing ones with numerical value(int, float, double, long, unsiged int, unsiged long).
            @param parameter: the parameter name.
            @param value: value of the parameter.
            @return: void.
        */
        template <typename T>
        void set(const std::string& parameter, T value)
        {
            try
            {
                mParametersList[parameter] = std::to_string(value);
            }
            catch(std::bad_alloc)
            {
                THROW_RS_EXCEPTION("ParameterList set() : Failed to set parameter '" + parameter + "'", RSErrorCode::PL_FailedToSet);
            }
        };        

        /**
            @description: Adds new parameter/Updates existing ones with string type value. 
            @param parameter: the parameter name.
            @param value: value of the parameter.
            @return: void.
        */
        void            set(const std::string& parameter, const std::string& value);        

        /**
            @description: Adds new parameter/Updates existing ones with char* type value.
            @param parameter: the parameter name.
            @param value: value of the parameter.
            @return: void.
        */
        void            set(const std::string& parameter, const char* value);

        /**
            @description: Adds new parameter/Updates existing ones with bool type value.
            @param parameter: the parameter name.
            @param value: value of the parameter.
            @return: void.
        */
        void            set(const std::string& parameter, bool value);

        /**
            @description: Returns the number of parameters in the list.
            @return: the number of parameters in the list.
        */
        ui32            getSize(void);
        
        /**
            @description: Clears all parameters.
            @return: void.
        */
        void            clear(void);

        /**
            @description: Returns the value of a parameter.
            @param parameter: the parameter name.
            @return: value of a parameter.
        */
        template <typename T>
        T get(const std::string& parameter)
        {            
            if (T value; std::istringstream(mParametersList[parameter]) >> value)
                return value;
            else
                THROW_RS_EXCEPTION("ParameterList get() : invalid value. parameter (" + parameter + ").", RSErrorCode::PL_InvalidValue);
        };
    };

    /**
        @description: Returns the value of a parameter for bool type.
        @param parameter: the parameter name.
        @return: value of a parameter in bool type.
    */
    template <>
    bool ParametersList::get<bool>(const std::string& parameter);

    /**
        @description: Returns the value of a parameter for string type.
        @param parameter: the parameter name.
        @return: value of a parameter in string type.
    */
    template <>
    std::string ParametersList::get<std::string>(const std::string& parameter);
}