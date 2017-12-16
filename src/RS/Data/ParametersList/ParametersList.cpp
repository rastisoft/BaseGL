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

#include "RS/Data/ParametersList/ParametersList.h"
#include "RS/Utility/String.h"

#include <fstream>
#include <iostream>
#include <vector>
#include <utility>

namespace RS::Data
{
    using namespace RS::Utility;
   
    ParametersList::ParametersList(const std::string_view& plfFile)
    {
        loadFromFile(plfFile);
    }

    ParametersList::~ParametersList(void)
    {        
    }

    void ParametersList::loadFromFile(const std::string_view& plfFile)
    {
        std::fstream inStream(&plfFile[0], std::fstream::in);
        if (!inStream.is_open())
            THROW_RS_EXCEPTION("ParametersList loadFile : " + std::string(plfFile) + " could not be opened. ", RSErrorCode::FailToOpenFile);

        std::string key;
        std::string value;    
        bool isKeyValueSeparatorFound{false};
        bool commentCharacterFound{false};
        std::vector<std::string> keysList;    
        char character;
        char lastCharacter{0};

        while (inStream >> std::noskipws >> character)
        {    
            if(commentCharacterFound)
            {
                commentCharacterFound = !(character == '\n' || character == '\r');
                continue;
            }

            if(character == '\n' || character == '\r' || character == '\t')
                continue;

            if(character == '/')
                commentCharacterFound = (lastCharacter == '/');
            else if(isKeyValueSeparatorFound)
            {
                if(character == ';')
                {
                    key = String::trim(key);
                    value = String::trim(value);
                    if(!key.empty() && !value.empty())
                    {
                        std::string parentKey;
                        for(const auto& keyItem : keysList)
                            parentKey += keyItem + '.';
                            
                        mParametersList[parentKey + key] = value;
                        key.clear();
                        value.clear();
                        isKeyValueSeparatorFound = false;                        
                    }
                }
                else if(character == '{')
                {
                    keysList.push_back(std::move(String::trim(key)));
                    key.clear();
                    isKeyValueSeparatorFound = false;
                }
                else if(character == '}')
                    keysList.pop_back();
                else
                    value += character;
            }
            else
            {
                if(character == '=')
                    isKeyValueSeparatorFound = true;
                else if(character == '}')
                    keysList.pop_back();
                else
                    key += character;
            }
            
            lastCharacter = character;
        }

        inStream.close();
    }    

    void ParametersList::set(const std::string& parameter, const std::string& value)
    {
        mParametersList[parameter] = value;
    }
    
    void ParametersList::set(const std::string& parameter, const char* value)
    {
        mParametersList[parameter] = value;
    };

    void ParametersList::set(const std::string& parameter, bool value)
    {
        mParametersList[parameter] = (value) ? "true" : "false";
    };

    template <>
    bool ParametersList::get<bool>(const std::string& parameter)
    {
        if(mParametersList[parameter] == "true" || mParametersList[parameter] == "false")
            return (mParametersList[parameter] == "true");
        else
            THROW_RS_EXCEPTION("ParametersList get() : invalid boolean value for parameter (" + parameter + ").", RSErrorCode::PL_InvalidValue);
    }

    template <>
    std::string ParametersList::get<std::string>(const std::string& parameter)
    {
        return mParametersList[parameter];
    }

    ui32 ParametersList::getSize(void)
    {
        return mParametersList.size();
    }    

    void ParametersList::clear(void)
    {
        mParametersList.clear();
    }
}