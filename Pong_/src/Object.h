//
//  Object.h
//  _PONG
//
//  Created by EOZIN CHE on 11/14/14.
//
//
#pragma once

#include "_main.h"


/////////////////////////////////////////////////////////////////////////// Object /////
class Object {
public:
    string  id;
    
    Vec3f   pos;
    Vec3f   vel;
    Vec3f   acc;
    
    float   mass;
    Vec3f   size;
    Vec3f   sizeVel;
    
    Vec3f   axis;
    float   angle;
    
    float   velReduction;
    
    bool    immobility;
    bool    wireframe;
    bool    bouncing;
    bool    colliding;
    
    bool    stateBounce;
    bool    stateCollision;
    
    Object( Vec3f _pos, Vec3f _vel, float _size, string _id ) {
        id              = _id;
        
        pos             = _pos;
        vel             = _vel;
        acc             = Vec3f( 0,0,0 );
        
        size            = Vec3f( _size, _size, _size );
        sizeVel         = Vec3f( 0,0,0 );
        mass            = ( size.x+size.y+size.z )/3;
        
        axis            = Vec3f( 1.0,0.0,0.5 );
        angle           = 0;
        
        immobility      = false;
        wireframe       = false;
        bouncing        = false;
        colliding       = false;
    }
    ~Object(){}
    
    void update( float velReduction );
    virtual void display();
    
    void setId( string _id );
    bool getId( string _id );
    
    void changeProperties( Vec3f _pos, Vec3f _vel, Vec3f _size );
    void changePosition( float x, float y, float z );
    void changeVelocity( float x, float y, float z );
    void changeSize( float _size );
    void changeSize( float x, float y, float z );
    
    void applyForce( Vec3f force );
    void applyForce( float x, float y, float z );
    
    void applyAttraction( Vec3f center );
    void applyAttraction( Vec3f center, float repelDist );
    
    void checkHittingBoundary( Vec3f spaceSize );
    
    bool isDone( Vec3f spaceSize );
};





// Object Cube - Inheritance Polymorphism
class ObjCube : public Object {
public:
    ObjCube ( Vec3f _pos, Vec3f _vel, float _size, string _id )
    : Object(_pos, _vel, _size, _id)
    {
        pos     = _pos;
        vel     = _vel;
        size    = Vec3f( _size, _size, _size );
        id      = _id;
    }
    
    void display(){
        gl::pushMatrices();
        gl::translate( pos );
        gl::rotate( Quatf( axis, angle) );
        if ( wireframe ) gl::drawStrokedCube( Vec3f::zero(), size );
        else gl::drawCube( Vec3f::zero(), size );
        gl::popMatrices();
    }
};





// Object Sphere - Inheritance Polymorphism
class ObjSphere : public Object {
public:
    ObjSphere ( Vec3f _pos, Vec3f _vel, float _size, string _id )
    : Object(_pos, _vel, _size, _id)
    {
        pos     = _pos;
        vel     = _vel;
        size    = Vec3f( _size, _size, _size );
        id      = _id;
    }
    
    void display(){
        float avgSize = ( size.x+size.y+size.z ) / 3;
        gl::pushMatrices();
        gl::translate( pos );
        gl::rotate( Quatf( axis, angle) );
        gl::drawSphere( Vec3f::zero(), avgSize, 50 );
        gl::popMatrices();
    }
};




/////////////////////////////////////////////////////////////////////////// Cubemesh /////

// Please note that this class should be really weird and abstract since I just tried a new method.

class ObjMesh {
public:
    string              id;
    
    Vec3f               pos;
    
    int                 numU;
    int                 numV;
    Vec3f               unitSize;
    Vec2f               gap;
    
    Vec3f               axis;
    float               angle;
    
    ObjMesh( Vec3f _pos, int _numU, int _numV, string _id ) {
        id              = _id;
        
        pos             = _pos;
        
        numU            = _numU;
        numV            = _numV;
        unitSize        = Vec3f( 1,1,1 );
        gap             = Vec2f::zero();
        
        axis            = Vec3f::zero();
        angle           = 0;
    }
};







