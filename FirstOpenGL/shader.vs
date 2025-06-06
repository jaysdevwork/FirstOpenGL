#version 330 core 
layout (location = 0) in vec3 aPos; // pos var has attribute pos 0
layout (location = 1) in vec3 aColor; // color var has attribute pos 1
layout (location = 2) in vec2 aTexCoord;

out vec3 ourColor; // output a color to fragment shader
out vec2 TexCoord; // for forwarding coords to fragment shader

uniform float offsetX;
uniform float rotAngle;

uniform mat4 transform;

void main()
{
    // rotation transformation matrix around the x axis
    mat3 rotX = mat3(
        1.0, 0.0, 0.0,                              // First column
        0.0, cos(rotAngle), sin(rotAngle),  // Second column
        0.0, -sin(rotAngle), cos(rotAngle)  // Third column
    );
    
    // mult rotation matrix by position
    vec3 rotatedPos = rotX * aPos;

    gl_Position = transform * vec4(aPos, 1.0f);
    //gl_Position = vec4(rotatedPos.x + offsetX, rotatedPos.y, rotatedPos.z, 1.0); // can go aPos alone since vec3 or indivually .x,.y, .z FOR LIVE ROT
    //gl_Position = vec4(aPos, 1.0);

    ourColor = aColor; // set ourColor to the input color got from vertex data (for rainbow)
    //ourColor = aPos;
    TexCoord = aTexCoord; 
}