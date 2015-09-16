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
    
    void drawGlows();
    
    ProtoMesh	mMeshProto;
//  gl::VboMesh mMeshVbo;
    gl::VboMesh mSphere;
    
    CameraPersp mCam;
    
    gl::Fbo mNoise;
//  gl::Fbo mFog, mCellular;
    
    gl::GlslProgRef mNoiseShader, mNoiseOShader, mExtrude;
    
    // SHADERS
    //gl::GlslProgRef       mFogShader, mCellularShader;
    gl::GlslProgRef         mFogShader, mCellularShader;
    
    gl::GlslProgRef         mGradientShader;
    gl::GlslProgRef 		mRoomShader;
    gl::GlslProgRef 		mStarShader;
    gl::GlslProgRef 		mGlowShader;
    gl::GlslProgRef 		mNebulaShader;
    gl::GlslProgRef 		mDustShader;
    gl::GlslProgRef 		mCoronaShader;
    gl::GlslProgRef 		mPlanetShader;
    
    gl::GlslProgRef		    mDefaultShader;
    
    
    // TEXTURES
    // CubeMap			mCubeMap;
    gl::Texture			mSpectrumTex;
    gl::Texture			mGlowTex;
    gl::Texture			mNebulaTex;
    gl::Texture			mCoronaTex;
    gl::Texture			mGridTex;
    gl::Texture			mSmallGridTex;
    gl::Texture			mBigGlow0Tex;
    gl::Texture			mBigGlow1Tex;
    gl::Texture			mIconTex;
    //
    
    int					mStage;
    vector<string>		mTitles;
    
    int					mNumStarTypes;
    int					mStarTypeIndex;
    vector<string>		mStarNames;
    vector<int>			mStarTemps;
    vector<float>		mStarRadii;
    vector<float>		mStarColors;
    vector<float>		mStarDists;
    vector<int>			mStarPlanets;
    int					mRandIterations;
    
    Vec2f				mRandSeed;
    bool				mBillboard;
    
    bool				mRenderGlows;
    bool				mRenderNebulas;
    bool				mRenderSphere;
    bool				mRenderTexture;
    bool				mRenderDusts;
    bool				mRenderCorona;
    bool				mRenderCanisMajoris;
    
    Vec3f               tLightPos;

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
    mCam.lookAt(Vec3f(0,0,100), Vec3f(cos( getElapsedSeconds() )*5, sin( getElapsedSeconds() )*2,sin( getElapsedSeconds() )*-5));
    
    mNoise = gl::Fbo(512,512);
//  mCellular = gl::Fbo(512,512);
    
    glShadeModel( GL_SMOOTH );
    
    
    // LOAD SHADERS
    try {
    mNoiseShader = gl::GlslProg::create(loadAsset("noise.vert"), loadAsset("noise.frag"));
    mNoiseOShader = gl::GlslProg::create(loadAsset("noise.vert"), loadAsset("noiseoriginal.frag"));
//  mFogShader = gl::GlslProg::create(loadAsset("fog.vert"), loadAsset("fog.frag"));
//  mCellularShader = gl::GlslProg::create(loadAsset("cellular.vert"), loadAsset("cellular.frag"));
    
    mGradientShader = gl::GlslProg::create(loadAsset("passThru.vert"), loadAsset("gradient.frag"));
    mRoomShader = gl::GlslProg::create(loadAsset("room.vert"), loadAsset("room.frag"));
    mStarShader = gl::GlslProg::create(loadAsset("star.vert"), loadAsset("star.frag"));
    mGlowShader = gl::GlslProg::create(loadAsset("passThru.vert"), loadAsset("glow.frag"));
    mNebulaShader = gl::GlslProg::create(loadAsset("passThru.vert"), loadAsset("nebula.frag"));
    mDustShader = gl::GlslProg::create(loadAsset("passThru.vert"), loadAsset("corona.frag"));
    mCoronaShader = gl::GlslProg::create(loadAsset("passThruColor.vert"), loadAsset("dust.frag"));
    mPlanetShader = gl::GlslProg::create(loadAsset("passThruNormals.vert"), loadAsset("planet.frag"));
 
    
    mExtrude = gl::GlslProg::create(loadAsset("extrude.vert"), loadAsset("extrude.frag"));
        
    mDefaultShader = gl::GlslProg::create(loadAsset("default.vert"), loadAsset("default.frag"));

    
} catch( gl::GlslProgCompileExc e ) {
    std::cout << e.what() << std::endl;
    quit();
    }
    

    ///////////////////////////////////////////////////////////////////
    
    // LOAD TEXTURES
    mSpectrumTex		= gl::Texture( loadImage( loadAsset( "spectrum.png" ) ) );
    mGlowTex			= gl::Texture( loadImage( loadAsset( "glow.png" ) ) );
    mNebulaTex			= gl::Texture( loadImage( loadAsset( "nebula.png" ) ) );
    mCoronaTex			= gl::Texture( loadImage( loadAsset( "corona.png" ) ) );
    mGridTex			= gl::Texture( loadImage( loadAsset( "grid.png" ) ) );
    mSmallGridTex		= gl::Texture( loadImage( loadAsset( "smallGrid.png" ) ) );
    mBigGlow0Tex		= gl::Texture( loadImage( loadAsset( "bigGlow0.png" ) ) );
    mBigGlow1Tex		= gl::Texture( loadImage( loadAsset( "bigGlow1.png" ) ) );
    mIconTex			= gl::Texture( loadImage( loadAsset( "iconStar.png" ) ) );

/*
    mCubeMap			= CubeMap( GLsizei(512), GLsizei(512),
                                  Surface8u( loadImage( loadResource( RES_CUBE1_ID ) ) ),
                                  Surface8u( loadImage( loadResource( RES_CUBE2_ID ) ) ),
                                  Surface8u( loadImage( loadResource( RES_CUBE3_ID ) ) ),
                                  Surface8u( loadImage( loadResource( RES_CUBE4_ID ) ) ),
                                  Surface8u( loadImage( loadResource( RES_CUBE5_ID ) ) ),
                                  Surface8u( loadImage( loadResource( RES_CUBE6_ID ) ) )
                                  );
*/
    
    
    ///////////////////////////////////////////////////////////////////
    
    
    // RENDER MODES
    mBillboard			= false;
    mRenderGlows		= false;
    mRenderSphere		= false;
    mRenderNebulas		= false;
    mRenderTexture		= false;
    mRenderDusts		= false;
    mRenderCorona		= false;
    mRenderCanisMajoris = false;
    
    mStage				= 0;
    mRandIterations		= 0;
    
    
    ////////////////////////////////////////////////////////////////////
    
    
    
    
    mParams = params::InterfaceGl::create("uniform", Vec2f(200,200));
    
    mParams->addParam("extrusion amount", &mScale);
    
    gl::enableDepthRead();
    gl::enableDepthWrite();
    
}


void meshDeformApp::prepareSettings( Settings *settings )
{
    settings->setTitle( "THROUGHLINE / Big Screens 2014" );
   // if ( SAVE_IMAGE_ON ) settings->setResizable( false );
   // settings->setWindowSize( 11520/7 , 1080/7 );
    settings->setWindowSize( 1920 , 1080 );
   // settings->setBorderless( true );
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
    
    
    // mNoiseOriginal shader
//    mNoiseOShader->bind();
//    mNoiseOShader->uniform("time",(float)getElapsedSeconds());
//    gl::drawSolidRect(Rectf(Vec2f(0,0),mNoise.getSize()));
//    mNoiseOShader->unbind();
//    
    
    gl::popMatrices();
    mNoise.unbindFramebuffer();
    
    mCam.setCenterOfInterestPoint(Vec3f(cos( getElapsedSeconds() )*10, sin( getElapsedSeconds() )*10,sin( getElapsedSeconds() )*-5));
    gl::rotate(Vec3f(getElapsedSeconds()*.5,getElapsedSeconds()*5,0));
    

    
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

//void meshDeformApp::drawGlows()
//{
//    mGlowTex.bind( 0 );
//    mSmallGridTex.bind( 1 );
//    mSpectrumTex.bind( 2 );
//    mGlowShader->bind();
//    mGlowShader->uniform( "glowTex", 0 );
//    mGlowShader->uniform( "gridTex", 1 );
//    mGlowShader->uniform( "spectrumTex", 2 );
//    mGlowShader->uniform( "color", mStar.mColor );
//    mGlowShader->uniform( "power", mRoom.getPower() );
//    mGlowShader->uniform( "starRadius", mStar.mRadiusDrawn );
//    Vec3f right = Vec3f::xAxis();
//    Vec3f up	= Vec3f::yAxis();
//    
//    
//    if( mBillboard ) mSpringCam.mCam.getBillboardVectors( &right, &up );
//    
//    //mController.drawGlows( &mGlowShader, right, up );
//    mGlowShader->unbind();
//}

void meshDeformApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) );
    
    if(!debug){

    
    gl::setViewport(getWindowBounds());
    gl::pushMatrices();
        gl::setMatrices( mCam );
        //gl::rotate( Vec3f(100, 0, 0) );
        
        mNoise.getTexture().bind();
        //mCellular.getTexture().bind();
        //mDefaultShader->bind();
        
        mExtrude->bind();
        
    
//        tLightPos = Vec3f( 5.0*cos(getElapsedSeconds()), 0.0, 5.0*sin(getElapsedSeconds()) );
//        mDefaultShader->uniform( "mLightPosition",	tLightPos );
//        mDefaultShader->uniform( "mAmbient",			ColorA( 0.1, 0.1, 0.1, 1.0 ) );
//        mDefaultShader->uniform( "mDiffuse",			ColorA( 0.2, 0.8, 0.4, 1.0 ) );
//        mDefaultShader->uniform( "mSpecular",         ColorA( 0.1, 0.4, 0.2, 1.0 ) );
//        mDefaultShader->uniform( "mShininess",		128.0f );
        
        
        
       mExtrude->uniform("noiseTex",0);
       mExtrude->uniform("scale", mScale);
        gl::rotate( Vec3f(50,84,0) );
        gl::rotate( Vec3f (Vec3f( cos( getElapsedSeconds() ), cos( getElapsedSeconds() ), sin( getElapsedSeconds() ) ) * 15.0)   );
      //  gl::rotate( Vec3f (( cos( getElapsedSeconds() ) * 15.0) + 80.0 ));
        
        gl::draw( mSphere );
        
        
        mExtrude->unbind();
        
  //    mDefaultShader->unbind();
        mNoise.getTexture().unbind();
  //    mCellular.getTexture().unbind();

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
