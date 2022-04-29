#version 330 core //sky
layout (location = 0) out vec4 fragColor;

in vec2 fragCoord;

uniform vec3 u_LookAt; // yaw, pitch, 0.0f

uniform mat4 InvProjection;
uniform mat4 InvView;

////////////////////////////////////////////////////////////
// TEMP   Values that uniforms in ShaderToy
//////////////////////////////////////////////////////////
const vec2 iResolution = vec2(1440.0, 900.0);
const vec4 iMouse = vec4(0);
const float iTime = 0;
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
    // vec3 dir = rayDirection(_halffov, iResolution.xy, fragCoord);
    // dir = rotation_matrix_y * rotation_matrix_x * dir;

    vec3 eye = vec3(0.0, 0.0, 0.0);

    // Ray direction function by SamCZ (https://github.com/SamCZ)

    float x = 2.0 * gl_FragCoord.x / iResolution.x - 1.0;
    float y = 2.0 * gl_FragCoord.y / iResolution.y - 1.0;
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
    vec3 c1 = vec3(0.6, 0.7, 1.0);
    vec3 c2 = vec3(0.7, 0.4, 1.0);
    vec3 c = mix(c1, c2, ray_world.y);

    
    fragColor = vec4(c, 1.0);
}

