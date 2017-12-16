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

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "RS/Graphics/BaseGL/BaseGL.h"
#include "RS/Graphics/BaseGL/Texture.h"
#include "RS/Graphics/BaseGL/Shader.h"
#include "RS/Graphics/BaseGL/Buffer.h"
#include "RS/Data/ParametersList/ParametersList.h"

namespace RS::Graphics::BaseGL
{
    struct MonitorInfo
    {
        i32 screenWidth;
        i32 screenHeight;
        i32 physicalWidth;
        i32 physicalHeight;
        i32 refreshRate;
    };

    class BaseGLApp
    {
    protected:
        //The ParameterList object that is used
        //to configure the application.
        Data::ParametersList        mConfigParameters;
        f32                         mElapsedTimeLimit;
        //Stores the time that takes to render a frame.(in millisec)
        double                      mElapsedTime{0.0};
        ui32                        mFPS{0};        
        ui32                        mFPSLimit;

         //Screen resolution.
        i32                         mScreenWidth;
        i32                         mScreenHeight;

        bool                        mIsFullScreen{false};

        i32                         mViewPortX;
        i32                         mViewPortY;
        
        //GLFW window handle.
        GLFWwindow*                 mWindow;
        i32                         mWindowWidth;
        i32                         mWindowHeight;

        MonitorInfo                 mMonitorInfo;
        
    public:
        static BaseGLApp*           baseGLAppInstance;

                                    BaseGLApp(void);
        virtual                     ~BaseGLApp(void);

        /**
            @description: Initializes the BaseGLApp. It uses mConfigParameters
            to configure openGL and other components of the application.

            @return void
        */
        virtual void                initialize(void);
        
        /**
            @description: Specifies the maximum FPS that scene should be rendered. If it is set to 0
            the scene is rendered in maximum frame rate supported by hardware.          
            @return void.
        */
        void                        setFPSLimit(ui32 fps);

        /**
            @description: Returns the FPS Limit. It is 0 the scene is rendered in maximum frame rate 
            supported by hardware.          
            @return ui32.
        */
        ui32                        getFPSLimit(void) noexcept;

        /**
            @description: Returns FPS.
            @return ui32.
        */
        ui32                        getFPS(void) noexcept;

        /**
            @description: Pure virtual function which should implement by child classes.
            This function is called by BaseGLApp class to render content. render()
            is pure virtual function which should implement by derived classes.
            @param elapsedTime: the time that takes to render a frame.(in millisec).
            @return void.
        */
        virtual void                render(double elapsedTime) = 0;

        /**
            @description: Starts the main render loop.            
            @return void.
        */
        virtual void                run(void);

        /**
            @description: GLFW window resize callback function. Whenever the window
            is resized this function is called and necessary parameters  are passed.
            @param width : the new window width.
            @param height : the new window height.

            @return void.
        */
        virtual void                windowResized(i32 width, i32 height);
    };

    RS_INLINE ui32 BaseGLApp::getFPSLimit(void) noexcept
    {
        return mFPSLimit;
    }

    RS_INLINE ui32 BaseGLApp::getFPS(void) noexcept
    {
        return mFPS;
    }
}
