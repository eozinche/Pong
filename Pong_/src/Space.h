//
//  Space.h
//  _PONG
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
    float                   camZdist;
    
    Vec3f                   gLightPos;
    
    params::InterfaceGlRef  GUI;
    
    float                   frame;
    
    Vec3f                   spaceSize;
    float                   gravity;
    Vec3f                   wind;
    float                   velReduction;
    
    vector<Object*>         objects;
    vector<ObjMesh>         objMeshes;
    
    float                   sinArray[360];
    float                   cosArray[360];
    
    Space(){
        spaceSize           = Vec3f( SPACE_SIZE );
        gravity             = GRAVITY_INIT;
        wind                = Vec3f::zero();
        velReduction        = VELOCITY_REDUCTION;
        camPersp            = 1;
        camZdist            = 1140;
    }
    
    
    void setup();
    void update();
    void display();
    
    void drawInPixelSpace();
    void drawIn3DSpace();
    void drawIn3DSpaceShader();
    
    void setCamera();
    void setCamera( float perspective, float zDistance ); // overrided
    
    bool sequence( int frameNum );
    bool sequence( int from, int until );
    
    void setGUIs();
    void saveImage();
    
    void updateEveryObject();
    void checkCollision();
    
    
    void createCube( float x, float y, float z );
    void createCube( Vec3f pos );
    void createCube( Vec3f pos, string id );
    void createCube( Vec3f pos, Vec3f vel,  float size );
    void createCube( Vec3f pos, Vec3f vel,  float size, string id );
    
    void createSphere( float x, float y, float z );
    void createSphere( Vec3f pos );
    void createSphere( Vec3f pos, string id );
    void createSphere( Vec3f pos, Vec3f vel,  float size );
    void createSphere( Vec3f pos, Vec3f vel,  float size, string id );
    
    
    void createObjMeshCube( Vec3f _pos, int numU, int numV, string _id );
    void createObjMeshSphere( Vec3f _pos, int numU, int numV, string _id );
    void displayObjMesh( string _id );
    void displayObjMesh( float z, string _id );
    
    void objMesh_changePosition( Vec3f _pos, string _id );
    void objMesh_rotate( Vec3f _axis, float _angle, string _id );
    void objMesh_changeUVSize( int _u, int _v, string _id );
    void objMesh_changeUnitSize( Vec3f _unitSize, string _id );
    void objMesh_changeGapSize( Vec2f _gap, string _id );
    
    

    void toTest();
    void consoleOut();
    
    // Scene Pong2D Properties
    void scenePong2D_createBalls();
    void scenePong2D_createPaddles();
    void scenePong2D_createMorePaddles();
    void scenePong2D_runBalls();
    void scenePong2D_runPaddles();
    void scenePong2D_arrangePaddles();
    void scenePong2D_changeCameraToPerspective();
    
    // Scene Pong3D Properties
    void scenePong3D_createBall();
    void scenePong3D_runBall();
    void scenePong3D_changeCamera();
    void scenePong3D_changeForces();
    
    // Scene CubeMesh Properties
    void sceneCubemesh_createCubemesh();
    void sceneCubemesh_runCubemesh();
    void sceneCubemesh_changeCamera();
    
    // Scene Attraction Properties
    void sceneAttraction_createCubes();
    void sceneAttraction_changeCamera();
    void sceneAttraction_changeForces();
    void sceneAttraction_run();
};




















