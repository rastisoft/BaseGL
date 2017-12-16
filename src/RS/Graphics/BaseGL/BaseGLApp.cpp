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

#include "RS/Graphics/BaseGL/BaseGLApp.h"
#include "RS/Exception/RSException.h"
#include "RS/Graphics/BaseGL/Texture.h"

#include <chrono>
#include <thread>
#include <cassert>

using namespace std::chrono;
using namespace RS::Exception;

namespace RS::Graphics::BaseGL
{
    BaseGLApp* BaseGLApp::baseGLAppInstance = nullptr;

    BaseGLApp::BaseGLApp(void)
    {
        mConfigParameters.set("windowTitle", "BaseGL Application");
        mConfigParameters.set("screen.width", 1280);
        mConfigParameters.set("screen.height", 720);
        mConfigParameters.set("screen.isFullScreen", false);
    }

    BaseGLApp::~BaseGLApp(void)
    {

    }

    void BaseGLApp::initialize(void)
    {
        mScreenWidth = mConfigParameters.get<i32>("screen.width");
        mScreenHeight = mConfigParameters.get<i32>("screen.height");

        if (!glfwInit())
            THROW_RS_EXCEPTION("(BaseGLApp::initialize) : glfwInit() failed.", RSErrorCode::BGL_GLFWInitFailed);

        //TODO: Add configuration parameters for this section.
        glfwWindowHint(GLFW_SAMPLES, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        //glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        
        //Gets the primary monitor info-----------------------
        GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        glfwGetMonitorPhysicalSize(monitor, &mMonitorInfo.physicalWidth, &mMonitorInfo.physicalHeight);
        const GLFWvidmode * mode = glfwGetVideoMode(monitor);        
        mMonitorInfo.screenWidth = mode->width;
        mMonitorInfo.screenHeight = mode->height;
        mMonitorInfo.refreshRate = mode->refreshRate;
        //----------------------------------------------------

        mIsFullScreen = mConfigParameters.get<bool>("screen.isFullScreen");
        if(mIsFullScreen)
            windowResized(mMonitorInfo.screenWidth, mMonitorInfo.screenHeight);
        else
        {
            monitor = nullptr;
            windowResized(mScreenWidth, mScreenHeight);            
        }

        mWindow = glfwCreateWindow( mScreenWidth, mScreenHeight, 
                                    mConfigParameters.get<std::string>("windowTitle").c_str(),
                                    monitor, nullptr);

        if (!mWindow)
        {
            glfwTerminate();
            THROW_RS_EXCEPTION("(BaseGLApp::initialize) : glfwCreateWindow() failed.", RSErrorCode::BGL_GLFWCreateWindowFailed);
        }

        glfwMakeContextCurrent(mWindow);
        glfwPollEvents();

        glewExperimental = GL_TRUE;
        if (glewInit() != GLEW_OK)
        {
            glfwTerminate();
            THROW_RS_EXCEPTION("(BaseGLApp::initialize) : glewInit() failed.", RSErrorCode::BGL_GLEWInitFailed);
        }
    }

    void  BaseGLApp::setFPSLimit(ui32 fps)
    {
        assert(fps != 0);

        mFPSLimit = fps;
        mElapsedTimeLimit = (1000.0f / mFPSLimit);
    }

    void BaseGLApp::run(void)
    {
        GLuint vertexArrayID;
	    glGenVertexArrays(1, &vertexArrayID);
	    glBindVertexArray(vertexArrayID);

        i32 framesDone{0};
        auto startTimeFPS = steady_clock::now();
        auto lastTime = steady_clock::now();

        while (glfwGetKey(mWindow, GLFW_KEY_ESCAPE ) != GLFW_PRESS && glfwWindowShouldClose(mWindow) == 0)
        {
            glViewport(0, 0, mScreenWidth, mScreenHeight);        
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            mElapsedTime = duration<double, std::milli>(steady_clock::now() - lastTime).count();
            lastTime = steady_clock::now();
            
            if(duration<double>(lastTime - startTimeFPS).count() >= 1)
            {
                mFPS = framesDone;                
                framesDone = 0;
                startTimeFPS = steady_clock::now();
            }
            ++framesDone;

            render(mElapsedTime);

            if(mFPSLimit > 0)
                std::this_thread::sleep_for(duration<f32, std::milli>(mElapsedTimeLimit) - (steady_clock::now() - lastTime));
            
            glfwSwapBuffers(mWindow);
            glfwPollEvents();
        }

        glDeleteVertexArrays(1, &vertexArrayID);
        glfwTerminate();
    }

    void BaseGLApp::windowResized(i32 width, i32 height)
    {
        mWindowWidth = width;
        mWindowHeight = height;

        mViewPortX = (width - mScreenWidth) * 0.5f;
        mViewPortY = (height - mScreenHeight) * 0.5f;

    }
}