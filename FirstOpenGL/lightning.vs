#version 330 core
layout (location = 0) in vec3 aPos;   // the position variable has attribute position 0
  
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float time;

void main()
{
   
    
    vec3 pos = aPos;
    
    // Create jittery motion that changes direction sporadically
    float timeScale = time * 6.0;
    float vertexPhase = float(gl_VertexID) * 2.3;
    
    // Sudden direction changes
    float direction = sign(sin(timeScale * 0.3 + vertexPhase));
    float intensity = abs(sin(timeScale + vertexPhase * 1.7));
    
    pos.y += direction * intensity * 0.25;
    
    gl_Position = projection * view * model * vec4(pos, 1.0);
    
}       


    // wavey motion
    // Each vertex gets different offset based on its index (0, 1, 2, 3, 4, 5)
    //float vertexOffset = sin(float(gl_VertexID) * 2.0) * 0.2; for static
    //float vertexOffset = sin(float(gl_VertexID) * 2.0 + time * 5.0) * 0.2;
    //pos.y += vertexOffset;