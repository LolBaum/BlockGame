#version 420 core //Frame
layout (location = 0) out vec4 FragColor;

  
in vec2 TexCoords;


layout (binding = 0) uniform sampler2D screenTexture;

// revealage threshold buffer
layout (binding = 1) uniform sampler2D u_texture_gui;


float w = 1920.0;
float h = 1080.0;
float px_w = 1.0/w/2;
float px_h = 1.0/h/2;

vec4 kernal_sharpen(vec2 Coords, sampler2D Texture, float inverse_sharpness){
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

vec4 samplePoint (vec2 pos){
//     vec4 screen =  texture(screenTexture, pos);
//     vec4 gui = texture(u_texture_gui, pos);
//     vec4 col = mix(screen, gui, gui.a);
//     return col;
return texture(u_texture_gui, pos);
}

void main()
{ 
    // Standard
    FragColor = texture(screenTexture, TexCoords);



        vec4 f0 = samplePoint(TexCoords);
        vec4 f1 = samplePoint(TexCoords + vec2(px_w, px_h));
        vec4 f2 = samplePoint(TexCoords + vec2(-px_w, px_h));
        vec4 f3 = samplePoint(TexCoords + vec2(-px_w, -px_h));
        vec4 f4 = samplePoint(TexCoords + vec2(px_w, -px_h));
    if (texture(u_texture_gui, TexCoords).a > 0.1){
        FragColor = f0;

    }else{
        vec4 screen =  texture(screenTexture, TexCoords);
        vec4 gui = (f0*4 + f1 + f2 + f3 + f4)/8;
    FragColor = mix(screen, gui, gui.a);
    }



FragColor.a = 1.0;


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