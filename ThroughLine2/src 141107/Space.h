//
//  Space.h
//  Throughline
//
//  Created by MOQN on 11/6/14.
//
//

#pragma once

#define SAVE_IMAGE_ON   8

#define WINDOW_WIDTH    (3840*3)  /8
#define WINDOW_HEIGHT   (1080+72) /8


#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/GlslProg.h"
#include "cinder/Camera.h"
#include "cinder/Rand.h"

#include "cinder/ImageIo.h"
#include "cinder/Utilities.h"

#include <sstream>

using namespace ci;
using namespace ci::app;
using namespace std;


#define STRINGIFY(x) #x

// See:
// http://stackoverflow.com/questions/15588860/what-exactly-are-eye-space-coordinates
// https://www.opengl.org/sdk/docs/tutorials/ClockworkCoders/lighting.php
// http://en.wikibooks.org/wiki/GLSL_Programming/GLUT/Multiple_Lights



/////////////////////////////////////////////////////////////////////////// Object /////
class Object {
public:
    Vec3f   pos;
    Vec3f   vel;
    Vec3f   acc;
    
    float   mass;
    Vec3f   size;
    Vec3f   axis;
    float   angle;
    
    float   velReduction;
    
    Object( Vec3f _pos, Vec3f _vel, float _mass ) {
        pos = _pos;
        vel = _vel;
        mass = _mass;
        acc = Vec3f( 0,0,0 );
        size = Vec3f( 1,1,1 );
        axis = Vec3f( 1.0,0.0,0.5 );
        angle = 0;
        velReduction = 1.001; //0.99;
    }
    ~Object(){}
    
    void update();
    void updateRotation();
    void applyForce( Vec3f force );
    bool checkEdges( Vec3f edge );
    
    void drawSphere();
    void drawCube();
    void drawStrokedCube();
};


/////////////////////////////////////////////////////////////////////////// Space /////

class Space {
public:
    CameraPersp         mCam;
    gl::GlslProg		mShader;
    
    Vec2f               winSize;
    int                 width, height;
    
    Vec3f               spaceSize;
    float               gravity;
    Vec3f               wind;
    
    vector<Object>      spheres;
    vector<Object>      frameCubes;
    vector<Object>      cubes;
    
    Space(){
        spaceSize       = Vec3f( 80, 8, 6 );
        gravity         = 0.05;
        wind            = Vec3f(0, 0, 0);
    }
    
    void setup();
    void update();
    void display();
    
    void updateObjects( Object* obj, float _gravity, Vec3f _wind, bool _edge  );
    void checkCollision();
    
    void createSphere( Vec3f pos, Vec3f vel,  float mass );
    void createCube( Vec3f pos, Vec3f vel,  float mass );
    void createFrameCube( Vec3f pos, Vec3f vel,  float mass );
    
    
    void saveImage();

};







/////////////////////////////////////////////////////////////////////////// Shaders /////

static const string kVertGlsl =
STRINGIFY(
          varying vec3 vVertex;
          varying vec3 vNormal;
          
          void main(void)
          {
              // In camera-space:
              
              // Get vertex position:
              vVertex = vec3( gl_ModelViewMatrix * gl_Vertex );
              // Get vertex normal:
              vNormal = normalize( gl_NormalMatrix * gl_Normal );
              // Set vertex position:
              gl_Position = ftransform();
          }
          );

static const string kFragGlsl =
STRINGIFY(
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
          }
          );




