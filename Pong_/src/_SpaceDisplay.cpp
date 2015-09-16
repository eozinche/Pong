//
//  SpaceDisplay.cpp
//  _PONG
//
//
//


#include "_main.h"
#include "Space.h"


///////////////////////////////////////////////////////////////// Space Display Functions /////

void Space::drawInPixelSpace() {
    
    
    
}

void Space::drawIn3DSpace() {
    
    // Scene Pong2D
    /*
    if ( sequence(1) )          scenePong2D_createBalls();
    if ( sequence(500) )        scenePong2D_createPaddles(); // 500
    if ( sequence(600) )        scenePong2D_createMorePaddles(); //950
    if ( sequence(2, 3000) )    scenePong2D_runBalls();
    if ( sequence(500, 100) )    scenePong2D_runPaddles();

    if ( sequence(100, 3000) )    scenePong2D_arrangePaddles();
    if ( sequence(280, 3000) )    scenePong2D_changeCameraToPerspective();
    */
    
    
    
    
}

void Space::drawIn3DSpaceShader() {
    // Scene Pong3D
    /*
    if ( sequence(1) ){
        scenePong3D_changeCamera();
        scenePong3D_changeForces();
        scenePong3D_createBall();
    }
    if ( sequence(1,3000) ) scenePong3D_runBall();
     */
    
    
    // CubeMesh
    /*
    if ( sequence(1) ){
        scenePong3D_changeCamera();
        
        sceneCubemesh_createCubemesh();
    }
    if ( sequence(1,360) ) {
        sceneCubemesh_runCubemesh();
        sceneCubemesh_changeCamera();
    }
    if ( sequence(360) ) {
        objMeshes.clear();
        gravity = 0;
        for ( auto& obj : objects ){
            obj->colliding = true;
            //obj->bouncing = true;
            obj->immobility = false;
            
            obj->vel = Vec3f (randFloat(-0.3,0.3),
                              randFloat(-0.3,0.3),
                              randFloat(-0.3,0.3));
        }
    }
    if ( sequence(360, 1800) ) {
        for ( auto& obj : objects ){
            obj->display();
            obj->applyAttraction( Vec3f::zero() );
        }
    }
    */
    
    // Scene Attraction
    
    if ( sequence(1) ){
        sceneAttraction_createCubes();
        sceneAttraction_changeCamera();
        sceneAttraction_changeForces();
    }
    for ( int i=0; i<8; i++){
        if ( sequence(30*i) ) sceneAttraction_createCubes();
    }
    
    if ( sequence(1,6000) ) sceneAttraction_run();
    
    
}














