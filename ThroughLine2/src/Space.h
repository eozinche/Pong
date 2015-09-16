//
//  Space.h
//  Throughline
//
//  Created by EOZIN CHE on 11/6/14.
//
//
#pragma once

#include "_main.h"
#include "Object.h"
#include "Renderer.h"                                               //EOZIN
#include "cinder/Perlin.h"                                          //EOZIN
#include "Resources.h"                                              //EOZIN

#include "Particle.h"                                               //EOZIN
#include "Emitter.h"                                                //EOZIN

/////////////////////////////////////////////////////////////////////////// Space /////

class Space {
public:
    
    gl::GlslProg            mShader;
    
    Vec2f                   winSize;
    int                     width, height;
    
    CameraPersp             mCam;
    float                   camPersp;
    
    params::InterfaceGlRef  GUI;
    
    float                   time;
    
    Vec3f                   spaceSize;
    float                   gravity;
    Vec3f                   wind;
    float                   velReduction;
    
    
    
    /////////////////////////////////////////////////EOZIN(start)
    
    Renderer* prepareRenderer() { return new RendererGl( RendererGl::AA_MSAA_2 ); }
    
    int			counter = 0;
    float		floorLevel = 400.0f;
    gl::TextureRef particleImg, emitterImg;
    
    
    bool		ALLOWFLOOR = true;
    bool		ALLOWGRAVITY = true;
    bool		ALLOWPERLIN = true;
    bool		ALLOWTRAILS = false;
    
    const int	CINDER_FACTOR = 10; // how many times more particles than the Java version
    
    
    Emitter		   mEmitter;
    bool           mouseIsDown;
    Vec2i          mSpherePos;                                          ////////////////
    
    /////////////////////////////////////////////////EOZIN(end)
    
    
    //gl::Texture           imgOverlay;
    //gl::Texture           imgBackground;
    
    vector<Object*>         objects;
    
    Space(){
        spaceSize           = Vec3f( SPACE_SIZE );
        gravity             = GRAVITY_INIT;
        wind                = Vec3f::zero();
        velReduction        = VELOCITY_REDUCTION;
        camPersp            = 1;
    }
    
    
    void setup();
    void update();
    void display();
    
    void setCamera();
    void setCamera( float perspective, float zDistance ); //overrided
    
    void setGUIs();
    
    void checkCollision();
    void saveImage();
    
    
    void createCube( float x, float y, float z );
    void createCube( Vec3f pos );
    void createCube( Vec3f pos, Vec3f vel,  float mass );
    
    void createSphere( float x, float y, float z );
    void createSphere( Vec3f pos );
    void createSphere( Vec3f pos, Vec3f vel,  float mass );
    
    
    void displaySetup();
    void displayUpdate();
    void drawInNormal();
    void drawIntoShader( Vec3f tLightPos );
    
    
    void keyDown( KeyEvent event );                     //EOZIN
    //void drawSceneOne();


};





