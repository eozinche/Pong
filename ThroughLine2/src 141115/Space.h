//
//  Space.h
//  Throughline
//
//  Created by MOQN on 11/6/14.
//
//
#pragma once

#include "_main.h"
#include "Object.h"



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
    
    //gl::Texture           imgOverlay;
    //gl::Texture           imgBackground;
    
    vector<Object*>         objects;
    
    Space(){
        spaceSize           = Vec3f( SPACE_SIZE );
        gravity             = GRAVITY_INIT;
        wind                = Vec3f::zero();
        velReduction        = VELOCITY_REDUCTION;
        camPersp            = 60;
    }
    
    
    void setup();
    void update();
    void display();
    
    void setCamera();
    void setCamera( float perspective, float zDistance );
    
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

};





