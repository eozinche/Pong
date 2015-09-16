#version 120
uniform float time;
uniform vec2 size;

//#define SHOW_TILING
#define TILES 2.0

//----------------------------------------------------------------------------------------
float Hash(in vec2 p, in float scale)
{
    // This is tiling part, adjusts with the scale...
    p = mod(p, scale);
    return fract(sin(dot(p, vec2(35.6898, 24.3563))) * 353753.373453);
}

//----------------------------------------------------------------------------------------
float Noise(in vec2 x, in float scale )
{
    x *= scale;
    
    x += vec2(sin(time * .3+scale*.2), cos(time * .33+scale*.23))*(scale*.11);
    
    vec2 p = floor(x);
    vec2 f = fract(x);
    f = f*f*(3.0-2.0*f);
    //f = (1.0-cos(f*3.1415927)) * .5;
    float res = mix(mix(Hash(p,                  scale),
                        Hash(p + vec2(1.0, 0.0), scale), f.x),
                    mix(Hash(p + vec2(0.0, 1.0), scale),
                        Hash(p + vec2(1.0, 1.0), scale), f.x), f.y);
    return res;
}

//----------------------------------------------------------------------------------------
float fBm(in vec2 p)
{
    float f = 0.4;
    // Change starting scale to any integer value...
    float scale = 14.0;                                             //14.0
    float amp = 0.55;                                               //0.55
    for (int i = 0; i < 2; i++)                                     //i < 8
    {
        f += Noise(p, scale) * amp;
        amp *= -.65;                                                //amp *= -.65;
        // Scale must be multiplied by an integer value...
        scale *= 2.0;                                              //scale *= 2.0;
    }
    return f;
}



void main(void)
{
    vec2 uv = gl_TexCoord[0].st;
    
//#ifdef SHOW_TILING
    uv *= TILES;
//#endif
    
    // Do the noise cloud (fractal Brownian motion)
    float bri = fBm(uv*time*0.01);
    //float bri = fBm(uv + vec2(time*.1, time*.3));
    
    bri = min(bri * bri, 1.0); // ...cranked up the contrast for no reason.
    vec3 col = vec3(bri);
//    
//#ifdef SHOW_TILING
//    vec2 pixel = (TILES / iResolution.xy);
//    // Flash borders...
//    if (uv.x > pixel.x && uv.y > pixel.y                                        // Not first pixel
//        && (fract(uv.x) < pixel.x || fract(uv.y) < pixel.y) // Is it on a border?
//        && mod(iGlobalTime-2.0, 4.0) < 2.0)                 // Flash every 2 seconds
//    {
//        col = vec3(1.0, 1.0, 0.0);
//    }
//#endif
    gl_FragColor = vec4(col,1.0);
}


