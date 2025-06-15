#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

// fragment shader needs access to texture object
uniform sampler2D texture1;
uniform sampler2D texture2;

uniform float ftime;
// growing veil from center that reveals each frag. 
// as white until all frags fully revealed
uniform float revealProgress;
uniform float revealPt1Speed;


void main()
{
    // Calculate distance from center Y for each fragment
    float distFromCenter = abs(TexCoord.y - 0.5);
    // Maximum distance from center to surface edge
    float maxDist = 0.5;
    float normalizedDist = distFromCenter / maxDist;
    float revealPt1Time = 1.0 / revealPt1Speed; // gives time in seconds for full reveal of all fragments
   
    // discard fragments that growing threshold/veil has yet reached
    if (normalizedDist > revealProgress) {
        discard;
    }

    // full texture mix to be revealed after full reveal commences
    vec4 textureColor = mix(texture(texture1, TexCoord), texture(texture2, vec2(1.0 - TexCoord.x, TexCoord.y)), 0.2); //* vec4(ourColor, 1.0); // invert x axis
    vec4 brightWhite = vec4(1.0, 1.0, 1.0, 1.0);
    bool fullyRevealed = (revealProgress >= maxDist); // true once reveal threshold has reached end
    if(fullyRevealed)
    {
        // restart time for next transition
        float transitionValue = (ftime - revealPt1Time) * 0.5; // Adjust 0.2 to control speed
        transitionValue = clamp(transitionValue, 0.0, 1.0);
        FragColor = mix(brightWhite, textureColor, transitionValue);
    } 
    // if every fragment not revealed yet, appear as white
    else
    {
        FragColor = brightWhite;
    }
   

    
    
    

}





    // sample color of texture using our texture object we setup and coords
    // output is filtered color of texture at the interpolated texture coordinate
    //FragColor = texture(ourTexture, TexCoord) * vec4(ourColor, 1.0);
    // final output color is combo of 2 txture look ups. 
    // 3rd param as 0.2 returns 80% of first input color