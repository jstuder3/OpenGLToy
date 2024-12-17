#version 330 core

layout (location = 0) in vec2 aPos;

out vec2 FragCoord;

uniform vec2 iResolution;

void main(){
	gl_Position = vec4(aPos, 0.0, 1.0);
	vec2 scaledPos = ((aPos.xy + 1.0) / 2.0) * iResolution.xy;
	FragCoord = scaledPos;
}
