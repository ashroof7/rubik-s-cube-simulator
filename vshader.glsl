#version 330

in vec3 vPosition;
in vec4 in_Color;

out vec4 ex_Color;

uniform mat4 P;
uniform mat4 M;
uniform mat4 V;
uniform mat4 S;

void main(){
    gl_Position = (P * V * S * M) * vec4(vPosition, 1.0);
    ex_Color = in_Color;
}

    