//
//  _SceneAttraction.cpp
//  Pong
//
//  Created by MOQN on 11/21/14.
//
//

#include "_main.h"
#include "Space.h"


void Space::sceneAttraction_createCubes() {
    for ( int i=0; i<60; i++ ){
        createCube( Vec3f(-spaceSize.x,
                          randFloat(-spaceSize.y, spaceSize.y),
                          randFloat( 0, spaceSize.z))
                   , "Cube|Attracted" );
        objects[0]->colliding = true;
        //objects[0]->wireframe = true;
        objects[0]->changeSize( randFloat( 0.1, 0.6 ) );
        objects[0]->vel = Vec3f(randFloat( 0.0, 3.2 ),
                                randFloat( -1.0,1.0 ),
                                randFloat( 0.0, 1.0 ));
        createCube( Vec3f(spaceSize.x,
                          randFloat(-spaceSize.y, spaceSize.y),
                          randFloat( 0, spaceSize.z))
                   , "Cube|Attracted" );
        objects[0]->colliding = true;
        //objects[0]->wireframe = true;
        objects[0]->changeSize( randFloat( 0.1, 0.6 ) );
        objects[0]->vel = Vec3f(randFloat( 0.0, -3.2 ),
                                randFloat( -1.0,1.0 ),
                                randFloat( 0.0, 1.0 ));
    }
}

void Space::sceneAttraction_changeCamera() {
    camPersp = 50;
    camZdist = 20;
}

void Space::sceneAttraction_changeForces() {
    velReduction = 0.975;
    gravity = 0.0;
}

void Space::sceneAttraction_run() {
    for ( auto& obj : objects ) {
        //if (obj->size.x < 1.2) obj->size *= 1.01;
        obj->applyAttraction( Vec3f::zero(), 2 );
        mShader.uniform( "mLightPosition", Quatf( Vec3f( obj->axis ), -obj->angle) * gLightPos );
        obj->display();
    }
}