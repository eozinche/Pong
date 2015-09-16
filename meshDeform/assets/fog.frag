#version 120

uniform float t;
uniform vec2 wh;

#define pi 3.14159265
#define R(p, a) p=cos(a)*p+sin(a)*vec2(p.y, -p.x)
#define hsv(h,s,v) mix(vec3(1.), clamp((abs(fract(h+vec3(3., 2., 1.)/3.)*6.-3.)-1.), 0., 1.), s)*v

float pn(vec3 p) {
    vec3 i = floor(p);
    vec4 a = dot(i, vec3(1., 57., 21.)) + vec4(0., 57., 21., 78.);
    vec3 f = cos((p-i)*pi)*(-.5) + .5;
    a = mix(sin(cos(a)*a), sin(cos(1.+a)*(1.+a)), f.x);
    a.xy = mix(a.xz, a.yw, f.y);
    return mix(a.x, a.y, f.z);
}

float fpn(vec3 p) {
    return pn(p*.06125)*.5 + pn(p*.125)*.25 + pn(p*.25)*.125;
}

vec3 n1 = vec3(1.000,0.000,0.000);
vec3 n2 = vec3(0.000,1.000,0.000);
vec3 n3 = vec3(0.000,0.000,1.000);
vec3 n4 = vec3(0.577,0.577,0.577);
vec3 n5 = vec3(-0.577,0.577,0.577);
vec3 n6 = vec3(0.577,-0.577,0.577);
vec3 n7 = vec3(0.577,0.577,-0.577);
vec3 n8 = vec3(0.000,0.357,0.934);
vec3 n9 = vec3(0.000,-0.357,0.934);
vec3 n10 = vec3(0.934,0.000,0.357);
vec3 n11 = vec3(-0.934,0.000,0.357);
vec3 n12 = vec3(0.357,0.934,0.000);
vec3 n13 = vec3(-0.357,0.934,0.000);
vec3 n14 = vec3(0.000,0.851,0.526);
vec3 n15 = vec3(0.000,-0.851,0.526);
vec3 n16 = vec3(0.526,0.000,0.851);
vec3 n17 = vec3(-0.526,0.000,0.851);
vec3 n18 = vec3(0.851,0.526,0.000);
vec3 n19 = vec3(-0.851,0.526,0.000);

float spikeball(vec3 p) {
    vec3 q=p;
    p = normalize(p);
    vec4 b = max(max(max(
                         abs(vec4(dot(p,n16), dot(p,n17),dot(p, n18), dot(p,n19))),
                         abs(vec4(dot(p,n12), dot(p,n13), dot(p, n14), dot(p,n15)))),
                     abs(vec4(dot(p,n8), dot(p,n9), dot(p, n10), dot(p,n11)))),
                 abs(vec4(dot(p,n4), dot(p,n5), dot(p, n6), dot(p,n7))));
    b.xy = max(b.xy, b.zw);
    b.x = pow(max(b.x, b.y), 140.);
    return length(q)-2.5*pow(1.5,b.x*(1.-mix(.3, 1., sin(t*2.)*.5+.5)*b.x));
}

float f(vec3 p) {
    p.z += 6.;
    R(p.xy, t);
    R(p.xz, t);
    return spikeball(p) +  fpn(p*50.+t*15.) * 0.45;
}

vec3 g(vec3 p) {
    vec2 e = vec2(.0001, .0);
    return normalize(vec3(f(p+e.xyy) - f(p-e.xyy),f(p+e.yxy) - f(p-e.yxy),f(p+e.yyx) - f(p-e.yyx)));
}

float as(vec3 p, vec3 n, float d, float i) {
    float s = sign(d);
    float o = s*.5+.5;
    for (; i > 0.; --i) {
        o -= (i*d - f(p+n*i*d*s)) / exp2(i);
    }
    return o;
}

uniform float time;

void main(void)
{
    // p: position on the ray
    // d: direction of the ray
    vec3 p = vec3(0.,0.,2.);
    vec3 d = vec3((gl_FragCoord.xy/(0.5*wh)-1.)*vec2(wh.x/wh.y,1.0), 0.) - p;
    
    //vec3 d = vec3((gl_TexCoord[0].xy/(0.5*wh)-1.)*vec2(wh.x/wh.y,1.0), 0.) - p;
    
    d = normalize(d);
    
    // ld, td: local, total density
    // w: weighting factor
    float ld, td= 0.;
    float w;
    
    // total color
    vec3 tc = vec3(0.);
    
    // i: 0 <= i <= 1.
    // r: length of the ray
    // l: distance function
    float i, r, l, b=0.;
    
    // rm loop
    for (i=r=l=0.; i<1. && l>=0.001*r && r < 50. && td < .95; i+=1./64.) {
        // evaluate distance function
        l = f(p) * 0.5;
        
        // check whether we are close enough
        if (l < .05) {
            // compute local density and weighting factor
            ld = 0.05 - l;
            w = (1. - td) * ld;
            
            // accumulate color and density
            tc += w; //* hsv(w, 1., 1.);
            td += w;
        }
        td += 1./200.;
        
        // enforce minimum stepsize
        l = max(l, 0.03);
        
        // step forward
        p += l*d;
        r += l;
    }     
    
    gl_FragColor = vec4(tc, 1.0);
}