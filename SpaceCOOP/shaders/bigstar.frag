#version 130
/*
Fancy shader that builds upon star.frag to add more variables to control brightness and such, as well as adding sunbeams
*/

float rand(vec2 co){
	return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}


#define FALLOFF 10.0
#define FALL_EXP 2.0
#define COUNT_AS_STAR 0.99
#define MIN_BRIGHTNESS 0.9
#define MAX_BRIGHTNESS 20.0
//#define Color vec4(0.6118, 0.3451, 0.1922, 1.0)
#define Color vec4(0.2649, 0.3022, 0.4329, 1.0)
#define BEAM_ANG 18.0
#define MIN_BEAM_BRIGHTNESS 0.0
#define MAX_BEAM_BRIGHTNESS 0.5
#define MIN_DIST_BRIGHTNESS 0.0
#define MAX_DIST_BRIGHTNESS 1.0

#define RAD_TO_DEG 57.2958
#define PI 3.1415926535

//Gets the brightness of the beam located at the given angle
float calcAngBrightness(float ang) {
	float subAng = ang - BEAM_ANG / 2.0;
	if(subAng < 0.0) {
		subAng += 360.0;
	}
	if(subAng > 360.0) {
		subAng -= 360.0;
	}
	float beamNum = trunc(subAng / BEAM_ANG);
	return rand(vec2(beamNum, 0.0));
}

void main() {
	float d = FALLOFF * length(gl_TexCoord[0].xy);
	
	float alpha = max(0.0, 1.0 / pow(d,FALL_EXP));

	vec2 dir = normalize(gl_TexCoord[0].xy);
	float ang = (atan(dir.y, dir.x) + PI) * RAD_TO_DEG;
	float angNoise = calcAngBrightness(ang);
	float distFromBeam = 2.0 * abs(mod(ang, BEAM_ANG) - (BEAM_ANG / 2.0)) / BEAM_ANG;
	//Adjust min and max brightness
	distFromBeam *= (MAX_DIST_BRIGHTNESS - MIN_DIST_BRIGHTNESS);
	distFromBeam += MIN_DIST_BRIGHTNESS;
	angNoise *= (MAX_BEAM_BRIGHTNESS - MIN_BEAM_BRIGHTNESS);
	angNoise += MIN_BEAM_BRIGHTNESS;
	alpha += alpha * angNoise * distFromBeam;

	gl_FragColor = gl_Color * vec4(1.0, 1.0, 1.0, alpha);
}