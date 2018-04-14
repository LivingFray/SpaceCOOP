#version 130

uniform sampler2D tex;

vec4 invert(vec4 c) {
    return vec4(1.0 - c.x, 1.0 - c.y, 1.0 - c.z, 1.0 - c.w);
}

vec4 burn(vec4 bottom, vec4 top) {
    return invert(invert(bottom) / top);
}

void main() {
	vec4 pixel = texture2D(tex, gl_TexCoord[0].xy);

	gl_FragColor = burn(pixel, gl_Color);
}