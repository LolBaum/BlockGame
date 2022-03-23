#version 330 core
layout (location = 0) out vec4 FragColor;

  
in vec2 TexCoords;

uniform sampler2D screenTexture;

vec4 kernal_sharpen(vec2 Coords, sampler2D Texture, float inverse_sharpness=3000.0){
    float offset = 1.0 / inverse_sharpness; 

    vec2 offsets[9] = vec2[](
        vec2(-offset,  offset), // top-left
        vec2( 0.0f,    offset), // top-center
        vec2( offset,  offset), // top-right
        vec2(-offset,  0.0f),   // center-left
        vec2( 0.0f,    0.0f),   // center-center
        vec2( offset,  0.0f),   // center-right
        vec2(-offset, -offset), // bottom-left
        vec2( 0.0f,   -offset), // bottom-center
        vec2( offset, -offset)  // bottom-right    
    );

    float kernel[9] = float[](
        -1, -1, -1,
        -1,  9, -1,
        -1, -1, -1
    );
    
    vec3 sampleTex[9];
    for(int i = 0; i < 9; i++)
    {
        sampleTex[i] = vec3(texture(Texture, Coords.st + offsets[i]));
    }
    vec3 col = vec3(0.0);
    for(int i = 0; i < 9; i++)
        col += sampleTex[i] * kernel[i];
    return vec4(col, 1.0);
}
vec4 inverte(vec4 c){
    vec4 color = vec4(vec3(1.0 - c.rgb), 1.0);
    return color;
}
vec4 grayscale(vec4 c){
    float average = (c.r + c.g + c.b) / 3.0;
    vec4 color = vec4(average, average, average, 1.0);
    return color;
}
vec4 weighted_grayscale(vec4 c){
    float average = 0.2126 * c.r + 0.7152 * c.g + 0.0722 * c.b;
    vec4 color = vec4(average, average, average, 1.0);
    return color;
}

void main()
{ 
    // Standard
    FragColor = texture(screenTexture, TexCoords);
    //FragColor1 = vec4(texture(screenTexture, TexCoords).x, 0, 0, 1);

    // Inverted
    //FragColor = vec4(vec3(1.0 - texture(screenTexture, TexCoords)), 1.0);

    // Grayscale
    //FragColor = texture(screenTexture, TexCoords);
    //float average = (FragColor.r + FragColor.g + FragColor.b) / 3.0;
    //FragColor = vec4(average, average, average, 1.0);

    // wieghted Grayscale
    //FragColor = texture(screenTexture, TexCoords);
    //float average = 0.2126 * FragColor.r + 0.7152 * FragColor.g + 0.0722 * FragColor.b;
    //FragColor = vec4(average, average, average, 1.0);

    // sharpening Kernal
    //FragColor = kernal_sharpen(TexCoords, screenTexture);
}