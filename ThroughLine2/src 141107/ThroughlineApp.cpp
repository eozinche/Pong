#include "Space.h"


class ThroughlineApp : public AppNative {
public:
    void setup();
    void mouseDown(MouseEvent event);
    void update();
    void draw();
    void resize();
    
    Space theSpace;
};


void ThroughlineApp::setup(){
    setWindowSize( WINDOW_WIDTH, WINDOW_HEIGHT );
    theSpace.setup();
}

void ThroughlineApp::update()
{
    theSpace.update();
}


void ThroughlineApp::draw()
{
    gl::clear( Color( 0, 0, 0 ) );
    theSpace.display();
    if (SAVE_IMAGE_ON == 1) theSpace.saveImage();
}


void ThroughlineApp::mouseDown(MouseEvent event)
{
}


void ThroughlineApp::resize()
{
    // Setup camera:
    theSpace.mCam.setPerspective( 60, getWindowAspectRatio(), 1, 1000 );
    theSpace.mCam.lookAt( Vec3f( 0.0, 0.0, 10.0 ), Vec3f::zero() );
}


CINDER_APP_NATIVE( ThroughlineApp, RendererGl )
