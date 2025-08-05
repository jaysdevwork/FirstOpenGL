#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture1;

void main()
{
    // filtering settings directly change how this function samples 
    vec4 texColor = texture(texture1, TexCoords);
    // if frag too close to completely transparent, discard
    if(texColor.a < 0.1)
    {
        discard;
    }

    FragColor = texColor;
}