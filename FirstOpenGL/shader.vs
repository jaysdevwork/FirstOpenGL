#version 330 core 
layout (location = 0) in vec3 aPos; // pos var has attribute pos 0
layout (location = 1) in vec3 aColor; // color var has attribute pos 1

out vec3 ourColor; // output a color to fragment shader

void main()
{
    gl_Position = vec4(aPos, 1.0); // can go aPos alone since vec3 or indivually .x,.y, .z
    ourColor = aColor; // set ourColor to the input color got from vertex data 
}