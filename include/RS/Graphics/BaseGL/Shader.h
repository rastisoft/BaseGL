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

#include <GL/glew.h>
#include <string>
#include <unordered_map>
#include "RS/Common/CommonTypes.h"
#include "RS/Common/RSErrorCode.h"

#include <iostream>

namespace RS::Graphics::BaseGL
{
    class Shader
    {
    protected:
        std::unordered_map<std::string, GLint> uniformLocationsList;
        bool        mIsCompiled{false};
        ui32        mProgramHandle{0};
        ui32        mVertexShaderHandle;
        ui32        mFragmentShaderHandle;
        
        void        loadFile(const std::string_view& fileAddress, std::string* outString);
        ui32        compileShader(const std::string_view& shaderCode, ui32 shaderType);

    public:
                    Shader(void) = default;
        virtual     ~Shader(void);

        void        link(void);
        void        loadCompileAndLink(const std::string_view& vertexShaderFile, const std::string_view& fragmentShaderFile);
        void        use(void);

        void        bindAttribLocation(GLint location, const std::string& attribute);
        RSErrorCode addUniform(const std::string& uniform);

        void        setUniformMatrix4fv(const std::string& uniform, const GLfloat* value, GLsizei count = 1, GLboolean transpose = GL_FALSE);
        void        setUniformMatrix3fv(const std::string& uniform, const GLfloat* value, GLsizei count = 1, GLboolean transpose = GL_FALSE);
        void        setUniform3f(const std::string& uniform, GLfloat value1, GLfloat value2, GLfloat value3);
        void        setUniform1f(const std::string& uniform, GLfloat value);
        void        setUniform1i(const std::string& uniform, GLint value);

        ui32        getProgramHandle(void);
        GLint       getAttribLocation(const std::string_view& attribute);
        GLint       getUniformLocation(const std::string_view& uniform);
    };

    RS_INLINE void Shader::bindAttribLocation(GLint location, const std::string& attribute)
    {
        glBindAttribLocation(mProgramHandle, location, attribute.c_str());
    }

    RS_INLINE void Shader::setUniformMatrix4fv(const std::string& uniform, const GLfloat* value, GLsizei count, GLboolean transpose)
    {
        glUniformMatrix4fv(uniformLocationsList[uniform] , count, transpose, value);
        // std::cout << uniformLocationsList[uniform] << "--\n";
    }

    RS_INLINE void Shader::setUniformMatrix3fv(const std::string& uniform, const GLfloat* value, GLsizei count, GLboolean transpose)
    {
        glUniformMatrix3fv(uniformLocationsList[uniform] , count, transpose, value);
    }

    RS_INLINE void Shader::setUniform3f(const std::string& uniform, GLfloat value1, GLfloat value2, GLfloat value3)
    {
        glUniform3f(uniformLocationsList[uniform], value1, value2, value3);
    }

    RS_INLINE void Shader::setUniform1f(const std::string& uniform, GLfloat value)
    {
        glUniform1f(uniformLocationsList[uniform], value);
    }

    RS_INLINE void Shader::setUniform1i(const std::string& uniform, GLint value)
    {
        glUniform1i(uniformLocationsList[uniform], value);
    }

    RS_INLINE ui32 Shader::getProgramHandle(void)
    {
        return mProgramHandle;
    }

    RS_INLINE void Shader::use(void)
    {
        glUseProgram(mProgramHandle);
    }

    RS_INLINE GLint Shader::getAttribLocation(const std::string_view& attribute)
    {
        return glGetAttribLocation(mProgramHandle, &attribute[0]);
    }

    RS_INLINE GLint Shader::getUniformLocation(const std::string_view& uniform)
    {
        return glGetUniformLocation(mProgramHandle, &uniform[0]);
    }
}