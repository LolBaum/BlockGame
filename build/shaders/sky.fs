#version 330 core //sky
layout (location = 0) out vec4 fragColor;

in vec2 fragCoord;

uniform vec3 u_LookAt; // yaw, pitch, 0.0f

uniform mat4 InvProjection;
uniform mat4 InvView;

uniform sampler2D u_NoiseTex;

////////////////////////////////////////////////////////////
// TEMP   Values that uniforms in ShaderToy
//////////////////////////////////////////////////////////
//const vec2 iResolution = vec2(1440.0, 900.0);
//const float iTime = 0;

uniform vec2 u_Resolution;
uniform float u_Time;

const vec4 iMouse = vec4(0);
const float _fov = 91.0;
const float _halffov = _fov/2.0;
const float pi = 3.14159;


////////////////////////////////////////////////////////////
// Constant Values for RayMarching
//////////////////////////////////////////////////////////
const int MAX_MARCHING_STEPS = 255;
const float MIN_DIST = 0.0;
const float MAX_DIST = 100.0;
const float EPSILON = 0.0001;


// ChatGPT

const mat2 m2 = mat2(0.8,-0.6,0.6,0.8);

float fbm(vec2 p)
{
    float f = 0.0;
    f += 0.5000 * texture(u_NoiseTex, p/256.0).r; p = m2*p*2.02;
    f += 0.2500 * texture(u_NoiseTex, p/256.0).r; p = m2*p*2.03;
    f += 0.1250 * texture(u_NoiseTex, p/256.0).r; p = m2*p*2.01;
    f += 0.0625 * texture(u_NoiseTex, p/256.0).r;
    return f / 0.9375;
}




/////////////////////////////////
// WIP CODE  |  From ShaderToy //
/////////////////////////////////

float sphereSDF(vec3 samplePoint) {
    return length(samplePoint + vec3(0.0, 0.0, 1.0)) - 0.1;
}
float sceneSDF(vec3 samplePoint) {
    return sphereSDF(samplePoint);
}
float shortestDistanceToSurface(vec3 eye, vec3 marchingDirection, float start, float end) {
    float depth = start;
    for (int i = 0; i < MAX_MARCHING_STEPS; i++) {
        float dist = sceneSDF(eye + depth * marchingDirection);
        if (dist < EPSILON) {
			return depth;
        }
        depth += dist;
        if (depth >= end) {
            return end;
        }
    }
    return end;
}
            
vec3 rayDirection(float fieldOfView, vec2 size, vec2 fragCoord) {
    vec2 xy = fragCoord - size / 2.0;
    float z = size.y / tan(radians(fieldOfView) / 2.0);
    return normalize(vec3(xy, -z));
}

//////////////////////////////////////////////////////////////

mat3 rot_y(float a){
    return mat3(cos(a), 0.0, sin(a),
            0.0, 1.0, 0.0,
            -sin(a), 0.0, cos(a));
}
mat3 rot_x(float b){
    return mat3(1.0,      0.0,    0.0,
                0.0,   cos(b), sin(b),
                0.0,   -sin(b), cos(b));
}


mat3 rotation_matrix_y =  rot_y(radians(u_LookAt.x));
mat3 rotation_matrix_x =  rot_x(radians(u_LookAt.y));


void main()
{ 
    // vec3 dir = rayDirection(_halffov, u_Resolution.xy, fragCoord);
    // dir = rotation_matrix_y * rotation_matrix_x * dir;

    vec3 eye = vec3(0.0, 0.0, 0.0);

    // Ray direction function by SamCZ (https://github.com/SamCZ)

    float x = 2.0 * gl_FragCoord.x / u_Resolution.x - 1.0;
    float y = 2.0 * gl_FragCoord.y / u_Resolution.y - 1.0;
    vec2 ray_nds = vec2(x, y);
    vec4 ray_clip = vec4(ray_nds, -1.0, 1.0);
    vec4 ray_view = InvProjection * ray_clip;
    ray_view = vec4(ray_view.xy, -1.0, 0.0);
    vec3 ray_world = (InvView * ray_view).xyz;
    ray_world = normalize(ray_world);



/*     float dist = shortestDistanceToSurface(eye, ray_world, MIN_DIST, MAX_DIST);
    
    if (dist > MAX_DIST - EPSILON) {
        // Didn't hit anything
        fragColor = vec4(0.0, 0.0, 0.0, 0.0);
		return;
    } */




    vec3 rd = ray_world;
    vec3 col;

    // ----- Sun -----
    vec3 light = normalize(vec3(0.9, 0.1, 0.9));
    float sundot = clamp(dot(rd, light), 0.0, 1.0);

    // ----- Sky gradient -----
    vec3 blueSky = vec3(0.3, 0.55, 0.8);
    vec3 redSky  = vec3(0.8, 0.8, 0.6);
    vec3 sky = mix(blueSky, redSky, 1.5 * pow(sundot, 8.0));

    col = sky * (1.0 - 0.8 * rd.y);

    // ----- Clouds -----
    if(rd.y > 0.00) // only draw above horizon
    {
        float cloudSpeed = 0.01;
        float cloudFlux  = 0.5;

        vec3 cloudColour = mix(vec3(1.0,0.95,1.0),
                               0.35 * redSky,
                               pow(sundot, 2.0));

        // Fake camera position (since skybox)
        vec3 ro = vec3(0.0, 50.0, 0.0);

        vec2 sc = cloudSpeed * 50.0 * u_Time * ro.xz
                + rd.xz * (1000.0 - ro.y) / rd.y;

        float cloud = fbm(0.0005 * sc +
                         fbm(0.0005 * sc + u_Time * cloudFlux));

        float cloudMask = 0.5 * smoothstep(0.5, 0.8, cloud);

        col = mix(col, cloudColour, cloudMask);
    }

    // ----- Sun glow -----
    col += 0.1 * vec3(0.9, 0.3, 0.9) * pow(sundot, 0.5);
    col += 0.2 * vec3(1.0, 0.7, 0.7) * pow(sundot, 1.0);
    col += 0.95 * vec3(1.0) * pow(sundot, 256.0);

    
    fragColor = vec4(col, 1.0);
}

