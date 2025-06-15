#version 330 core 
layout (location = 0) in vec3 aPos; // pos var has attribute pos 0
layout (location = 1) in vec3 aColor; // color var has attribute pos 1
layout (location = 2) in vec2 aTexCoord;


out vec2 TexCoord; // for forwarding coords to fragment shader
out float surfaceMinY;
out float surfaceMaxY;
out vec3 worldPos;
out vec3 objectYAxisInWorld;    // Object's Y-axis direction in world space


uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0f); // read mult from right to lefts

    // --- For orienting white reveal relative to world y axis, pass to frag shader ---
    // Hard-coded based  vertex data:
    float localMinY = -0.5;
    float localMaxY = 0.5;
    // Transform these bounds to world space
    vec4 worldMinYVec = model * vec4(0.0, localMinY, 0.0, 1.0);
    vec4 worldMaxYVec = model * vec4(0.0, localMaxY, 0.0, 1.0);
    surfaceMinY = worldMinYVec.y;
    surfaceMaxY = worldMaxYVec.y;
    worldPos = vec3(model * vec4(aPos, 1.0));
    objectYAxisInWorld = normalize(vec3(model[1]));
   
    TexCoord = aTexCoord; 
}