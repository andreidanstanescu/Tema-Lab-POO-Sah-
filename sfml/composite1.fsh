#version 120

#define MAX_COLOR_RANGE 48.0
/*
!! DO NOT REMOVE !!
This code is from Chocapic13' shaders
Read the terms of modification and sharing before changing something below please !
!! DO NOT REMOVE !!
*/

//////////////////////////////ADJUSTABLE VARIABLES
//////////////////////////////ADJUSTABLE VARIABLES
//////////////////////////////ADJUSTABLE VARIABLES

	#define GODRAYS			//in this step previous godrays result is blurred
		const float exposure = 1.5;			//godrays intensity 15.0 is default
		const float density = 1.0;			
		const int NUM_SAMPLES = 7;			//increase this for better quality at the cost of performance /8 is default
		const float grnoise = 0.0;		//amount of noise /0.0 is default
		
	#define WATER_REFLECTIONS			
		#define REFLECTION_STRENGTH 1.5

//////////////////////////////END OF ADJUSTABLE VARIABLES
//////////////////////////////END OF ADJUSTABLE VARIABLES
//////////////////////////////END OF ADJUSTABLE VARIABLES


//don't touch these lines if you don't know what you do!
const int maxf = 4;				//number of refinements
const float stp = 1.2;			//size of one step for raytracing algorithm
const float ref = 0.1;			//refinement multiplier
const float inc = 2.2;			//increasement factor at each step

//ground constants (lower quality)
const int Gmaxf = 3;				//number of refinements
const float Gstp = 1.2;			//size of one step for raytracing algorithm
const float Gref = 0.11;			//refinement multiplier
const float Ginc = 3.0;			//increasement factor at each step

varying vec4 texcoord;

varying vec3 lightVector;
varying vec3 sunVec;
varying vec3 moonVec;
varying vec3 upVec;

varying vec3 sunlight;
varying vec3 moonlight;
varying vec3 ambient_color;

varying float eyeAdapt;

varying float SdotU;
varying float MdotU;
varying float sunVisibility;
varying float moonVisibility;

uniform sampler2D composite;
uniform sampler2D gaux1;
uniform sampler2D depthtex0;
uniform sampler2D depthtex1;
uniform sampler2D gnormal;
uniform sampler2D gdepth;
uniform sampler2D noisetex;

uniform vec3 sunPosition;
uniform vec3 moonPosition;
uniform vec3 upPosition;
uniform vec3 cameraPosition;
uniform vec3 skyColor;

uniform mat4 gbufferProjection;
uniform mat4 gbufferProjectionInverse;
uniform mat4 gbufferModelViewInverse;

uniform int isEyeInWater;
uniform int worldTime;
uniform float far;
uniform float near;
uniform float aspectRatio;
uniform float viewWidth;
uniform float viewHeight;
uniform float rainStrength;
uniform float wetness;
uniform float frameTimeCounter;
uniform int fogMode;

float pw = 1.0/ viewWidth;
float ph = 1.0/ viewHeight;
float matflag = texture2D(gaux1,texcoord.xy).g;

vec3 fragpos = vec3(texcoord.st, texture2D(depthtex0, texcoord.st).r);
vec3 normal = texture2D(gnormal, texcoord.st).rgb * 2.0 - 1.0;

float time = float(worldTime);
float night = clamp((time-12000.0)/300.0,0.0,1.0)-clamp((time-22800.0)/200.0,0.0,1.0);

float sky_lightmap = texture2D(gaux1,texcoord.xy).r;
	
vec4 color = texture2D(composite,texcoord.xy);


vec3 nvec3(vec4 pos) {
    return pos.xyz/pos.w;
}

vec4 nvec4(vec3 pos) {
    return vec4(pos.xyz, 1.0);
}

float cdist(vec2 coord) {
	return max(abs(coord.s-0.5),abs(coord.t-0.5))*2.0;
}


vec3 getSkyColor(vec3 fposition) {
//sky gradient
/*----------*/
vec3 skycoaa = ivec3(0,125,300)/250.0;
vec3 sky_color = pow(skycoaa,vec3(2.0));
vec3 nsunlight = normalize(pow(sunlight,vec3(0.2)));
vec3 sVector = normalize(fposition);
vec3 upVector = normalize(upPosition);

sky_color = normalize(mix(sky_color,vec3(0.15,0.3,0.4)*length(ambient_color),rainStrength)); //normalize colors in order to don't change luminance

float Lz = 1.0;
float cosT = dot(sVector,upVec); 
float absCosT = max(cosT,0.0);
float cosS = dot(sunVec,upVec);
float S = acos(cosS);				
float cosY = dot(sunVec,sVector);
float Y = acos(cosY);				
float a = -0.8;
float b = -0.2;
float c = 7.0;
float d = -0.8;
float e = 3.0;

//sun sky color
float L =  (1+a*exp(b/(absCosT+0.2)))*(1+c*exp(d*Y)+e*cosY*cosY); 
L = pow(L,1.7-rainStrength*0.8)*(1.0-rainStrength*0.2); //modulate intensity when raining
vec3 skyColorSun = mix(sky_color, nsunlight,1-exp(-0.19*L*(1-rainStrength*0.8)))*L*0.5 ; //affect color based on luminance (0% physically accurate)
skyColorSun *= sunVisibility;

//moon sky color
float McosS = MdotU;
float MS = acos(McosS);
float McosY = dot(moonVec,sVector);
float MY = acos(McosY);

float L2 =  (1+a*exp(b/(absCosT+0.01)))*(1+c*exp(d*MY)+e*McosY*McosY);
L2 = pow(L2,1.0-rainStrength*0.8)*(1.0-rainStrength*0.2); //modulate intensity when raining
vec3 skyColormoon = mix(moonlight,normalize(vec3(0.25,0.3,0.4))*length(moonlight),rainStrength)*L2*0.4 ; //affect color based on luminance (0% physically accurate)
skyColormoon *= moonVisibility;

sky_color = skyColormoon*2.0+skyColorSun;
//sky_color = vec3(Lc);
/*----------*/
return sky_color;
}


vec3 drawSun(vec3 fposition,vec3 color,int land) {
vec3 sVector = normalize(fposition);

float angle = (1-max(dot(sVector,sunVec),0.0))*250.0;
float sun = exp(-angle*angle);
sun *= land*(1-rainStrength*0.)*sunVisibility;
vec3 sunlight = mix(sunlight,vec3(0.25,0.3,0.4)*length(ambient_color)*4.,rainStrength*0.8);

return mix(color,sunlight*40.,sun);

}

vec3 calcFog(vec3 fposition, vec3 color, vec3 fogclr) {
	const float density = 325.0;
	const float start = 0.3;
	float rainFog = 0.8+0.8*rainStrength;
	float fog = min(exp(-length(fposition)/density/(sunVisibility*0.1+0.7)*rainFog)+start*sunVisibility*(1-rainStrength),1.0);
	
	vec3 fc = fogclr*0.8;
	return mix(fc,color,fog);
}

vec4 raytrace(vec3 fragpos, vec3 normal,vec3 fogclr) {
    vec4 color = vec4(0.0);
    vec3 start = fragpos;
    vec3 rvector = normalize(reflect(normalize(fragpos), normalize(normal)));
    vec3 vector = stp * rvector;
    vec3 oldpos = fragpos;
    fragpos += vector;
	vec3 tvector = vector;
    int sr = 0;
    for(int i=0;i<40;i++){
        vec3 pos = nvec3(gbufferProjection * nvec4(fragpos)) * 0.5 + 0.5;
        if(pos.x < 0 || pos.x > 1 || pos.y < 0 || pos.y > 1 || pos.z < 0 || pos.z > 1.0) break;
        vec3 spos = vec3(pos.st, texture2D(depthtex1, pos.st).r);
        spos = nvec3(gbufferProjectionInverse * nvec4(spos * 2.0 - 1.0));
        float err = abs(fragpos.z-spos.z);
if(err < pow(length(vector)*1.85,1.15)){
	
                sr++;
                if(sr >= maxf){
                    float border = clamp(1.0 - pow(cdist(pos.st), 20.0), 0.0, 1.0);
                    color = texture2D(composite, pos.st);
					float land = texture2D(gaux1, pos.st).g;
					land = float(land < 0.03);
					spos.z = mix(fragpos.z,2000.0*(0.4+sunVisibility*0.6),land);
					color.rgb = calcFog(spos,pow(color.rgb,vec3(2.2))*MAX_COLOR_RANGE,fogclr);
					color.a = 1.0;
                    color.a *= border;
                    break;
                }
				tvector -=vector;
                vector *=ref;
				
        
}
        vector *= inc;
        oldpos = fragpos;
        tvector += vector;
		fragpos = start + tvector;
    }
    return color;
	
}	

vec3 drawCloud(vec3 fposition,vec3 color) {
vec3 sVector = normalize(fposition);
float cosT = dot(sVector,upVec);
float McosY = MdotU;
float cosY = SdotU;
//cloud generation
/*----------*/
vec3 tpos = vec3(gbufferModelViewInverse * vec4(fposition,1.0));
vec3 wvec = normalize(tpos);
vec3 wVector = normalize(tpos);
vec3 intersection = wVector*(0.0/(wVector.y));

//float canHit = length(intersection)-length(tpos);

	vec2 wind = vec2(frameTimeCounter*(cos(frameTimeCounter/1000.0)+0.5),frameTimeCounter*(sin(frameTimeCounter/1000.0)+0.5));
	
	
	vec3 wpos = tpos.xyz+cameraPosition;
	vec2 coord1 = (intersection.xz+ 1.2*cosT*cosT*intersection.xz)/768.0/32.+wind/8192*0.5;
	vec2 coord = sin(coord1.yx*6.28);
	float noise = texture2D(noisetex,coord).x;
	
	const float scale = 3.;
	float mult = 0.0;
	float r = 3.;
	float tmult = 1.0;

	  float N = 0.0;
vec3 cloud_color = moonlight*0.0*moonVisibility*(1-rainStrength*0.0) + sunlight*0.0*sunVisibility*(1-rainStrength*0.0) + ambient_color + sunlight*.0*pow(max(cosY,0.0),N)*(N+1)/0.0  * (1-rainStrength)*sunVisibility + moonlight*0.0*pow(max(McosY,0.0),N)*(N+1)/0.0  * (1-rainStrength)*moonVisibility ;	//coloring clouds
/*----------*/

	coord = fract(coord1/2.0);
	noise = texture2D(noisetex,coord).x;
	

	mult = 1.0;
	r = 2.5;
	tmult = 1.0;
	for (int i = 0; i < 4; i++) {
	coord *= scale;
	mult /= r;
	noise += texture2D(noisetex,coord).x*mult;
	tmult += mult;
	}
	noise /= tmult;
	float cl = max(noise-0.55+rainStrength*0.3-sin(frameTimeCounter/3000.0)*0.1,0.0);
	float ef = 0.55;
 
      float cloud2 = (1.0 - (pow((1-rainStrength*0.9)*ef,cl)))*sqrt(max(cosT,0.0));
	    
vec3 c = mix(color,cloud_color,cloud2);

//c = mix(c,cloud_color,cloud);  //mix up sky color and clouds

return c;
}

vec4 raytraceGround(vec3 fragpos, vec3 normal, vec3 fogclr) {
    vec4 color = vec4(0.0);
    vec3 start = fragpos;
    vec3 rvector = normalize(reflect(normalize(fragpos), normalize(normal)));
    vec3 vector = Gstp * rvector;
    vec3 oldpos = fragpos;
    fragpos += vector;
	vec3 tvector = vector;
    int sr = 0;
    for(int i=0;i<30;i++){
        vec3 pos = nvec3(gbufferProjection * nvec4(fragpos)) * 0.5 + 0.5;
		if(pos.x < 0 || pos.x > 1 || pos.y < 0 || pos.y > 1 || pos.z < 0 || pos.z > 1.0) break;
        vec3 spos = vec3(pos.st, texture2D(depthtex1, pos.st).r);
        spos = nvec3(gbufferProjectionInverse * nvec4(spos * 2.0 - 1.0));
        float err = distance(fragpos.xyz,spos.xyz);
        if(err < length(vector)){

                sr++;
                if(sr >= maxf){
                    float border = clamp(1.0 - pow(cdist(pos.st), 20.0), 0.0, 1.0);
                    color = texture2D(composite, pos.st);
					float land = texture2D(gaux1, pos.st).g;
					land = float(matflag < 0.03);
					spos.z = mix(fragpos.z,2000.0*(0.25+sunVisibility*0.75),land);
					color.rgb = calcFog(spos,pow(color.rgb,vec3(2.2))*MAX_COLOR_RANGE,fogclr);
					color.a = 1.0;
                    color.a *= border;
                    break;
                }
				tvector -=vector;
                vector *=Gref;
				    
}
        vector *= Ginc;
        oldpos = fragpos;
        tvector += vector;
		fragpos = start + tvector;
    }
    return color;
}
vec3 underwaterFog (float depth,vec3 color) {
	const float density = 5.0;
	float fog = exp(-depth/density);
	vec3 Ucolor= normalize(pow(vec3(0.0,0.3,0.6),vec3(2.2)))*(sqrt(3.0));
	
	vec3 c = mix(color*Ucolor,color,fog);
	vec3 fc = Ucolor*length(ambient_color)*0.02;
	return mix(fc,c,fog);
}
float waterH(vec3 posxz) {

      float wave = 0.0;

float factor = 2.0;
float amplitude = 0.2;
float speed = 4.0;
float size = 0.2;

float px = posxz.x/25.0 + 400.0;
float py = posxz.z/50.0  + 250.0;

float fpx = abs(fract(px*20.0)-0.5)*2.0;
float fpy = abs(fract(py*20.0)-0.5)*2.0;

float d = length(vec2(fpx,fpy));

for (int i = 1; i < 6; i++) {
wave -= d*factor*cos( (1/factor)*px*py*size + 1.0*frameTimeCounter*speed);
factor /= 2;
}

factor = 4.0;
px = -posxz.x/24.0 + 250.0;
py = -posxz.z/180.0 - 250.0;

fpx = abs(fract(px*20.0)-0.5)*2.0;
fpy = abs(fract(py*20.0)-0.5)*2.0;

d = length(vec2(fpx,fpy));
float wave2 = 0.0;
for (int i = 1; i < 8; i++) {
wave2 -= d*factor*cos( (1/factor)*px*py*size + 1.3*frameTimeCounter*speed);
factor /= 3;
}

return amplitude*wave2+amplitude*wave;
}
//////////////////////////////VOID MAIN//////////////////////////////
//////////////////////////////VOID MAIN//////////////////////////////
//////////////////////////////VOID MAIN//////////////////////////////
//////////////////////////////VOID MAIN//////////////////////////////
//////////////////////////////VOID MAIN//////////////////////////////

void main() {
	color.rgb = pow(color.rgb,vec3(2.2))*MAX_COLOR_RANGE;
	int land = int(matflag < 0.03);
	int iswater = int(matflag > 0.04 && matflag < 0.07);
	int hand  = int(matflag > 0.75 && matflag < 0.85);
	
	fragpos = nvec3(gbufferProjectionInverse * nvec4(fragpos * 2.0 - 1.0));
	vec3 uPos  = nvec3(gbufferProjectionInverse * nvec4(vec3(texcoord.xy,texture2D(depthtex1,texcoord.xy).x) * 2.0 - 1.0));		//underwater position
	color.rgb = drawSun(fragpos,color.rgb,land);
	vec3 fogclr = getSkyColor(fragpos.xyz);
	uPos.z = mix(uPos.z,2000.0*(0.25+sunVisibility*0.75),land);
		float normalDotEye = dot(normal, normalize(fragpos));
		float fresnel = pow(1.0 + normalDotEye, 5.0);
		fresnel = mix(1.,fresnel,0.98);
		
		
	if (iswater > 0.9 && isEyeInWater == 0) {
	
		vec3 lc = mix(vec3(0.0),sunlight,sunVisibility);
		vec4 reflection = vec4(0.0);
		vec3 npos = normalize(fragpos);
		vec3 reflectedVector = reflect(normalize(fragpos), normalize(normal));
		reflectedVector = fragpos + reflectedVector * (2048.0-fragpos.z);
		vec3 skyc = getSkyColor(reflectedVector);
		vec3 sky_color = calcFog(reflectedVector,drawCloud(reflectedVector,vec3(0.0)),skyc)*clamp(sky_lightmap*2.0-2/32.0,0.0,1.0);
			
		reflection = raytrace(fragpos, normal,skyc);
		reflection.rgb = mix(sky_color, reflection.rgb, reflection.a)+(color.a)*lc*(1.0-rainStrength)*64.0;			//fake sky reflection, avoid empty spaces
		reflection.a = min(reflection.a,1.0);
		reflection.rgb = reflection.rgb*REFLECTION_STRENGTH;
		color.rgb = fresnel*reflection.rgb + (1-fresnel)*color.rgb;
    }

	
	if (hand < 0.1) color.rgb = calcFog(uPos.xyz,color.rgb,fogclr);
	if (isEyeInWater == 1) color.rgb = underwaterFog(length(fragpos),color.rgb);
	
	vec4 tpos = vec4(sunPosition,1.0)*gbufferProjection;
	tpos = vec4(tpos.xyz/tpos.w,1.0);
	vec2 pos1 = tpos.xy/tpos.z;
	vec2 lightPos = pos1*0.5+0.5;
	float gr = 0.0;
	
#ifdef GODRAYS
	float truepos = sunPosition.z/abs(sunPosition.z);		//1 -> sun / -1 -> moon
	vec3 rainc = mix(vec3(1.),fogclr*1.5,rainStrength);
	vec3 lightColor = mix(sunlight*sunVisibility*rainc,3*moonlight*moonVisibility*rainc,(truepos+1.0)/2.);
	
	const int nSteps = NUM_SAMPLES;
	const float blurScale = 0.002/nSteps*9.0;
	const int center = (nSteps-1)/2;
	vec3 blur = vec3(0.0);
	float tw = 0.0;
	const float sigma = 0.5;
	

	vec2 deltaTextCoord = normalize(texcoord.st - lightPos.xy)*blurScale;
	vec2 textCoord = texcoord.st - deltaTextCoord*center;
		
	float distx = texcoord.x*aspectRatio-lightPos.x*aspectRatio;
	float disty = texcoord.y-lightPos.y;
	float illuminationDecay = pow(max(1.0-sqrt(distx*distx+disty*disty),0.0),5.0);
	/*-----------*/
		for(int i=0; i < nSteps ; i++) {
				textCoord += deltaTextCoord;
				
				float dist = (i-float(center))/center;
				float weight = exp(-(dist*dist)/(2.0*sigma));
				
				float sample = texture2D(gdepth, textCoord).r*weight;
				tw += weight;
				gr += sample;		
		
	}
	vec3 grC = mix(lightColor,fogclr,rainStrength)*exposure*(gr/tw)*(1.0 - rainStrength*0.8)*illuminationDecay * (1-isEyeInWater);
	color.xyz = (1-(1-color.xyz/48.0)*(1-grC.xyz/48.0))*48.0;
	/*-----------*/
	
#endif
	
	float visiblesun = 0.0;
	float temp;
	float nb = 0;
	
//calculate sun occlusion (only on one pixel) 
if (texcoord.x < 3.0*pw && texcoord.x < 3.0*ph) {
	for (int i = 0; i < 10;i++) {
		for (int j = 0; j < 10 ;j++) {
		temp = texture2D(gaux1,lightPos + vec2(pw*(i-5.0)*10.0,ph*(j-5.0)*10.0)).g;
		visiblesun +=  1.0-float(temp > 0.04) ;
		nb += 1;
		}
	}
	visiblesun /= nb;

}

	color.rgb = clamp(pow(color.rgb/MAX_COLOR_RANGE,vec3(1.0/2.2)),0.0,1.0);

/* DRAWBUFFERS:5 */
	gl_FragData[0] = vec4(color.rgb,visiblesun);
}