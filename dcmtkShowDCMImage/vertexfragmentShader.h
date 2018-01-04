#pragma once
#include <gl/GL.h>
#include "glext.h"
#define R(...) #__VA_ARGS__

/*************************demoCube---begin************************************/
/*the shader show the demo cube, we use the postion data as the color for that the their data is same form 0.0 to 1.0 */

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

/*************************demoCube---end************************************/

/*************************indexPlane for demoCube---begin************************************/
    /*the shader show the index plane, and move the position for z by the uniform variant indexPlane*/
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
/*************************indexPlane for demoCube---end************************************/