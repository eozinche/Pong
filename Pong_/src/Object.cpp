//
//  Object.cpp
//  _PONG
//
//  Created by EOZIN CHE on 11/14/14.
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
    mass = (size.x+size.y+size.z)/3;
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


void Object::setId( string _id ){
    id = _id;
}

bool Object::getId( string _id ){
    vector<string> words;
    words = split(id, "|");
    for ( auto& word : words){
        if (word == _id) return true;
    }
    return false;
}

bool Object::isDone( Vec3f spaceSize ) {
    /*
    if (pos.y < spaceSize.y * -2 || pos.y > spaceSize.y * 3) return true;
    else if (pos.x < spaceSize.x * -2 || pos.x > spaceSize.x * 2) return true;
    else if (pos.z < spaceSize.z * -4 || pos.z > spaceSize.z * 4) return true;
    */
    if ( mass < OBJECT_SIZE_MIN ) return true;
        
    return false;
}


void Object::checkHittingBoundary( Vec3f spaceSize ) {
    stateBounce = false;
    // x
    if (pos.x > spaceSize.x) {
        pos.x = spaceSize.x -1;
        vel.x *= -1;
        stateBounce = true;
    } else if (pos.x < -spaceSize.x) {
        pos.x = -spaceSize.x +1;
        vel.x *= -1;
        stateBounce = true;
    }
    // y
    if (pos.y > spaceSize.y*1.5) {
        pos.y = spaceSize.y*1.5;
        vel.y *= -1;
        stateBounce = true;
    } else if (pos.y < -spaceSize.y+SCREEN_BOTTOM_AREA){
        pos.y = -spaceSize.y+SCREEN_BOTTOM_AREA;
        vel.y *= -1;
        stateBounce = true;
    }
    // z
    if (pos.z > 0) {
        pos.z = 0;
        vel.z *= -1;
        stateBounce = true;
    } else if (pos.z < -spaceSize.z*3){
        pos.z = -spaceSize.z*3;
        vel.z *= -1;
        stateBounce = true;
    }
}


void Object::changeProperties( Vec3f _pos, Vec3f _vel, Vec3f _size ){
    pos = _pos;
    vel = _vel;
    size = _size;
}


void Object::changePosition( float x, float y, float z ) {
    pos = Vec3f( x,y,z );
}


void Object::changeVelocity( float x, float y, float z ) {
    vel = Vec3f( x,y,z );
}


void Object::changeSize( float _size ) {
    size = Vec3f( _size,_size,_size );
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

void Object::applyAttraction( Vec3f center ) {
    acc = (center - pos) * 0.001 * mass;
}

void Object::applyAttraction( Vec3f center, float repelDist ) {
    
    Vec3f dir = center - pos;
    float dist = sqrt( dir.lengthSquared() );
    
    if ( dist < repelDist ){
        vel *= -1.3 ;
    } else acc = (center - pos) * 0.001 * mass;
}








