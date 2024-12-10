#version 420 core //Frame
layout (location = 0) out vec4 FragColor;

  
in vec2 TexCoords;

const float width = 1920.0f;
const float height = 1080.0f;


layout (binding = 0) uniform sampler2D screenTexture;

// revealage threshold buffer
layout (binding = 1) uniform sampler2D u_texture_gui;

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


void make_kernel(inout vec4 n[9], sampler2D tex, vec2 coord)
{
	float w = 1.0 / width;
	float h = 1.0 / height;

	n[0] = texture(tex, coord + vec2( -w, -h));
	n[1] = texture(tex, coord + vec2(0.0, -h));
	n[2] = texture(tex, coord + vec2(  w, -h));
	n[3] = texture(tex, coord + vec2( -w, 0.0));
	n[4] = texture(tex, coord);
	n[5] = texture(tex, coord + vec2(  w, 0.0));
	n[6] = texture(tex, coord + vec2( -w, h));
	n[7] = texture(tex, coord + vec2(0.0, h));
	n[8] = texture(tex, coord + vec2(  w, h));
}

vec4 sobel_edges(sampler2D tex, vec2 coord)
{
    vec4 n[9];
    make_kernel( n, screenTexture, TexCoords);

    vec4 sobel_edge_h = n[2] + (2.0*n[5]) + n[8] - (n[0] + (2.0*n[3]) + n[6]);
    vec4 sobel_edge_v = n[0] + (2.0*n[1]) + n[2] - (n[6] + (2.0*n[7]) + n[8]);
    vec4 sobel = sqrt((sobel_edge_h * sobel_edge_h) + (sobel_edge_v * sobel_edge_v));

    return sobel;
}

vec4 sobel_value(sampler2D tex, vec2 coord)
{
    vec4 sobel = sobel_edges(tex, coord);
    float value = (sobel.r + sobel.g + sobel.b) / 3;
    return vec4(value);
}

vec4 gaussian_blur(float r, int axis){
    float x,y,rr=r*r,d,w,w0;
    vec2 p=TexCoords;
    vec4 col=vec4(0.0,0.0,0.0,0.0);
    w0=0.5135/pow(r,0.96);
    if (axis==0) for (d=1.0/width,x=-r,p.x+=x*d;x<=r;x++,p.x+=d){ w=w0*exp((-x*x)/(2.0*rr)); col+=texture(screenTexture,p)*w; }
    if (axis==1) for (d=1.0/height,y=-r,p.y+=y*d;y<=r;y++,p.y+=d){ w=w0*exp((-y*y)/(2.0*rr)); col+=texture(screenTexture,p)*w; }
    return col;
}

void main()
{ 
    // Standard
    //FragColor = texture(screenTexture, TexCoords);

    vec4 sobel = sobel_value(screenTexture, TexCoords);

    FragColor = sobel;


        // NOT GOOD!!!
//     float radius = 5.0f;
//
//
//     vec4 gauss_x = gaussian_blur(radius, 0);
//     vec4 gauss_y = gaussian_blur(radius, 1);
//
//     vec4 gauss_both = mix(gauss_x, gauss_y, 0.5f);
//
//     FragColor = gauss_both;


// TRY THIS:
//     float Pi = 6.28318530718; // Pi*2
//
//     // GAUSSIAN BLUR SETTINGS {{{
//     float Directions = 16.0; // BLUR DIRECTIONS (Default 16.0 - More is better but slower)
//     float Quality = 3.0; // BLUR QUALITY (Default 4.0 - More is better but slower)
//     float Size = 8.0; // BLUR SIZE (Radius)
//     // GAUSSIAN BLUR SETTINGS }}}
//
//     vec2 Radius = Size/iResolution.xy;
//
//     // Normalized pixel coordinates (from 0 to 1)
//     vec2 uv = fragCoord/iResolution.xy;
//     // Pixel colour
//     vec4 Color = texture(iChannel0, uv);
//
//     // Blur calculations
//     for( float d=0.0; d<Pi; d+=Pi/Directions)
//     {
// 		for(float i=1.0/Quality; i<=1.0; i+=1.0/Quality)
//         {
// 			Color += texture( iChannel0, uv+vec2(cos(d),sin(d))*Radius*i);
//         }
//     }
//
//     // Output to screen
//     Color /= Quality * Directions - 15.0;
//     fragColor =  Color;


    if (texture(u_texture_gui, TexCoords).a > 0.1){
        FragColor = texture(u_texture_gui, TexCoords);
    }


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