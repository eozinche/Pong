#include "_main.h"
#include "Space.h"




class ThroughlineApp : public AppNative {
public:
    void setup();
    void prepareSettings( Settings *settings );
    void update();
    void draw();
    
    void resize();
    void keyDown(KeyEvent event);
    void mouseDown(MouseEvent event);
    
    
    Space theSpace;
};


void ThroughlineApp::prepareSettings( Settings *settings )
{
    settings->setTitle( "THROUGHLINE / Big Screens 2014" );
    if ( SAVE_IMAGE_ON ) settings->setResizable( false );
    settings->setWindowSize( WINDOW_WIDTH, WINDOW_HEIGHT );
}


void ThroughlineApp::setup()
{
    theSpace.setup();
    if ( !(SAVE_IMAGE_ON) ) theSpace.setGUIs();
}


void ThroughlineApp::update()
{
    theSpace.update();
}


void ThroughlineApp::draw()
{
    theSpace.display();
    if ( SAVE_IMAGE_ON ) theSpace.saveImage();
    else theSpace.GUI->draw();
}


void ThroughlineApp::resize()
{
    // Setup camera:
    theSpace.setCamera();
}


void ThroughlineApp::keyDown(KeyEvent event)
{
    switch( event.getCode() ) {
        case KeyEvent::KEY_r:
            theSpace.objects.clear();
            theSpace.setup();
            break;
    }
}


void ThroughlineApp::mouseDown(MouseEvent event)
{
}








CINDER_APP_NATIVE( ThroughlineApp, RendererGl )



