#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/Camera.h"
#include "cinder/BSpline.h"
#include "cinder/Timeline.h"
#include "cinder/MayaCamUI.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class SplineCameraPathApp : public AppNative {
public:
    void setup();
    void prepareSettings( Settings *settings) { settings->setWindowSize(Vec2f(1280, 800)); }
    void mouseDown( MouseEvent event );
    void mouseDrag( MouseEvent event );
    void keyUp(KeyEvent event);
    void update();
    void draw();
    
    CameraPersp mMainCam1, mMainCam2, mDebugCam;
    MayaCamUI mMayaCam;
    Anim<float> mT;
    
    BSpline3f mCameraTrack;
    
    
    bool mDebug;
    
};

void SplineCameraPathApp::setup()
{
    
    mDebug = true;
    mMainCam1.setPerspective(60, getWindowAspectRatio(), .1, 100000);
    mMainCam1.lookAt(Vec3f( 0,0,10 ), Vec3f::zero()  );
    
    
    mMainCam2.setPerspective(60, getWindowAspectRatio(), .1, 100000);
    mMainCam2.lookAt(Vec3f( 5,-4,-15 ), Vec3f::zero()  );
    
    mDebugCam.setPerspective(60, getWindowAspectRatio(), .1, 100000);
    mDebugCam.lookAt(Vec3f( 0,0,20 ), Vec3f::zero()  );
    
    mMayaCam.setCurrentCam(mDebugCam);
    
    gl::enableDepthWrite();
    gl::enableDepthRead();
    
    ///+
    
    vector<Vec3f> points;
    
    points.push_back(mMainCam1.getEyePoint());
    points.push_back(Vec3f(-2,4,5));
    points.push_back(Vec3f(3,8,0));
    points.push_back(Vec3f(-1,4,-5));
    points.push_back(Vec3f(0,0,-10));
    points.push_back(mMainCam2.getEyePoint());
    
    mCameraTrack = BSpline3f( points, 4, false, true);
    mT = 0.f;
    
}

void SplineCameraPathApp::mouseDown( MouseEvent event )
{
    mMayaCam.mouseDown(event.getPos());
}



void SplineCameraPathApp::mouseDrag( MouseEvent event )
{
    mMayaCam.mouseDrag(event.getPos(), event.isLeftDown(), event.isMiddleDown(), event.isRightDown());
}

void SplineCameraPathApp::update()
{
    mMainCam1.setEyePoint(mCameraTrack.getPosition(mT));
    mMainCam1.setCenterOfInterestPoint(Vec3f( cos( getElapsedSeconds() )*5, sin( getElapsedSeconds() )*2,sin( getElapsedSeconds() )*-5) ); //Vec3f::zero()
}

void SplineCameraPathApp::keyUp(cinder::app::KeyEvent event)
{
    if(event.getChar()==' ') mDebug = !mDebug;
    if(event.getChar() == 'r') {
      //  timeline().apply(&mT, 0.f, 1.f, 3.f, EaseInOutCubic() );
        
        timeline().apply(&mT, 0.f, 1.f, 3.f, EaseInOutCirc() );
    }
}

void SplineCameraPathApp::draw()
{
    // clear out the window with black
    gl::clear( Color( 0, 0, 0 ) );
    
    if (mDebug){
        gl::setMatrices(mMayaCam.getCamera());
        gl::drawFrustum(mMainCam1);
        
        gl::begin( GL_LINE_STRIP );
        for( int i=0; i<100; i++  ){
            
            gl::vertex( mCameraTrack.getPosition((float)i/100.0f));
        }
        gl::end();
    }
    else gl::setMatrices(mMainCam1);
    gl::drawColorCube(Vec3f::zero(), Vec3f(1,1,1));
    //gl::drawSphere(Vec3f::zero(), 1);
    
}

CINDER_APP_NATIVE( SplineCameraPathApp, RendererGl )
