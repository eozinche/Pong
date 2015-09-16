
#version 330

in vec4 vPosition;
in vec3 vNormal;

layout ( location = 0 ) out vec4 FragColor;

const int numLights = 2;
uniform vec3 lightPositions[numLights];

void main(){
    
    vec3    ambient = vec3( .1,.1,.1 );
    
    vec3    normal = vNormal;
    
    vec3	lightVector = normalize( lightPositions[0] - vPosition.xyz );
    vec3	lightVector1 = normalize( lightPositions[1] - vPosition.xyz );
    
    //light attenuation
    float   dist = length(lightVector);
    float   dist1 = length(lightVector1);
    
    float   attenuation = 1. / ( dist +  dist * dist );
    float   attenuation1 = 1. / ( dist1 +  dist1 * dist1 );
    
    // To camera vector
    vec3	cameraVector = normalize( -vPosition.xyz );
    
    // Surface reflection vector
    vec3	reflection = normalize( -reflect( lightVector, normal) );
    vec3	reflection1 = normalize( -reflect( lightVector1, normal) );
    
    // Diffuse factor
    vec3	diffuse = vec3( max( dot( normal, lightVector ), 0.0 ) );
    vec3	diffuse1 = vec3( max( dot( normal, lightVector1 ), 0.0 ) );
    
    // Specular factor
    const float shininess_factor = 128.;
    
    vec3    specular =	vec3(  pow( max( dot( reflection, cameraVector ), 0.0 ), shininess_factor ) );
    vec3    specular1 =	vec3(  pow( max( dot( reflection1, cameraVector ), 0.0 ), shininess_factor ) );
    
    FragColor = vec4( attenuation*( specular + diffuse ) + ambient, 1. );
    FragColor += vec4( attenuation1*( specular1 + diffuse1 ), 1. );
    
}



//
//#version 330
//
//in vec4 vPosition;
//in vec3 vNormal;
//
//layout ( location = 0 ) out vec4 FragColor;
//
//const int numLights = 2;
//uniform vec3 lightPositions[numLights];
//
//void main(){
//
//    vec3    ambient = vec3( .1,.1,.1 );
//
//    vec3    normal = vNormal;
//
//    vec3	lightVector = normalize( lightPositions[0] - vPosition.xyz );
//    vec3	lightVector1 = normalize( lightPositions[1] - vPosition.xyz );
//
//    //light attenuation
//    float   dist = length(lightVector);
//    float   dist1 = length(lightVector1);
//
//    float   attenuation = 1. / ( dist +  dist * dist );
//    float   attenuation1 = 1. / ( dist1 +  dist1 * dist1 );
//
//    // To camera vector
//    vec3	cameraVector = normalize( -vPosition.xyz );
//    // Surface reflection vector
//
//    vec3	reflection = normalize( -reflect( lightVector, normal) );
//    vec3	reflection1 = normalize( -reflect( lightVector1, normal) );
//
//    // Diffuse factor
//    vec3	diffuse = vec3( max( dot( normal, lightVector ), 0.0 ) );
//    vec3	diffuse1 = vec3( max( dot( normal, lightVector1 ), 0.0 ) );
//    // Specular factor
//
//    const float shininess_factor = 128.;
//
//    vec3    specular =	vec3(  pow( max( dot( reflection, cameraVector ), 0.0 ), shininess_factor ) );
//    vec3    specular1 =	vec3(  pow( max( dot( reflection1, cameraVector ), 0.0 ), shininess_factor ) );
//
//    FragColor += vec4( attenuation*( specular + diffuse ) + ambient, 1. );
//    FragColor += vec4( attenuation1*( specular1 + diffuse1 ) + ambient, 1. );
//
//}