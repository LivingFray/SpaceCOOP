

#define FALLOFF 10.0

void main() {
	
	float d = FALLOFF * length(gl_TexCoord[0].xy);
	
	float alpha = max(0.0, 1.0 / (d * d));
	
	gl_FragColor = gl_Color * vec4(1.0, 1.0, 1.0, alpha);
}