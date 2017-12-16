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

#include "RS/Graphics/BaseGL/Shader.h"
#include "RS/Exception/RSException.h"
#include <cassert>
#include <fstream>

using namespace RS::Exception;

namespace RS::Graphics::BaseGL
{
    Shader::~Shader(void)
    {
        glUseProgram(0);
        glDeleteShader(mVertexShaderHandle);
        glDeleteShader(mFragmentShaderHandle);
        glDeleteProgram(mProgramHandle);
        mProgramHandle = 0;
    }

    void Shader::link(void)
    {
        assert(mProgramHandle != 0);

        i32 status;
        glLinkProgram(mProgramHandle);
        glGetProgramiv(mProgramHandle, GL_LINK_STATUS, &status);

        if (status == 0)
           THROW_RS_EXCEPTION("(Shader::link) : link() failed.", RSErrorCode::BGL_ShaderLinkFailed);
    }

    void Shader::loadFile(const std::string_view& fileAddress, std::string* outString)
    {
        std::ifstream inStream(&fileAddress[0], std::ios::in);
        if(inStream.is_open())
        {
            std::string line;
            while(getline(inStream, line))
                *outString += line + "\n";

            inStream.close();
        }
        else
            THROW_RS_EXCEPTION("(Shader::loadFile) : Shader file could not be loaded.", RSErrorCode::BGL_ShaderFileLoadingFailed);
    }

    void Shader::loadCompileAndLink(const std::string_view& vertexShaderFile, const std::string_view& fragmentShaderFile)
    {
        mIsCompiled = false;

        std::string vertexShaderCode;
        std::string fragmentShaderCode;

        loadFile(vertexShaderFile, &vertexShaderCode);
        loadFile(fragmentShaderFile, &fragmentShaderCode);

        mProgramHandle = glCreateProgram();
        if(mProgramHandle == 0)
           THROW_RS_EXCEPTION("(Shader::loadCompileAndLink) : glCreateProgram().", RSErrorCode::BGL_CreatingShaderProgramFailed);

        mVertexShaderHandle = compileShader(vertexShaderCode, GL_VERTEX_SHADER);
        mFragmentShaderHandle = compileShader(fragmentShaderCode, GL_FRAGMENT_SHADER);        

        glAttachShader(mProgramHandle, mVertexShaderHandle);
        glAttachShader(mProgramHandle, mFragmentShaderHandle);

        link();
        
        mIsCompiled = true;
    }

    ui32 Shader::compileShader(const std::string_view& shaderCode, ui32 shaderType)
    {
        const ui32 shaderHandle = glCreateShader(shaderType);
        
        if(shaderHandle == 0)
            THROW_RS_EXCEPTION("(Shader::compileShader) : glCreateShader() failed.", RSErrorCode::BGL_CreatingShaderFailed);

        const char* cShaderCode = &shaderCode[0];
        glShaderSource(shaderHandle, 1, &cShaderCode, NULL);
        //glShaderSource(shaderHandle, 1, (const char**)shaderCode[0], NULL);
        glCompileShader(shaderHandle);

        i32 status;
        glGetShaderiv(shaderHandle, GL_COMPILE_STATUS, &status);

        if (status == 0)
        {
            i32 infoLength = 0;
            glGetShaderiv(shaderHandle, GL_INFO_LOG_LENGTH, &infoLength);
            if (infoLength)
            {
                //TODO: use normal string?!.
                auto infoBuffer = new char[infoLength];

                glGetShaderInfoLog(shaderHandle, infoLength, NULL, infoBuffer);
                const std::string errorMessage(infoBuffer);
                
                delete infoBuffer;
                infoBuffer = 0;
                
                THROW_RS_EXCEPTION("(Shader::compileShader) : Could not compile shader. " + errorMessage, RSErrorCode::BGL_CompilingShaderFailed);  
            }
            glDeleteShader(shaderHandle);

            THROW_RS_EXCEPTION("(Shader::compileShader) : Could not compile shader.", RSErrorCode::BGL_CompilingShaderFailed);  
        }

        return shaderHandle;
    }

    RSErrorCode Shader::addUniform(const std::string& uniform)
    {
        if(mProgramHandle == 0)
            return RSErrorCode::BGL_ShaderAddUniformFailed;

        uniformLocationsList[uniform] = glGetUniformLocation(mProgramHandle, uniform.c_str());
        
        return  RSErrorCode::Done;
    }
}