#version 120

varying vec3 vVertex;
varying vec3 vNormal;

void main(void)
{
    
    // Get vertex position:
    vVertex = vec3( gl_ModelViewMatrix * gl_Vertex );
    // Get vertex normal:
    vNormal = normalize( gl_NormalMatrix * gl_Normal );
    // Set vertex position:
    gl_Position = ftransform();
}