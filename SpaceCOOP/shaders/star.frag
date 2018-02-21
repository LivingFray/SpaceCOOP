
in vec2 centre;

#define FALLOFF 10.0
#define SCALE 2.0

/*
Distance, d
Falloff, f
Brightness, B = 1/(d*d * f * f)
When d = 0, B = 1
When d = 1, B ~= 0
When d = 1, B < 1/255
1/255 = 1/(1 * 1 * f * f)
f = sqrt(255) ~= sqrt(256) = sqrt(2^8) = 2^4 = 16
*/

void main() {
	
	float d = FALLOFF * length(gl_TexCoord[0].xy);
	
	float alpha = max(0.0, 1.0 / (d * d));
	
	gl_FragColor = gl_Color * vec4(1.0, 1.0, 1.0, alpha);
}