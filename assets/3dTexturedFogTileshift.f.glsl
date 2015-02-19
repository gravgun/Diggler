#version 120

varying vec4 v_coord;
varying vec2 v_texcoord;
uniform sampler2D texture;
uniform vec2 tileshift;
float fogEnd = 32.0, fogStart = 16.0;

void main(void) {
	float fogCoord = (gl_FragCoord.z/gl_FragCoord.w);
	gl_FragColor = mix(texture2D(texture, v_texcoord), vec4(0.0, 0.0, 0.0, 1.0), 1.0-clamp((fogEnd-fogCoord)/(fogEnd-fogStart), 0.0, 1.0));
}