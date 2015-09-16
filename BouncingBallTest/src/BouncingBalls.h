//
//  BouncingBalls.h
//  BouncingBallTest
//
//  Created by EOZIN CHE on 10/16/14.
//
//

#ifndef __BouncingBallTest__BouncingBalls__
#define __BouncingBallTest__BouncingBalls__

#include <stdio.h>

//#endif /* defined(__BouncingBallTest__BouncingBalls__) */


#include "cinder/app/AppNative.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

#include "cinder/gl/Batch.h"
#include "cinder/gl/GlslProg.h"
#include "cinder/gl/Shader.h"

#endif /* defined(__BouncingBalls__BouncingBalls__) */


using namespace ci;
using namespace ci::app;
using namespace std;


enum LightingModel {
    
    FLAT,
    PHONG
    
};

class Object {
    vec3    pos     = vec3( 0, 2 ,0 );
    vec3    vel     = vec3( -0.2f, 0 ,-0.05f );
    vec3    acc     = vec3( 0,-0.01f,0 );
    
    float   rad;
    
public:
    vec4                mLightPos;
    vec4                mLightPos1;
    
    gl::BatchRef        mBall, mCube, mRect;
    gl::BatchRef        mLightBulb;
    
    void create(gl::GlslProgRef _shader) {
        
        mBall = gl::Batch::create(geom::Sphere().radius(.4).subdivisions(12).enable(geom::Attrib::NORMAL), _shader);
        mCube = gl::Batch::create(geom::Cube().enable(geom::Attrib::NORMAL), _shader);
        mRect = gl::Batch::create(geom::Plane().size(vec2(16,6)), _shader);
        
        mLightBulb = gl::Batch::create(geom::Sphere().radius(.1), gl::getStockShader(gl::ShaderDef().color()));
        
        mLightPos = vec4( 5,5,5, 1 );
        
        mBall->setGlslProg(_shader);
        
    }
    void update(){
        
        mLightPos = vec4( vec3(cos(getElapsedSeconds())*3, sin(getElapsedSeconds()*2)*3, sin(getElapsedSeconds())*3)*.5f, 1. );
        mLightPos1 = vec4( vec3( cos(getElapsedSeconds())*3, 0, sin(getElapsedSeconds())*3), 1 );
        
    }
    void display(){
        
        mBall->getGlslProg()->uniform("lightPositions[0]",  vec3( gl::getModelView() * mLightPos ) );
        mBall->getGlslProg()->uniform("lightPositions[1]",  vec3( gl::getModelView() * mLightPos1 ) );
        
        
        //        {
        //            gl::ScopedMatrices push;
        //            gl::translate(vec3(0,0,-3));
        //            //gl::rotate( M_PI , vec3(1,0,0) );
        //            mRect->draw();
        //        }
        //        {
        //            gl::ScopedMatrices push;
        //            gl::translate(vec3(0,-2.5,0));
        //            gl::rotate( -M_PI/2 , vec3(1,0,0) );
        //            mRect->draw();
        //        }
        //        {
        //            gl::ScopedMatrices push;
        //            gl::translate(vec3(-6.5,0,0));
        //            gl::rotate( M_PI/2 , vec3(0,1,0) );
        //            mRect->draw();
        //        }
        //        {
        //            gl::ScopedMatrices push;
        //            gl::translate(vec3(6.5,0,0));
        //            gl::rotate( -M_PI/2 , vec3(0,1,0) );
        //            mRect->draw();
        //        }
        
        
        
        {
            gl::ScopedMatrices push;
            gl::translate(vec3(2,0,0));
            gl::scale(vec3(0.5,0.5,0.5));
            gl::rotate((float)getElapsedSeconds()*1. , vec3(1,1,1) );
            mCube->draw();
        }
        
        {
            gl::ScopedMatrices push;
            gl::translate(pos);
            gl::rotate((float)getElapsedSeconds()*.5f , vec3(1,1,1) );
            mBall->draw();
        }
        
        vel += acc;
        pos += vel;
        
        if (pos.y > 2){
            pos.y = 1.9f ;
            vel.y *= -1.025f ;
        }
        
        if (pos.y < -2){
            pos.y += 0.1f ;
            vel.y *= -1.025f ;
        }
        
        if (pos.z < -2){
            pos.z += 0.1f ;
            vel.z *= -1.0f ;
            
        }
        if (pos.z > 2){
            pos.z -= 0.1f ;
            vel.z *= -1.0f ;
            
        }
        
        if (pos.x < -13){
            pos.x += 0.1f ;
            vel.x *= -1.0f ;
            
        }
        if (pos.x > 13){
            pos.x -= 0.1f ;
            vel.x *= -1.0f ;
            
        }
        
        //vel *= 0.98;
        
        
        {
            gl::ScopedMatrices push;
            gl::translate(vec3(2,0,0));
            gl::rotate((float)getElapsedSeconds()*1. , vec3(1,1,1) );
            mBall->draw();
        }
        
        {
            gl::ScopedMatrices push;
            gl::translate(vec3(-2,0,0));
            gl::scale(vec3(1.6,1.6,1.6));
            gl::rotate( (float)getElapsedSeconds()*7., vec3(-1,-1,-1) );
            mBall->draw();
        }
        
        {
            gl::ScopedMatrices push;
            gl::translate(mLightPos.x, mLightPos.y, mLightPos.z);
            mLightBulb->draw();
        }
        
        {
            gl::ScopedMatrices push;
            gl::scale(vec3(1.5,1.5,1.5));
            gl::translate(mLightPos1.x, mLightPos1.y, mLightPos1.z);
            mLightBulb->draw();
        }
    }
    
};


class BouncingBalls {
    
    CameraPersp         mCamera;
    
    gl::GlslProgRef     mFlatShading;
    gl::GlslProgRef     mPhongShading;
    
    Object              mBall;
    
public:
    
    void setup(){
        
        // shaders setup
        mFlatShading = gl::GlslProg::create(loadAsset("Flat.vert"), loadAsset("Flat.frag"));
        mPhongShading = gl::GlslProg::create(loadAsset("Phong.vert"), loadAsset("Phong.frag"));
        
        mBall.create(mPhongShading);
        
        // camera setup
        mCamera.setPerspective(60, getWindowAspectRatio(), .1, 10000);
        mCamera.lookAt(vec3(0,0,5),vec3(0,0,0));
        
        gl::enableDepthWrite();
        gl::enableDepthRead();
        
    }
    
    void update(){
        
        mBall.update();
        
    }
    void display(){
        
        gl::setMatrices(mCamera);
        mBall.display();
        
    }
    
    void checkBouncing(){
        
        
    }
    
};

