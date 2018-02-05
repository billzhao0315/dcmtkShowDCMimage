#pragma once
#include <gl/GL.h>
#include "glext.h"
#define R(...) #__VA_ARGS__

/*************************MPR line index ---begin *****************************************/
    char* vertexLineIndexForMPR = R(
        #version 430 compatibility\n
        layout( location = 0 ) in vec4 vPosition;
        uniform vec4 vLineColor;
        uniform mat4 mModelViewProjectionMatrix;
        out vec4 fragColor;
        void main()
        {
            gl_Position = mModelViewProjectionMatrix * vPosition;
            fragColor = vLineColor;
        }
        );

    char* fragLineIndexForMPR = R(
        #version 430 compatibility\n
        in vec4 fragColor;
        out vec4 outColor;
        void main()
        {
            outColor = fragColor;
        }
        );

/*************************MPR line index ---end *****************************************/

/************************* MPR frame data ---begin***************************************/
        char* vertexFrameDataForMPR = R(
            #version 430 compatibility\n
            layout( location = 0 ) in vec4 vPosition;
            layout( location = 1 ) in vec3 vTextureCoord;
            uniform mat4 mModelViewProjectionMatrix;
            out vec3 texCoord;
            void main()
            {
                gl_Position = mModelViewProjectionMatrix*vPosition;
                texCoord = vTextureCoord;
            }
            );

        char* frahFrameDataForMPR = R(
            #version 430 compatibility\n
            uniform sampler3D baseTexture;
            in vec3 texCoord;
            out vec4 outColor;
            void main()
            {
                outColor = texture( baseTexture, texCoord );
            }
            ); 

/************************* MPR frame data ---end***************************************/