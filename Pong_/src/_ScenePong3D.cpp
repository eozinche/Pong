//
//  _ScenePong3D.cpp
//  _PONG
//
//
//

#include "_main.h"
#include "Space.h"


void Space::scenePong3D_createBall() {
    createSphere( Vec3f(0,9,0) , "ball" );
    objects[0]->colliding = true;
    objects[0]->bouncing = true;
    objects[0]->changeSize( 3 );
    objects[0]->vel = Vec3f( 0.4, 0.1, 0.3 );
}

void Space::scenePong3D_changeCamera() {
    camPersp = 50;
    camZdist = 20;
}

void Space::scenePong3D_changeForces() {
    velReduction = 1.001;
}

void Space::scenePong3D_runBall() {

    for ( auto& obj : objects ){
        if ( obj->id == "ball" && obj->stateBounce ){
            Vec3f pos = obj->pos;
            Vec3f vel = obj->vel;
            float avgVel = abs( ( vel.x+vel.y+vel.z )/3 );
            for (int i=0; i<randFloat( avgVel*50, avgVel*100 ); i++){
                createCube( pos, "cube" );
                objects[0]->colliding = true;
                objects[0]->bouncing = true;
                float r = 0.3;
                Vec3f f = Vec3f(randFloat( -r,r ),
                                randFloat( -r,r ),
                                randFloat( -r,r ));
                objects[0]->changeSize( randFloat( 0.4, 0.7 ) );
                objects[0]->vel = -vel*0.2 + f;
                objects[0]->vel.y += randFloat( r*2 );
            }
        }
    }
    
    for ( auto& obj : objects ){
        if ( obj->id == "ball" ){
            mShader.uniform( "mLightPosition", Quatf( Vec3f( obj->axis ), -obj->angle) * gLightPos );
        } else if ( obj->id == "cube" ){
            obj->size *= 0.97;
        }
        obj->display();
    }
}