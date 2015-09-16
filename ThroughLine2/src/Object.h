//
//  Object.h
//  Throughline
//
//  Created by MOQN on 11/14/14.
//
//
#pragma once

#include "_main.h"


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
    
    bool    immobility;
    bool    wireframe;
    bool    bouncing;
    bool    colliding;
    
    Object( Vec3f _pos, Vec3f _vel, float _mass ) {
        pos             = _pos;
        vel             = _vel;
        mass            = _mass;
        
        acc             = Vec3f( 0,0,0 );
        size            = Vec3f( 1,1,1 );
        
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
    
    void changeProperties( Vec3f _pos, Vec3f _vel, float _mass );
    void changePosition( float x, float y, float z );
    void changeVelocity( float x, float y, float z );
    void changeSize( float x, float y, float z );
    
    void applyForce( Vec3f force );
    void applyForce( float x, float y, float z );           // overriding
    
    bool checkHitBoundary( Vec3f spaceSize );
    bool isDone( Vec3f spaceSize );
};





// Object Cube - Inheritance Polymorphism
class ObjCube : public Object {
public:
    ObjCube ( Vec3f _pos, Vec3f _vel, float _mass )
    : Object(_pos, _vel, _mass)
    {
        pos = _pos;
        vel = _vel;
        mass = _mass;
    }
    
    void display(){
        gl::pushMatrices();
        gl::translate( pos );
        gl::rotate( Quatf( axis, angle) );
        if ( wireframe ) gl::drawStrokedCube( Vec3f::zero(), size*mass );
        else gl::drawCube( Vec3f::zero(), size*mass );
        gl::popMatrices();
    }
};





// Object Sphere - Inheritance Polymorphism
class ObjSphere : public Object {
public:
    ObjSphere ( Vec3f _pos, Vec3f _vel, float _mass )
    : Object(_pos, _vel, _mass)
    {
        pos = _pos;
        vel = _vel;
        mass = _mass;
    }
    
    void display(){
        gl::pushMatrices();
        gl::translate( pos );
        gl::rotate( Quatf( axis, angle) );
        
        gl::drawSphere( Vec3f::zero(), mass );
        gl::popMatrices();
    }
};





