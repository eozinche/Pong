
#version 330

in vec4 vPosition;
in vec3 vNormal;

layout ( location = 0 ) out vec4 FragColor;

const int numLights = 2;
uniform vec3 lightPositions[2];

void main(){
    vec3    ambient = vec3( .1,.1,.1 );
    
    for (int i=0; i<2; i++) {
        vec3    normal = normalize( vNormal );
        vec3	lightVector = normalize( lightPositions[i] - vPosition.xyz );
        vec3	diffuse = vec3( max( dot( normal, lightVector ), 0.0 ) );
        
        FragColor += vec4( diffuse , 1. );
    }
    FragColor += vec4( ambient , 1. );
    
}