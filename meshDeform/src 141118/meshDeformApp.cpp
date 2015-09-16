#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Vbo.h"
#include "cinder/gl/GlslProg.h"
#include "cinder/gl/Fbo.h"
#include "MeshFactory.h"
#include "cinder/Camera.h"
#include "cinder/params/Params.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class meshDeformApp : public AppNative {
  public:
	void setup();
    void prepareSettings( Settings *settings );

	void mouseDown( MouseEvent event );	
	void update();
	void draw();
    
//  gl::VboMeshRef mSphere;
    
    ProtoMesh	mMeshProto;
//  gl::VboMesh mMeshVbo;
    gl::VboMesh mSphere;
    
    CameraPersp mCam;
    
    gl::Fbo mNoise;
//  gl::Fbo mFog, mCellular;
    
    gl::GlslProgRef mNoiseShader, mExtrude;
//  gl::GlslProgRef mFogShader, mCellularShader;
    
    params::InterfaceGlRef mParams;
    
    bool debug = false;
    
    float mScale;
};


void meshDeformApp::setup()
{
    mScale = 10.0f;
    
    gl::enableAlphaBlending();
    
    createSphere( 250, 250, 30.0, mMeshProto );

    createMeshVbo( mMeshProto, mSphere );
    
    mCam.setPerspective(60, getWindowAspectRatio(), .1, 10000);
    mCam.lookAt(Vec3f(0,0,100), Vec3f(0,0,0));
    
    mNoise = gl::Fbo(512,512);
//  mCellular = gl::Fbo(512,512);

    mNoiseShader = gl::GlslProg::create(loadAsset("noise.vert"), loadAsset("noise.frag"));
    
//  mFogShader = gl::GlslProg::create(loadAsset("fog.vert"), loadAsset("fog.frag"));
//  mCellularShader = gl::GlslProg::create(loadAsset("cellular.vert"), loadAsset("cellular.frag"));
    
    mExtrude = gl::GlslProg::create(loadAsset("extrude.vert"), loadAsset("extrude.frag"));
    
    mParams = params::InterfaceGl::create("uniform", Vec2f(200,200));
    
    mParams->addParam("extrusion amount", &mScale);
    
    gl::enableDepthRead();
    gl::enableDepthWrite();
    
}


void meshDeformApp::prepareSettings( Settings *settings )
{
    settings->setTitle( "THROUGHLINE / Big Screens 2014" );
   // if ( SAVE_IMAGE_ON ) settings->setResizable( false );
    settings->setWindowSize( 1024 , 768 );
}


void meshDeformApp::mouseDown( MouseEvent event )
{
    debug = !debug;
}

void meshDeformApp::update()
{
    ///////////////////////////noise test
    
    mNoise.bindFramebuffer();
    gl::clear();
    
    gl::pushMatrices();
    gl::setMatricesWindow(mNoise.getWidth(), mNoise.getHeight());
    gl::setViewport(mNoise.getBounds());
    
    mNoiseShader->bind();
    mNoiseShader->uniform("size", Vec2f(mNoise.getSize()));
    mNoiseShader->uniform("time",(float)getElapsedSeconds());
    gl::drawSolidRect(Rectf(Vec2f(0,0),mNoise.getSize()));
    mNoiseShader->unbind();
    
    gl::popMatrices();
    mNoise.unbindFramebuffer();
    
    
    ///////////////////////////cellular test
    
//    mCellular.bindFramebuffer();
//    gl::clear();
//    
//    gl::pushMatrices();
//    gl::setMatricesWindow(mCellular.getWidth(), mCellular.getHeight());
//    gl::setViewport(mCellular.getBounds());
//    
//    mCellularShader->bind();
//    mCellularShader->uniform("time",(float)getElapsedSeconds());
//    gl::drawSolidRect(Rectf(Vec2f(0,0),mCellular.getSize()));
//    mCellularShader->unbind();
//    
//    gl::popMatrices();
//    mCellular.unbindFramebuffer();
    
    
    ///////////////////////////fog test
    
//    mFog.bindFramebuffer();
//    gl::clear();
//    
//    gl::pushMatrices();
//    gl::setMatricesWindow(mFog.getWidth(), mFog.getHeight());
//    gl::setViewport(mFog.getBounds());
//    
//    mFogShader->bind();
//    //  mFogShader->uniform("time",(float)getElapsedSeconds());
//    gl::drawSolidRect(Rectf(Vec2f(0,0),mFog.getSize()));
//    mFogShader->unbind();
//    
//    gl::popMatrices();
//    mFog.unbindFramebuffer();
}

void meshDeformApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) );
    
    if(!debug){

    gl::setMatrices( mCam );
    gl::setViewport(getWindowBounds());
    gl::pushMatrices();

       // gl::rotate( Vec3f(100, 0, 0) );
        
        mNoise.getTexture().bind();
        //mCellular.getTexture().bind();
        
        mExtrude->bind();
        
        mExtrude->uniform("noiseTex",0);
        mExtrude->uniform("scale", mScale);
        gl::rotate( Vec3f(50,84,0) );
        gl::rotate( Vec3f (Vec3f( cos( getElapsedSeconds() ), cos( getElapsedSeconds() ), sin( getElapsedSeconds() ) ) * 15.0)   );
      //  gl::rotate( Vec3f (( cos( getElapsedSeconds() ) * 15.0) + 80.0 ));
        
        gl::draw( mSphere );

        mExtrude->unbind();
        mNoise.getTexture().unbind();
      //  mCellular.getTexture().unbind();

     gl::popMatrices();
    
    }
    else{
        gl::pushMatrices();
        
        gl::setMatricesWindow(getWindowSize());
        gl::setViewport(getWindowBounds());
        gl::draw(mNoise.getTexture());
      //  gl::draw(mFog.getTexture());
      //  gl::draw(mCellular.getTexture());
        
        gl::popMatrices();
    }

    mParams->draw();
}

CINDER_APP_NATIVE( meshDeformApp, RendererGl )
