#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

// fragment shader needs access to texture object
uniform sampler2D texture1;
uniform sampler2D texture2;

uniform float visVal;
uniform float ftime;

void main()
{
    // sample color of texture using our texture object we setup and coords
    // output is filtered color of texture at the interpolated texture coordinate
    //FragColor = texture(ourTexture, TexCoord) * vec4(ourColor, 1.0);
    
    // final output color is combo of 2 txture look ups. 
    // 3rd param as 0.2 returns 80% of first input color

    // Calculate distance from center Y
    float distFromCenter = abs(TexCoord.y - 0.5);
    
    // Maximum distance from center to edge (for normalization)
    float maxDist = 0.5;
    
    float normalizedDist = distFromCenter / maxDist;
    
    float revealPt1Speed = 0.2;
    float revealPt1Time = 1.0 / revealPt1Speed;
    // Create reveal threshold based on time
    float revealThreshold = ftime * revealPt1Speed;
   

    // Discard fragments that haven't been "revealed" yet
    if (normalizedDist > revealThreshold) {
        discard;
    }
    vec4 textureColor = mix(texture(texture1, TexCoord), texture(texture2, vec2(1.0 - TexCoord.x, TexCoord.y)), 0.2); //* vec4(ourColor, 1.0); // invert x axis

    vec4 brightWhite = vec4(1.0, 1.0, 1.0, 1.0);
    bool fullyRevealed = (revealThreshold >= revealPt1Time / 10.0); // true when 5 seconds have past
    if(fullyRevealed)
    {
        float transitionValue = (ftime - revealPt1Time) * 0.2; // Adjust 0.2 to control speed
        transitionValue = clamp(transitionValue, 0.0, 1.0);
        FragColor = mix(brightWhite, textureColor, transitionValue);
    } 
    else
    {

        FragColor = brightWhite;
    }
   

    
    
    

}