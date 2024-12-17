#version 330 core

out vec4 FragColor;

in vec2 FragCoord;

uniform vec2 iResolution;
uniform float iTime;

void main(){
    vec2 uv = (FragCoord * 2.0 - iResolution.xy) / iResolution.y;
//    vec2 uv = FragCoord / iResolution.xy;
//    uv -= 0.5;
//    uv *= 2.0;
//    uv.x *= iResolution.x / iResolution.y;

//    vec2 uv = FragCoord;
//    uv.x *= (iResolution.x / iResolution.y);

    float d = length(uv);
    d -= sin(iTime)+1.0;

    vec3 color = d > 0.0 ? vec3(0.0, 1.0, 0.0) : vec3(1.0, 0.0, 0.0);
    
    color *= min(abs(d)*7.0, 1.0);

    d = 1.0 + sin(d*100.0) / 2.0;
    
    FragColor = vec4(d * color, 1.0);
}
