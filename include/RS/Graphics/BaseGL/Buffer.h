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
#include "RS/Common/CommonTypes.h"

namespace RS::Graphics::BaseGL
{
    template <class T>
    class Buffer
    {
    private:
        GLuint      mBufferId;
        GLenum      mTarget;
    public:
                    Buffer(GLenum target);
        virtual     ~Buffer(void);

        void        bind(void);
        void        unbind(void);

        void        set(const T* bufferData, ui32 size);
    };

    template <typename T>
    Buffer<T>::Buffer(GLenum target): 
        mTarget(target)
        ,mBufferId(0)
    {
        glGenBuffers(1, &mBufferId);
    }

    template <typename T>
    Buffer<T>::~Buffer(void)
    {
        glDeleteBuffers(1, &mBufferId);
        mBufferId = 0;
    }

    template <typename T>
    RS_INLINE void Buffer<T>::bind(void)
    {
        glBindBuffer(mTarget, mBufferId);
    }

    template <typename T>
    RS_INLINE void Buffer<T>::unbind(void)
    {
        glBindBuffer(mTarget, 0);
    }

    template <typename T>
    RS_INLINE void Buffer<T>::set(const T* bufferData, ui32 size)
    {
        glBindBuffer(mTarget, mBufferId);
        glBufferData(mTarget, size, bufferData, GL_STATIC_DRAW);
    }
};