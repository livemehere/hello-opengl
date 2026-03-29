#version 330 core

in vec3 color;
out vec4 fragment;

uniform vec4 u_color;

void main() {
  fragment = u_color;
}
