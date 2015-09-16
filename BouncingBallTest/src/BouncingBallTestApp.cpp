#include "cinder/app/AppNative.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"


#include "cinder/gl/Batch.h"
#include "cinder/gl/GlslProg.h"
#include "cinder/gl/Shader.h"

#include "BouncingBalls.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class BouncingBallTestApp : public AppNative {
  public:
    void preparesettings( Settings *settings );
    
    void setup() override;
	void update() override;
	void draw() override;

    void mouseDown( MouseEvent event ) override;
    void keyUp( KeyEvent event ) override;
    
    BouncingBalls mBouncingBalls;
};


void BouncingBallTestApp::preparesettings( Settings *settings ){
    
    settings->setWindowSize( 3840/4, 1080/2 );
    //settings->setWindowSize( 3840, 1080 );    //IAC BigScreens Size
    //settings->setFullScreen();
    //settings->setFrameRate( 60.0f );
    
}


void BouncingBallTestApp::setup()
{
    mBouncingBalls.setup();
}

void BouncingBallTestApp::update()
{
    mBouncingBalls.update();
}

void BouncingBallTestApp::draw()
{
	gl::clear( Color( 0, 0, 0 ) );
    mBouncingBalls.display();
}


void BouncingBallTestApp::mouseDown( MouseEvent event )
{
}

void BouncingBallTestApp::keyUp( KeyEvent event )
{
    
    //    if( event.getChar() == '1' ){
    //        mCurrentModel = FLAT;
    //
    //        mTorus->setGlslProg(mFlatShading);
    //        mTeapot->setGlslProg(mFlatShading);
    //
    //    }else if( event.getChar() == '2' ){
    //        mCurrentModel = PHONG;
    //
    //        mTorus->setGlslProg(mPhongShading);
    //        mTeapot->setGlslProg(mPhongShading);
    //        mBall->setGlslProg(mPhongShading);
    //
    //    }
}



CINDER_APP_NATIVE( BouncingBallTestApp, RendererGl )
