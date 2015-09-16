#version 120

uniform sampler2D noiseTex;
uniform float scale;

varying vec3 vPosition;
varying vec3 vNormal;

void main(){

    gl_TexCoord[0] = gl_MultiTexCoord0;
    
    vec4 position = gl_Vertex;
    vec3 normal = gl_Normal;
    
    float noise = texture2D( noiseTex, gl_TexCoord[0].xy).r;
    
    position.xyz += normal * noise * scale;
    normal += normal * noise * scale;

    
    vPosition = vec3(gl_ModelViewMatrix * position);
    vNormal = gl_NormalMatrix * normalize(normal);
    
    gl_Position = gl_ModelViewProjectionMatrix * position;
    
}