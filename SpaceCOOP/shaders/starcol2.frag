#define EXP 4.0
#define MULT 1.0
#define CONST 0.75
#define IN_MULT 0.4

uniform sampler2D tex;

void main() {
	vec4 pixel = texture2D(tex, gl_TexCoord[0].xy);
    
    float cIN = pixel.r * IN_MULT;
    
    float c = (MULT) * pow(cIN + CONST,EXP);
    
    vec3 col = vec3(c,c,c);

    // Output to screen
    gl_FragColor = vec4(col * gl_Color, pixel.a);
}