//
//  Space.cpp
//  Throughline
//
//  Created by EOZIN CHE on 11/6/14.
//
//

#include "_main.h"
#include "space.h"

#define STRINGIFY(x) #x


/////////////////////////////////////////////////////////////////////////// Shaders /////

static const string kVertGlsl =
STRINGIFY(
          varying vec3 vVertex;
          varying vec3 vNormal;
          
          void main(void)
          {
              // In camera-space:
              
              // Get vertex position:
              vVertex = vec3( gl_ModelViewMatrix * gl_Vertex );
              // Get vertex normal:
              vNormal = normalize( gl_NormalMatrix * gl_Normal );
              // Set vertex position:
              gl_Position = ftransform();
          }
          );

static const string kFragGlsl =
STRINGIFY(
          uniform vec3 mLightPosition;
          uniform vec4 mAmbient;
          uniform vec4 mDiffuse;
          uniform vec4 mSpecular;
          uniform float mShininess;
          
          varying vec3 vVertex;
          varying vec3 vNormal;
          
          void main (void)
          {
              // In camera-space:
              
              // Convert light position from world-space to camera-space:
              vec4 tLightCamSpace = gl_ModelViewMatrix * vec4( mLightPosition, 1.0 );
              // Compute light direction:
              vec3 tLightDir = normalize( tLightCamSpace.xyz - vVertex );
              // Compute camera vector:
              vec3 tCamVec = normalize( -vVertex );
              // Compute reflection direction for light vector:
              vec3 tRefDir = normalize( -reflect( tLightDir, vNormal ) );
              // Compute diffuse color:
              vec4 tDiff = clamp( mDiffuse * max( dot( vNormal, tLightDir ), 0.0 ), 0.0, 1.0 );
              // Compute specular color:
              vec4 tSpec = clamp( mSpecular * pow( max( dot( tRefDir, tCamVec ), 0.0 ), 0.3 * mShininess ), 0.0, 1.0 );
              // Compute final color:
              gl_FragColor = mAmbient + tDiff + tSpec;
          }
          );




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
    
    // textures
    //imgBackground = loadImage( loadAsset( "texture.jpg" ) );
    //imgOverlay = loadImage( loadAsset( "texture.jpg" ) );
    
    setCamera();
    
    displaySetup();
}


void Space::update(){
    winSize = getWindowSize();
    width = getWindowWidth();
    height = getWindowHeight();
    
    time = getElapsedFrames();
    
    if ( !(SAVE_IMAGE_ON) ) {
        //camPersp = ;
        
        setCamera();
    }
    
    
    
    
    displayUpdate();
    
    
    
    
    // check collision
    checkCollision();
    
    //remove some objects which is done!
    for( vector<Object*>::iterator it = objects.begin(); it != objects.end(); ) {
        if ( (*it)->isDone( spaceSize ) ){
            // delete the pointer
            delete *it;
            // remove the pointer from the vector
            it = objects.erase( it );
        }else{
            ++it;
        }
    }

    if (SAVE_IMAGE_ON != 1) {
        cout << "" << endl;
        cout << "NumOfObj: " << objects.size() << endl;
    }
}


void Space::display()
{
    gl::clear( Color( 0, 0, 0 ) );
    
    
    
    drawInNormal();
    
    
    
    gl::pushMatrices();
    gl::setMatrices( mCam );
    
    {
        gl::color( 1.0, 1.0, 1.0 );
        
        // Set light position (in world-space):
        Vec3f tLightPos = Vec3f( 5.0*cos(time/50.0),
                                5.0,
                                5 + 2.0*sin(time/50.0)
                          );
        
        // Bind shader:
        mShader.bind();
        
        // Set shader uniform parameters:
        mShader.uniform( "mLightPosition",	tLightPos );
        mShader.uniform( "mAmbient",		ColorA( 0.2, 0.2, 0.2, 1.0 ) );
        mShader.uniform( "mShininess",		128.0f );
        mShader.uniform( "mDiffuse",		ColorA( 0.8, 0.8, 0.8, 1.0 ) );
        mShader.uniform( "mSpecular",		ColorA( 0.6, 0.4, 0.4, 1.0 ) );
        
        
        
        drawIntoShader( tLightPos );
        
        
        
        // Unbind shader:
        mShader.unbind();
        
        // Draw light position (as a small sphere):
        //gl::drawSphere( tLightPos, 0.1 );
    }
    
    gl::popMatrices();
    
    
    
    
    // texture testing
    /*
    gl::pushMatrices();
    gl::translate( Vec3f( 0,0,100 ) );
    
    glBlendFunc(GL_DST_COLOR, GL_ONE_MINUS_SRC_ALPHA);
    gl::draw( imgOverlay, Rectf( Vec2f(0,0), Vec2f(width,height+20) ) );
    
    //glBlendFunc(GL_DST_COLOR, GL_ONE);
    gl::draw( imgBackground, Rectf( Vec2f(0,0), Vec2f(width,height+20) ) );
    
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    gl::popMatrices();
     */
    
}



void Space::createCube( float x,float y, float z ) {
    objects.insert( objects.begin(), new ObjCube( Vec3f( x,y,z ), Vec3f::zero(), 1 ) );
}
void Space::createCube( Vec3f pos ) {
    objects.insert( objects.begin(), new ObjCube( pos, Vec3f::zero(), 1 ) );
}
void Space::createCube( Vec3f pos, Vec3f vel, float mass ) {
    objects.insert( objects.begin(), new ObjCube( pos, vel, mass ) );
}

void Space::createSphere( float x,float y, float z ) {
    objects.insert( objects.begin(), new ObjSphere( Vec3f( x,y,z ), Vec3f::zero(), 1 ) );
}
void Space::createSphere( Vec3f pos ) {
    objects.insert( objects.begin(), new ObjSphere( pos, Vec3f::zero(), 1 ) );
}
void Space::createSphere( Vec3f pos, Vec3f vel, float mass ) {
    objects.insert( objects.begin(), new ObjSphere( pos, vel, mass ) );
}


void Space::checkCollision(){
    for ( int j=0; j<objects.size(); j++ ){
        for ( int i=0; i<objects.size(); i++ ){
            
            
            
            if (i != j && objects[j]->colliding) {
                Vec3f dist3f = objects[i]->pos - objects[j]->pos;
                float dist = abs(dist3f.x) + abs(dist3f.y) + abs(dist3f.z)
                - ( objects[i]->mass + objects[j]->mass );
                if ( dist < 1 ){
                    objects[i]->applyForce(dist3f * COLLISION_IMPACT);
                }
            
            
            }
        }
    }
}


void Space::setCamera() {
    float zDist = 900 / camPersp; // 1 900
    mCam.setPerspective( camPersp, getWindowAspectRatio(), 1, 1000 );
    mCam.lookAt( Vec3f( 0.0, 0.0, zDist ), Vec3f::zero() );
}
void Space::setCamera( float perspective, float zDistance ) {
    mCam.setPerspective( perspective, getWindowAspectRatio(), 1, 1000 );
    mCam.lookAt( Vec3f( 0.0, 0.0, zDistance ), Vec3f::zero() );
}


void Space::saveImage(){
    // pull down the current window as a surface and pass it to writeImage
    writeImage( getHomeDirectory() / "_cinder" / "! savedImages" / ( toString( getElapsedFrames() ) + ".png" ), copyWindowSurface() );
    cout << "Saved Frame: " << getElapsedFrames() << endl;
}


