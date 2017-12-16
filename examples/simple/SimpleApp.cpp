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

#include "SimpleApp.h"
#include <RS/Graphics/BaseGL/Buffer.h>

using namespace RS;
using namespace RS::Graphics::BaseGL;

SimpleApp::SimpleApp(void)
{
}

SimpleApp::~SimpleApp(void)
{
}

void  SimpleApp::initialize(void)
{
    mConfigParameters.set("windowTitle", "Hello world application");
    mConfigParameters.set("screen.width", 512);
    mConfigParameters.set("screen.height", 512);

    BaseGLApp::initialize();

    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

    mShader.loadCompileAndLink("../Data/Shaders/simpleShader.vert", "../Data/Shaders/simpleShader.frag");

    mPositionLocation = mShader.getAttribLocation("position");
    mUVLocation = mShader.getAttribLocation("uv");
    mTextureLocation = mShader.getUniformLocation("textureSampler");

    constexpr GLfloat verticesBufferData[] = { 
        -1.0f, -1.0f, 0.0f, 1.0f,
        1.0f, -1.0f, 1.0f, 1.0f,
        -1.0f,  1.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 1.0f, 0.0f
    };

    constexpr GLushort indicesBufferData[] = {0, 1, 3, 0, 3, 2};
    
    mVBO = std::make_unique<Buffer<f32>>(GL_ARRAY_BUFFER);
    mVBO->set(verticesBufferData, sizeof(verticesBufferData));

    mIBO = std::make_unique<Buffer<ui16>>(GL_ELEMENT_ARRAY_BUFFER);
    mIBO->set(indicesBufferData, sizeof(indicesBufferData));

    mBackgroundTexture = std::make_unique<Texture>("../Data/Images/hello_world.png");
    mBackgroundTexture->loadToGPU();

}

void  SimpleApp::render(double elapsedTime)
{
    mShader.use();
    mVBO->bind();
    mIBO->bind();
    
    glEnableVertexAttribArray(mPositionLocation);
    glVertexAttribPointer(mPositionLocation, 2, GL_FLOAT, GL_FALSE, 16, 0);
    
    glEnableVertexAttribArray(mUVLocation);
    glVertexAttribPointer(mUVLocation, 2, GL_FLOAT, GL_FALSE, 16, (void*)(8));        
    
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);

    glDisableVertexAttribArray(mPositionLocation);
    glDisableVertexAttribArray(mUVLocation);

    mVBO->unbind();
    mIBO->unbind();
}