#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;

const float offset = 1.0 / 300.0;

void main()
{
	
	vec2 offsets[9] = vec2[]
	(
		vec2(-offset, offset),
		vec2(0.0f, offset),
		vec2(offset, offset),
		vec2(-offset, 0.0f),
		vec2(0.0f, 0.0f),
		vec2(offset, 0.0f),
		vec2(-offset, -offset),
		vec2(0.0f, -offset),
		vec2(offset, -offset)
	);

	float kernel[9] = float[](
		-1, -1, -1,
		-1,  9, -1,
		-1, -1, -1
	);
	

	float kernel2[9] = float[](
    1.0 / 16, 2.0 / 16, 1.0 / 16,
    2.0 / 16, 4.0 / 16, 2.0 / 16,
    1.0 / 16, 2.0 / 16, 1.0 / 16  
);

	float kernel3[9] = float[](
		1, 1, 1,
		1, -8, 1,
		1, 1, 1

	);

	// samples texture at each of the offset positions
	vec3 sampleTex[9];
	for(int i = 0; i < 9; i++)
	{
		sampleTex[i] = vec3(texture(screenTexture, TexCoords.st	+ offsets[i]));
	}


	vec3 col = vec3(0.0);
	// multiply each sampled color by its corresponding kernel weight, add em all up
	for(int i = 0; i < 9; i++)
	{
		col += sampleTex[i] * kernel[i];
	}

    
    // 2. Apply threshold - adjust 0.1 to control sensitivity
  //  float edgeStrength = length(col);
  //  if (edgeStrength < 0.1) {
  //      col = vec3(0.0); // Pure black for non-edges
  //  } else {
   //     col = vec3(1.0); // White for edges (or keep original col for colored edges)
 //   }
    

	FragColor = vec4(col, 1.0);
}