#version 330 core

out vec4 FragColor;

in vec2 FragCoord;

uniform vec2 iResolution;
uniform float iTime;
uniform vec4 iMouse;

void mainImage( out vec4 fragColor, in vec2 fragCoord );

void main(){
    mainImage(FragColor, FragCoord);
}

vec3 palette(float t){
    vec3 a = vec3(0.5, 0.5, 0.5);
    vec3 b = vec3(0.5, 0.5, 0.5);
    vec3 c = vec3(1.000,1.000,1.000);
    vec3 d = vec3(0.263, 0.416, 0.557);
    

    return a + b * cos(6.28318 * (c * t + d));
}

float sdSphere(vec3 p, float s){
    return length(p) - s;
}

float sdBox(vec3 p, vec3 b){
    vec3 q = abs(p) - b;
    return length(max(q, 0.0)) + min(max(q.x, max(q.y, q.z)), 0.0);
}

float sdOctahedron(vec3 p, float s){
    p = abs(p);
    return (p.x + p.y + p.z - s) * 0.57735027;

}

float smoothUnion(float o1, float o2, float k){
    float h = clamp(0.5 + 0.5 * (o2-o1) / k, 0.0, 1.0);
    return mix(o2, o1, h) - k * h * (1.0 - h);
}

vec3 rot3D(vec3 p, vec3 axis, float angle){
    // Rodrigues' rotation formula
    return mix(dot(axis, p) * axis, p, cos(angle)) + cross(axis, p) * sin(angle);
}

float map(vec3 p){
    p.z += iTime * 0.4;

    p.xy = (fract(p.xy) - 0.5);
    p.z = mod(p.z, 0.25) - 0.125;

    float octahedron = sdOctahedron(p, 0.15);
    return octahedron;

    // return smoothUnion(sphere, box, 2.0);
    // return box;
    // return min(sphere, box);
}

void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    vec2 uv = (fragCoord * 2.0 - iResolution.xy) / iResolution.y;
    vec2 m = (iMouse.xy * 2.0 - iResolution.xy) / iResolution.y;

    // Initialization
    vec3 ro = vec3(0.0, 0.0, -3.0); // ray origin
    vec3 rd = normalize(vec3(uv*1.2, 1.0)); // ray direction
    vec3 col = vec3(0.0);

    float t = 0.0; // total distance travelled

//     camera
     ro = rot3D(ro, vec3(1.0, 0.0, 0.0), -m.y);
     rd = rot3D(rd, vec3(1.0, 0.0, 0.0), -m.y);

     ro = rot3D(ro, vec3(0.0, 1.0, 0.0), m.x);
     rd = rot3D(rd, vec3(0.0, 1.0, 0.0), m.x);

    // Raymarching
    int i;
    for(i = 0; i < 80; i++){
        vec3 p = ro + rd * t; // position along the ray
        p = rot3D(p, vec3(0.0, 0.0, 1.0), t * 0.2);

        // p.y +=  sin(t * (m.y + 1.0) * 0.5) * 0.35;
        // p.x += sin(t * (m.x + 1.0) * 0.5) * 0.35;

        float d = map(p); // current distance to next primitive

        t += d; // "march" the ray

        if(d < 0.001 || t > 100.0) break;
    }

    // col = vec3(t * 0.01);
    // col = palette(t * 0.01);
    col = palette(t * 0.04 + float(i) * 0.005);

    fragColor = vec4(col, 1.0);
}
