#version 330 core
in vec4 texcoord;
uniform sampler2DArray texarray;

void main(void) {
	vec2 coord2d = vec2(fract(texcoord.x + texcoord.z), -texcoord.y);
 	gl_FragColor = texture(texarray, vec3(fract(texcoord.x + texcoord.z), -texcoord.y, texcoord.w));
}