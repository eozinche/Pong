#include "_main.h"
#include "Space.h"


class _PongApp : public AppNative {
public:
    Renderer* prepareRenderer() { return new RendererGl( RendererGl::AA_MSAA_2 ); }
    
    void setup();
    void prepareSettings( Settings *settings );
    void update();
    void draw();
    
    void resize();
    
    void mouseDown( MouseEvent event );
    void mouseUp( MouseEvent event );
    void mouseMove( MouseEvent event );
    void mouseDrag( MouseEvent event );
    void keyDown( KeyEvent event );
    
    Space theSpace;
    
    
    
    
    /////////////////////////////////// Camera
    
    CameraPersp mMainCam1, mMainCam2, mMasterDebugCam, mDebugCam;
    MayaCamUI   mMayaCam;
    
    bool        mDebug;
    float		floorLevel;
};


void _PongApp::prepareSettings( Settings *settings )
{
    settings->setTitle( "PONG / Big Screens 2014" );
    settings->setFrameRate(30);
    if ( SAVE_IMAGE_ON ) settings->setResizable( false );
    settings->setWindowSize( WINDOW_WIDTH, WINDOW_HEIGHT );
}


void _PongApp::setup()
{
    
    setWindowSize( WINDOW_WIDTH, WINDOW_HEIGHT );
    theSpace.setup();
    if ( !(SAVE_IMAGE_ON) ) theSpace.setGUIs();
    
    
    
    //////////////////////////// Camera
    
    mDebug = true;
    
    mMainCam1.setPerspective( 30, getWindowAspectRatio(), 1, 10000 );
    mMainCam1.lookAt( Vec3f( 0, 0, 50 ), Vec3f::zero() );
    
    mMainCam2.setPerspective( 30, getWindowAspectRatio(), 1, 100000 );
    mMainCam2.lookAt( Vec3f( 0, 0, 25 ), Vec3f::zero() );
    
    //    mMainCam2.setPerspective( 60, getWindowAspectRatio(), 1, 10000 );
    //    mMainCam2.lookAt( Vec3f( 30.0, 10.0, 15 ), Vec3f::zero() );
    
    
    //mFrontCam
    mDebugCam.setPerspective(35, getWindowAspectRatio(), 1, 100000);
    mDebugCam.lookAt(Vec3f( 0,0,20 ), Vec3f::zero()  );
    
    mMayaCam.setCurrentCam(mDebugCam);
}


void _PongApp::update()
{
    theSpace.update();
    
    
    
    //////////////////////Camera
    mMainCam1.setEyePoint(mMainCam1.getEyePoint()); ////////// ++
    mMainCam1.setCenterOfInterestPoint(Vec3f::zero() ); //Vec3f::zero() //cos( getElapsedSeconds() )*5, sin( getElapsedSeconds() )*2,sin( getElapsedSeconds() )*-5)
    
    mMainCam2.setEyePoint(Vec3f(-14,7,17)); ////////// ++
    mMainCam2.setCenterOfInterestPoint(Vec3f::zero() ); //Vec3f::zero() //cos( getElapsedSeconds() )*5, sin( getElapsedSeconds() )*2,sin( getElapsedSeconds() )*-5)
    
}


void _PongApp::draw()
{
    theSpace.display();
    if ( SAVE_IMAGE_ON ) theSpace.saveImage();
    else {
        if (SHOW_PARAMETERS) theSpace.GUI->draw();
    }
    
    
    
    
    // added
    
    gl::pushMatrices();
    ///draw 3d stufff
    gl::color( 1.0, 1.0, 1.0, 0.1 );
    gl::lineWidth(5);
    if (mDebug){
        gl::setMatrices(mMayaCam.getCamera());
        //cout << mMayaCam.getCamera().getEyePoint()<< endl; //(-14,7,17)
        gl::drawFrustum(mDebugCam);
        
    } else {
        gl::setMatrices(mMainCam1);
    }
    gl::enableAlphaBlending();
    gl::enableDepthRead( true );
    gl::enableDepthWrite( false );
    gl::popMatrices();
    
    
    gl::pushMatrices();
    gl::setMatricesWindow(getWindowSize());
    //draw 2d stuff not affected by the camera
    
    //  gl::pushMatrices();
    gl::color( 1.0, 1.0, 1.0, 0.1 );
    gl::lineWidth(2);
    //gl::drawSolidRect( Rectf( Vec2f(0,getWindowHeight()*973/1080), Vec2f(width,getWindowHeight()) ) );
    //gl::drawLine(Vec2f(0,getWindowHeight()*973/1080), Vec2f(getWindowWidth(),getWindowHeight()*973/1080));
    //  gl::popMatrices();
    
    gl::popMatrices();
    
}


void _PongApp::resize()
{
    // Setup camera:
    theSpace.setCamera();
}


void _PongApp::keyDown(KeyEvent event)
{
    switch( event.getCode() ) {
        case KeyEvent::KEY_r:
            theSpace.objects.clear();
            theSpace.setup();
            break;
    }
    
    if(event.getChar()==' ') mDebug = !mDebug;
}


void _PongApp::mouseDown( MouseEvent event )
{
    mMayaCam.mouseDown(event.getPos());
}

void _PongApp::mouseUp( MouseEvent event )
{
    
}

void _PongApp::mouseMove( MouseEvent event )
{
    
}

void _PongApp::mouseDrag( MouseEvent event )
{
    mMayaCam.mouseDrag(event.getPos(), event.isLeftDown(), event.isMiddleDown(), event.isRightDown());
}










CINDER_APP_NATIVE( _PongApp, RendererGl )



