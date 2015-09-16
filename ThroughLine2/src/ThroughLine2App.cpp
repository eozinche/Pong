#include "_main.h"
#include "Space.h"




class Throughline2App : public AppNative {
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


void Throughline2App::prepareSettings( Settings *settings )
{
    settings->setTitle( "THROUGHLINE / Big Screens 2014" );
    if ( SAVE_IMAGE_ON ) settings->setResizable( false );
    settings->setWindowSize( WINDOW_WIDTH, WINDOW_HEIGHT );
}


void Throughline2App::setup()
{
    theSpace.setup();
    if ( !(SAVE_IMAGE_ON) ) theSpace.setGUIs();
}


void Throughline2App::update()
{
    theSpace.update();
}


void Throughline2App::draw()
{
    theSpace.display();
    if ( SAVE_IMAGE_ON ) theSpace.saveImage();
    else theSpace.GUI->draw();
}


void Throughline2App::resize()
{
    // Setup camera:
    theSpace.setCamera();
}


void Throughline2App::keyDown(KeyEvent event)
{
    switch( event.getCode() ) {
        case KeyEvent::KEY_r:
            theSpace.objects.clear();
            theSpace.setup();
            break;
    }
}


void Throughline2App::mouseDown(MouseEvent event)
{
}



CINDER_APP_NATIVE( Throughline2App, RendererGl )
