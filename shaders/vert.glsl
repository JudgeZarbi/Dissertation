#version 330 core
in vec4 coord;
uniform mat4 mvp;
out vec4 texcoord;

void main(void) {
  gl_Position = mvp * vec4(coord.xyz, 1.0);
  texcoord = coord;
}
