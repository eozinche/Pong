#version 120

varying vec3 vPosition;
varying vec3 vNormal;


void main(){

    vec3 normal = normalize( vNormal );
    vec3 position = vPosition;
    
    vec3 lightVector = normalize( vec3(0,25,0) - position );
    //vec3(0,25,0)
    
    gl_FragColor.xyz =  + mix(
                              vec3(.3),
                              vec3( max( dot(normal, lightVector), 0.0) ) ,
                              max( dot( normal, normalize(-position) ), 0.0 )
    );
   // gl_FragColor.xyz += vec3(.7, .8, .9);
    
    //gray diffuse // normalize(-position) = direction towards eye point
    //vec3(.7, .8, 1), vec3(.3)
    
   // gl_FragColor.xyz = normal;
    gl_FragColor.a = 1.;
    
}