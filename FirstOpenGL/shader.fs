#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 worldPos;          // Add this - you need world position
in float surfaceMinY;
in float surfaceMaxY;

uniform sampler2D texture1;
uniform sampler2D texture2;

uniform float ftime;
uniform float revealProgress;
uniform float revealPt1Speed;

void main()
{
    // Calculate the center Y of this surface in world space
    float surfaceYCenter = (surfaceMinY + surfaceMaxY) * 0.5;
    
    // Calculate distance from surface center along world Y axis
    float distFromSurfaceCenter = abs(worldPos.y - surfaceYCenter);
    
    // Maximum distance from center to surface edge in world Y
    float maxYDist = abs(surfaceMaxY - surfaceMinY) * 0.5;
    
    // Normalize the distance (0.0 at surface center, 1.0 at surface edges)
    float normalizedDist = distFromSurfaceCenter / maxYDist;
    
    float revealPt1Time = 1.0 / revealPt1Speed;
   
    // Discard fragments that growing threshold/veil has yet reached
    if (normalizedDist > revealProgress) {
        discard;
    }

    // Full texture mix to be revealed after full reveal commences
    vec4 textureColor = mix(texture(texture1, TexCoord), texture(texture2, vec2(1.0 - TexCoord.x, TexCoord.y)), 0.2);
    vec4 brightWhite = vec4(1.0, 1.0, 1.0, 1.0);
    
    bool fullyRevealed = (revealProgress >= 1.0); // Changed to 1.0 since we're using normalized distance
    
    if(fullyRevealed)
    {
        float transitionValue = (ftime - revealPt1Time) * 0.5;
        transitionValue = clamp(transitionValue, 0.0, 1.0);
        FragColor = mix(brightWhite, textureColor, transitionValue);
    } 
    else
    {
        FragColor = brightWhite;
    }
}