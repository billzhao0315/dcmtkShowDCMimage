#pragma once
#include <gl/GL.h>
#include "glext.h"
#define R(...) #__VA_ARGS__

char* vertextShader = R(
         #version 430 core\n
         layout( location = 0 ) in vec4 vPosition;
         layout( location = 1 ) in vec4 vColor;
         uniform mat4 mModelViewProjectionMatrix;
         out vec4 fragColor;
        void main()
        {
            gl_Position = mModelViewProjectionMatrix*vPosition;
            fragColor = vPosition;
        }
    );

char* fragShader = R(
         #version 430 core\n
         in vec4 fragColor;
         out vec4 outColor;
         void main()
         {
             outColor = fragColor;
         }
    );



    char* vertextIndexPlaneShader = R(
         #version 430 core\n
        layout( location = 0 ) in vec4 vPosition;
        layout( location = 1 ) in vec4 vColor;
         uniform mat4 mModelViewProjectionMatrix;
         uniform float indexPlane;
         out vec4 fragColor;
        void main()
        {
            vec4 position = vPosition;
            position.z = indexPlane;
            gl_Position = mModelViewProjectionMatrix*position;
            fragColor = vColor;
        }
    );

char* fragIndexPlaneShader = R(
         #version 430 core\n
         in vec4 fragColor;
         out vec4 outColor;
         void main()
         {
             outColor = fragColor;
         }
    );