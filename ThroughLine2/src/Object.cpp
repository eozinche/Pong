//
//  Object.cpp
//  Throughline
//
//  Created by MOQN on 11/14/14.
//
//

#include "_main.h"
#include "Object.h"

/////////////////////////////////////////////////////////////////////////// Object /////

void Object::display() {
    gl::pushMatrices();
    gl::translate( pos );
    gl::rotate( Quatf( axis, angle) );
    gl::drawSphere( Vec3f::zero(), mass );
    gl::popMatrices();
}


void Object::update( float velReduction ) {
    if ( !immobility ){
        vel += acc;
        pos += vel;
        acc *= 0;
        vel *= velReduction;
        
        // Rotation
        float avrVel = ( abs(vel.x)+abs(vel.y)+abs(vel.x) ) /3;
        avrVel = floor(avrVel*100) / 100;
        
        /*
         float maxVal = 0;
         for (int i=0; i<sizeof(vel)/sizeof(vel[0]); i++) {
         if ( maxVal >= vel[i] ) {
         maxVal = vel[i];
         if (axis[i] < 1.0)  axis[i] += 0.01;
         else axis[i] = 1.0;
         } else {
         if (axis[i] > 0.0) axis[i] -= 0.01;
         else axis[i] = 0.0;
         }
         }
         */
        
        angle += avrVel;
        if (angle > 360) angle -= 360;
    }
}


bool Object::isDone( Vec3f spaceSize ) {
    if (pos.y < spaceSize.y * -2 || pos.y > spaceSize.y * 3) return true;
    else if (pos.x < spaceSize.x * -2 || pos.x > spaceSize.x * 2) return true;
    else if (pos.z < spaceSize.z * -4 || pos.z > spaceSize.z * 4) return true;
    
    return false;
}


bool Object::checkHitBoundary( Vec3f spaceSize ) {
    // x
    if (pos.x > spaceSize.x) {
        pos.x = spaceSize.x;
        vel.x *= -1;
        return true;
    } else if (pos.x < -spaceSize.x) {
        pos.x = -spaceSize.x;
        vel.x *= -1;
        return true;
    }
    // y
    if (pos.y > spaceSize.y*1.5) {
        pos.y = spaceSize.y*1.5;
        vel.y *= -1;
        return true;
    } else if (pos.y < -spaceSize.y+1){
        pos.y = -spaceSize.y+1;
        vel.y *= -1;
        return true;
    }
    // z
    if (pos.z > 0) {
        pos.z = 0;
        vel.z *= -1;
        return true;
    } else if (pos.z < -spaceSize.z*3){
        pos.z = -spaceSize.z*3;
        vel.z *= -1;
        return true;
    }
    return false;
}


void Object::changeProperties( Vec3f _pos, Vec3f _vel, float _mass ){
    pos = _pos;
    vel = _vel;
    mass = _mass;
}


void Object::changePosition( float x, float y, float z ) {
    pos = Vec3f( x,y,z );
}


void Object::changeVelocity( float x, float y, float z ) {
    vel = Vec3f( x,y,z );
}


void Object::changeSize( float x, float y, float z ) {
    size = Vec3f( x,y,z );
}


void Object::applyForce(Vec3f force) {
    Vec3f f = force / mass;
    acc += f;
}


void Object::applyForce( float x, float y, float z ) {
    Vec3f f = Vec3f( x,y,z ) / mass;
    acc += f;
}


