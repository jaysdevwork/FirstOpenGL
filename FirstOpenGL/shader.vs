#version 330 core 
layout (location = 0) in vec3 aPos; // pos var has attribute pos 0
layout (location = 1) in vec3 aColor; // color var has attribute pos 1

out vec3 ourColor; // output a color to fragment shader
uniform float offsetX;
uniform float rotAngle;

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


    gl_Position = vec4(rotatedPos.x + offsetX, rotatedPos.y, rotatedPos.z, 1.0); // can go aPos alone since vec3 or indivually .x,.y, .z
    //ourColor = aColor; // set ourColor to the input color got from vertex data (for rainbow)
    ourColor = aPos;
}