//
//  SpaceDisplay.cpp
//  Throughline
//
//  Created by MOQN on 11/14/14.
//
//


#include "_main.h"
#include "space.h"

#include "Renderer.h"                                               //EOZIN
#include "cinder/Perlin.h"                                          //EOZIN
#include "Resources.h"                                              //EOZIN

#include "Particle.h"                                               //EOZIN
#include "Emitter.h"                                                //EOZIN



/////////////////////////////////////////////////////////////////////////// SetUp /////

void Space::displaySetup()
{
    //  createSpheres
    
    //for (int i=0; i<1; i++){
    createSphere(Vec3f( 0, randFloat(-10, 10), 0 ),
                     Vec3f( randFloat(-0.3, 0.3), randFloat(-0.3, 0.0), randFloat(-0.3, 0.3) ),
                    1);
    objects[0]->changeSize(150., 150., 150.);
    //objects[0]->vel  = Vec3f(.5,.5,.5);
    objects[0]->changeVelocity(.2,.2,0);
    objects[0]->bouncing = true;
    
    mouseIsDown = false;
    mSpherePos = getWindowCenter();
    //mSpherePos = Vec2i( objects[0]->pos.x, objects[0]->pos.y );

    // }
    
    
    particleImg = gl::Texture::create( loadImage( loadResource( RES_EMITTER ) ) );   //PARTICLEBLACK
    emitterImg = gl::Texture::create( loadImage( loadResource( RES_EMITTER ) ) );
    
}



/////////////////////////////////////////////////////////////////////////// Update /////

void Space::displayUpdate()
{
    
    //update the whole objects
    for ( auto& obj : objects ) {
        Vec3f tGravity = -Vec3f(0, gravity * obj->mass, 0);
        obj->applyForce( tGravity );
        obj->applyForce( wind );
        obj->update( velReduction );
        if (obj->bouncing) {
            bool hit = obj->checkHitBoundary( spaceSize );
            if ( hit ) {
                // do somthing
                // call any function
               
               // counter++;
                mouseIsDown = ! mouseIsDown;
                if(mouseIsDown){
                    mSpherePos = Vec2i( objects[0]->pos.x, objects[0]->pos.y );
                    mEmitter.addParticles( 10 * CINDER_FACTOR );
                    floorLevel = 1 / 2.0f * getWindowHeight();

                    mEmitter.exist( mSpherePos );
                }
            }
        }
    }
}


/////////////////////////////////////////////////////////////////////////// Draw /////

void Space::drawInNormal() {
    
    //////////     draw here     //////////
    gl::clear();
    gl::enableAdditiveBlending();
    
    mEmitter.exist( mSpherePos );

    
    }


void Space::drawIntoShader( Vec3f tLightPos ) {
    
    for ( auto& obj : objects ){
        mShader.uniform( "mLightPosition",	Quatf( Vec3f( obj->axis ), - obj->angle) * tLightPos );
        obj->display();
    }
  //  drawSceneOne();
}


//void Space::drawSceneOne(){
//    
//}


/////////////////////////////////////////////////////////////////////////// GUI /////

void Space::setGUIs() {
    GUI = params::InterfaceGl::create( "Parameters" , Vec2f( 200,300 ), ColorA( 0,0,0,.3 ) );
    GUI->setPosition( Vec2f( 10, 10 ) );
    
    GUI->addSeparator();
    GUI->addText( "Press 'R' to reset" );
    
    GUI->addSeparator();
    GUI->addParam("Gravity", &gravity,
                  "min=-0.05 max=0.05 step=0.001" );
    GUI->addParam("Wind", &wind);
    GUI->addParam("VelReduction", &velReduction,
                  "min=0.975 max=1.000 step=0.001" );
    
    GUI->addSeparator();
    GUI->addParam("Perspective", &camPersp,
                  "min=1.0 max=60.0 step=1.0" );
    GUI->addSeparator();
}

//
//void renderImage( Vec3f _loc, float _diam, Color _col, float _alpha )
//{
//    gl::pushMatrices();
//    gl::translate( _loc.x, _loc.y, _loc.z );
//    gl::scale( _diam, _diam, _diam );
//    gl::color( _col.r, _col.g, _col.b, _alpha );
//    gl::begin( GL_QUADS );
//    gl::texCoord(0, 0);    gl::vertex(-.5, -.5);
//    gl::texCoord(1, 0);    gl::vertex( .5, -.5);
//    gl::texCoord(1, 1);    gl::vertex( .5,  .5);
//    gl::texCoord(0, 1);    gl::vertex(-.5,  .5);
//    gl::end();
//    gl::popMatrices();
//}








