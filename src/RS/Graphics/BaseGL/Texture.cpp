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

#include "RS/Graphics/BaseGL/Texture.h"
#include "RS/Exception/RSException.h"
#include "RS/Graphics/BaseGL/3rdparty/stb/stb_image.h"

#include <cassert>
#include <fstream>
#include <vector>

#include <iostream>

using namespace RS::Exception;

namespace RS::Graphics::BaseGL
{
    Texture::Texture(const std::string_view& textureFile) :
        mTextureHandle(0),
        mIsLoadedToGPU(false)
    {
        glGenTextures(1, &mTextureHandle);

        if(mTextureHandle < 0)
            THROW_RS_EXCEPTION("(Texture) : generating texture failed.", RSErrorCode::BGL_GeneratingTextureFailed);

        glBindTexture(GL_TEXTURE_2D, mTextureHandle);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        if(textureFile != "")
              loadToMemory(textureFile);
    }

    Texture::~Texture(void)
    {
        if(!mIsLoadedToGPU)
            return;

        const GLuint textureHandles[] = {mTextureHandle};
        glDeleteTextures(1, textureHandles);
        mTextureHandle = 0;

        mIsLoadedToGPU = false;
    }

    void Texture::loadToMemory(const std::string_view& textureFile)
    {
        std::fstream inStream(&textureFile[0], std::ios::in|std::ios::binary|std::ios::ate);
        if (!inStream.is_open())
        {
            THROW_RS_EXCEPTION("(Texture::loadToMemory) : image file could not be opened.", RSErrorCode::FailToOpenFile);
            return;
        }

        std::streampos size = inStream.tellg();
        std::vector<char> memblock = std::vector<char>(size, '\0');
        inStream.seekg (0, std::ios::beg);
        inStream.read (&memblock[0], size);
        inStream.close();

        i32 width{0};
        i32 height{0};
        i32 numberComponents;

        // mImageData = stbi_load_from_memory((const unsigned char*)&memblock[0], size, &width, &height,
        //                                             &numberComponents, 0);

        mImageData = stbi_load(&textureFile[0], &width, &height, &numberComponents, 0);

        mWidth = static_cast<ui32>(width);
        mHeight = static_cast<ui32>(height);

        mFormat = [numberComponents](void) -> GLenum
        {
            switch (numberComponents)
            {
                case 1:
                    return GL_LUMINANCE;
                case 2:
                    return GL_LUMINANCE_ALPHA;
                case 3:
                    return GL_RGB;
                case 4:
                    return GL_RGBA;
                default:
                    return 0;
            }
        }();

        if(mFormat == 0)
            assert(0);
        else
            mIsLoadedToMemory = true;
    }

    void Texture::loadToGPU(void)
    {
        if(!mImageData)
            THROW_RS_EXCEPTION("(Texture::loadToGPU) : The image has not loaded in the memory yet. Use LoadToMemory() to load image to the memory or use LoadToMemoryAndGPU() instead.",
                                RSErrorCode::BGL_ImageDataHasNotBeenLoaded);

        // activeAndBind();
        bind();
        glTexImage2D(GL_TEXTURE_2D, 0, mFormat, mWidth, mHeight, 0, mFormat, GL_UNSIGNED_BYTE, mImageData);
        mIsLoadedToGPU = true;
    }

    void Texture::loadToMemoryAndGPU(const std::string_view& textureFile)
    {
        loadToMemory(textureFile);
        loadToGPU();
    }
}