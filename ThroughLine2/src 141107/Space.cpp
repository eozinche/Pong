//
//  Space.cpp
//  Throughline
//
//  Created by MOQN on 11/6/14.
//
//

#include "Space.h"



/////////////////////////////////////////////////////////////////////////// Space /////

void Space::setup()
{
    // Enable alpha blending:
    gl::enableAlphaBlending();
    
    // Enable depth buffer read/write:
    gl::enableDepthRead();
    gl::enableDepthWrite();
    
    // Load shader from strings (using the stringify macro):
    mShader = gl::GlslProg( kVertGlsl.c_str(), kFragGlsl.c_str() );
    
    winSize = getWindowSize();
    width = getWindowWidth();
    height = getWindowHeight();
    
    for (int i=0; i<1; i++){
        createSphere(Vec3f( -40.0, 10, 0 ),
                     Vec3f( 0.8, 0.2, randFloat(-0.2,0.2) ),
                     2);
    }
}

void Space::update(){
    winSize = getWindowSize();
    width = getWindowWidth();
    height = getWindowHeight();
    
    for ( auto& obj : spheres ) {
        updateObjects( &obj, gravity, wind, true );
    }
    for ( auto& obj : cubes ) {
        updateObjects( &obj, gravity, wind, false );
    }
    for ( auto& obj : frameCubes ) {
        updateObjects( &obj, gravity*0, wind, false );
    }
    
    for (int i=cubes.size()-1; i>=0; i--){
        if ( cubes[i].pos.y < -spaceSize.y ) {
            cubes.erase(cubes.begin()+i);
        }
    }
}


void Space::updateObjects( Object* obj, float _gravity, Vec3f _wind, bool _edge ){
    Vec3f newGravity = -Vec3f(0, _gravity * obj->mass, 0);
    
    obj->applyForce(newGravity);
    obj->applyForce(_wind);
    
    obj->update();
    if (_edge) {
        bool hit = obj->checkEdges( spaceSize );
        if (hit) {
            if ( getElapsedFrames() > 500 ) {
                for (int i=0; i<randFloat(0,10); i++){
                    createCube(
                               obj->pos,
                               Vec3f( randFloat(-0.1,0.1),
                                     randFloat(0.0,1.0),
                                     randFloat(-0.1,0.1)
                                     ),
                               randFloat(1.0) * getElapsedFrames()/500
                               );
                }
            }
            if ( getElapsedFrames() > 1500 ) {
                for (int i=0; i<randFloat(0,3); i++){
                    createFrameCube(
                                    obj->pos,
                                    Vec3f( 0,0,0 ),
                                    randFloat(0,10)
                                    );
                }
                gravity = 0.001;
            }
            if ( getElapsedFrames() > 2500 ) {
                gravity = 0.01;
            }
        }
    }
}


void Space::display()
{
    gl::clear( Color( 0, 0, 0 ) );
    gl::setMatrices( mCam );
    gl::color( 1.0, 1.0, 1.0 );
    
    // Set light position (in world-space):
    float time = getElapsedFrames()/50.0;
    Vec3f tLightPos = Vec3f( 5.0*cos(time), 5.0, 5 + 2.0*sin(time) );
    
    
    // Bind shader:
    mShader.bind();
    
    /*
    mShader.uniform( "mLightPosition",	tLightPos );
    gl::pushMatrices();
    gl::translate( Vec3f( 0,-12, 0.0 ) );
    gl::rotate( Quatf( Vec3f( 1.0,0,0 ), 90) );
    gl::drawSolidRect( Rectf( Vec2f(-24,-18), Vec2f(24,24) ) );
    gl::popMatrices();
     */
    
    // Set shader uniform parameters:
    //mShader.uniform( "mLightPosition",	Quatf( Vec3f( 1.0,0,0 ), 90) * tLightPos );
    mShader.uniform( "mAmbient",		ColorA( 0.2, 0.2, 0.2, 1.0 ) );
    mShader.uniform( "mShininess",		128.0f );
    mShader.uniform( "mDiffuse",		ColorA( 0.8, 0.8, 0.8, 1.0 ) );
    mShader.uniform( "mSpecular",		ColorA( 0.6, 0.4, 0.4, 1.0 ) );     // point color
    
    
    // Draw sphere:
    
    for ( auto& obj : spheres ){
        mShader.uniform( "mLightPosition",	Quatf( Vec3f( obj.axis ), -obj.angle) * tLightPos );
        obj.drawSphere();
    }
    for ( auto& obj : cubes ){
        mShader.uniform( "mLightPosition",	Quatf( Vec3f( obj.axis ), -obj.angle) * tLightPos );
        obj.drawCube();
    }
    for ( auto& obj : frameCubes ){
        mShader.uniform( "mLightPosition",	Quatf( Vec3f( obj.axis ), -obj.angle) * tLightPos );
        obj.drawStrokedCube();
    }
    
    // Unbind shader:
    mShader.unbind();
    
    // Draw light position (as a small sphere):
    gl::drawSphere( tLightPos, 0.1 );
}

void Space::checkCollision(){
    for ( int j=0; j<spheres.size(); j++ ){
        for ( int i=0; i<spheres.size(); i++ ){
            if (i != j) {
                Vec3f dist3f = spheres[i].pos-spheres[j].pos;
                float dist = abs(dist3f.x) + abs(dist3f.y) + abs(dist3f.z)
                - ( spheres[i].mass + spheres[j].mass );
                if ( dist < 1 ){
                    spheres[i].applyForce(dist3f*0.0001);
                }
            }
        }
    }
}

void Space::createSphere( Vec3f pos, Vec3f vel, float mass ){
    spheres.push_back( Object(pos, vel, mass) );
}
void Space::createCube( Vec3f pos, Vec3f vel, float mass ){
    cubes.push_back( Object(pos, vel, mass) );
}
void Space::createFrameCube( Vec3f pos, Vec3f vel, float mass ){
    frameCubes.push_back( Object(pos, vel, mass) );
}


void Space::saveImage(){
    // pull down the current window as a surface and pass it to writeImage
    writeImage( getHomeDirectory() / "_cinder" / "! savedImage" / ( toString( getElapsedFrames() ) + ".png" ), copyWindowSurface() );
    cout << "Saved Frame: " << getElapsedFrames() << endl;
}



/////////////////////////////////////////////////////////////////////////// Object /////

void Object::applyForce(Vec3f force) {
    Vec3f f = force / mass;
    acc += f;
}

void Object::update() {
    vel += acc;
    pos += vel;
    acc *= 0;
    vel *= velReduction;
    
    updateRotation();
}

void Object::updateRotation() {
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


bool Object::checkEdges( Vec3f edge ) {
    if (pos.x > edge.x) {
        pos.x = edge.x;
        vel.x *= -1;
        return true;
    } else if (pos.x < -edge.x) {
        pos.x = -edge.x;
        vel.x *= -1;
        return true;
    }
    
    if (pos.y > edge.y) {
        pos.y = edge.y;
        vel.y *= -1;
        return true;
    } else if (pos.y < -edge.y){
        pos.y = -edge.y;
        vel.y *= -1;
        return true;
    }
    
    if (pos.z > 0) {
        vel.z *= -1;
        pos.z = 0;
        return true;
    } else if (pos.z < -edge.z*2){
        vel.z *= -1;
        pos.z = -edge.z*2;
        return true;
    }
    return false;
}

void Object::drawSphere() {
    gl::pushMatrices();
    {
        gl::translate( pos );
        gl::rotate( Quatf( axis, angle) );
        gl::drawSphere( Vec3f::zero(), mass, 30 );
    }
    gl::popMatrices();
}

void Object::drawCube() {
    gl::pushMatrices();
    {
        gl::translate( pos );
        gl::rotate( Quatf( axis, angle) );
        gl::drawCube( Vec3f::zero(), size*mass );
    }
    gl::popMatrices();
}

void Object::drawStrokedCube() {
    gl::pushMatrices();
    {
        gl::translate( pos );
        gl::rotate( Quatf( axis, angle) );
        gl::drawStrokedCube( Vec3f::zero(), size*mass );
    }
    gl::popMatrices();
}




