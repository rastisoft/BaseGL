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
#include "RS/Common/CommonTypes.h"

namespace RS::Graphics::BaseGL
{
    class Texture
    {
    protected:
        ui8*        mImageData;
        GLenum      mFormat;
        GLuint      mTextureHandle;
        ui32        mWidth;
        ui32        mHeight;
        bool        mIsLoadedToGPU;
        bool        mIsLoadedToMemory;
        
    public:
                    Texture(const std::string_view& textureFile = "");
        virtual     ~Texture(void);

        void        loadToMemory(const std::string_view& textureFile);
        void        loadToGPU(void);
        void        loadToMemoryAndGPU(const std::string_view& textureFile);        

        void        activeAndBind(ui16 textureUnit = 0);
        void        bind(void);
        void        unbind(void);

        ui32        getWidth(void);
        ui32        getHeight(void);
        GLuint      getHandle(void);
        bool        isLoadedToGPU(void);

    };

    RS_INLINE void Texture::activeAndBind(ui16 textureUnit)
    {
        if(mTextureHandle > 0)
        {
            glActiveTexture(GL_TEXTURE0 + textureUnit);
            glBindTexture(GL_TEXTURE_2D, mTextureHandle);
        }
    }

    RS_INLINE void Texture::bind(void)
    {
        if(mTextureHandle > 0)
            glBindTexture(GL_TEXTURE_2D, mTextureHandle);
    }

    RS_INLINE void Texture::unbind(void)
    {
        if(mTextureHandle > 0)
            glBindTexture(GL_TEXTURE_2D, 0);        
    }

    RS_INLINE ui32 Texture::getWidth(void)
    {
        return mWidth;
    }

    RS_INLINE ui32 Texture::getHeight(void)
    {
        return mHeight;
    }

    RS_INLINE GLuint Texture::getHandle(void)
    {
        return mTextureHandle;
    }

    RS_INLINE bool Texture::isLoadedToGPU(void)
    {
        return mIsLoadedToGPU;
    }
}