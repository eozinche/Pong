#version 120

uniform vec3 mLightPosition;
uniform vec4 mAmbient;
uniform vec4 mDiffuse;
uniform vec4 mSpecular;
uniform float mShininess;

varying vec3 vVertex;
varying vec3 vNormal;

void main (void)
{
    // In camera-space:
    
    // Convert light position from world-space to camera-space:
    vec4 tLightCamSpace = gl_ModelViewMatrix * vec4( mLightPosition, 1.0 );
    // Compute light direction:
    vec3 tLightDir = normalize( tLightCamSpace.xyz - vVertex );
    // Compute camera vector:
    vec3 tCamVec = normalize( -vVertex );
    // Compute reflection direction for light vector:
    vec3 tRefDir = normalize( -reflect( tLightDir, vNormal ) );
    // Compute diffuse color:
    vec4 tDiff = clamp( mDiffuse * max( dot( vNormal, tLightDir ), 0.0 ), 0.0, 1.0 );
    // Compute specular color:
    vec4 tSpec = clamp( mSpecular * pow( max( dot( tRefDir, tCamVec ), 0.0 ), 0.3 * mShininess ), 0.0, 1.0 );
    // Compute final color:
    gl_FragColor = mAmbient + tDiff + tSpec;
    gl_FragColor.a = 1.;
}