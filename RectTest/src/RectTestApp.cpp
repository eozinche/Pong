#include "cinder/app/AppNative.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

#include "cinder/gl/Batch.h"
#include "cinder/gl/Shader.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class RectTestApp : public AppNative {
  public:
	void setup() override;
	void mouseDown( MouseEvent event ) override;
	void update() override;
	void draw() override;
    
    gl::BatchRef mRect;
    CameraPersp mCam;
};

void RectTestApp::setup()
{
    
    mRect = gl::Batch::create(geom::Plane().size(vec2(20,20)).normal(vec3(0,0,1)), gl::getStockShader(gl::ShaderDef().color()));
    
    
    mCam.setPerspective(50, getWindowAspectRatio(), .1, 10000);
    mCam.lookAt(vec3(0,0,50), vec3(0,0,0));
}

void RectTestApp::mouseDown( MouseEvent event )
{
}

void RectTestApp::update()
{
}

void RectTestApp::draw()
{
	gl::clear( Color( 0, 0, 0 ) );
    gl::setMatrices(mCam);
    
    {
        gl::ScopedModelMatrix pushModel;
        gl::multModelMatrix(ci::translate(vec3(5*cos(getElapsedSeconds()), 0, 5*sin(getElapsedSeconds()))));
        gl::multModelMatrix(ci::rotate((float)getElapsedSeconds(),vec3(0,0,1)));
        mRect->draw();
    }
    
}

CINDER_APP_NATIVE( RectTestApp, RendererGl )
