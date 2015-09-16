//
//  Space.cpp
//  _PONG
//
//  Created by MOQN on 11/6/14.
//
//

#include "_main.h"
#include "Space.h"

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
    // get Sin and Cos arrays
    for (int i=0; i<360; i++){
        sinArray[i] = sin(toRadians(i*1.0));
        cosArray[i] = cos(toRadians(i*1.0));
    }
    
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
    
    setCamera();
    
    //timeline().step(1./getFrameRate());
}


void Space::update(){
    winSize = getWindowSize();
    width = getWindowWidth();
    height = getWindowHeight();
    frame = getElapsedFrames();

    setCamera();
    
    //update the whole objects
    updateEveryObject();
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
    
    // console out data
    if ( !(SAVE_IMAGE_ON) ) consoleOut();
}


void Space::display()
{
    gl::clear( Color( 0, 0, 0 ) );
    
    
    // draw the Bottom Area where we can't see
    if ( !(SAVE_IMAGE_ON) ) {
        if ( SHOW_GUIDELINE ) {
            // draw the bottom area
            gl::color( 1.0, 1.0, 1.0, 0.3 );
            gl::drawSolidRect( Rectf( Vec2f(0,getWindowHeight()*973/1080), Vec2f(width,getWindowHeight()) ) );
            
            if ( WINDOW_REDUCTION_WIDTH == 1 ){
                // COLUMN_A_L : 11520(3840*3) = x : getWindowWidth()
                // x = COLUMN_A_L * getWindowWidth() / 11520(3840*3)
                
                //drawCenter
                float tCenterX = ( COLUMN_CENTER_L * getWindowWidth() / 11520 - COLUMN_LEFT_R * getWindowWidth() / 11520 )/2 + COLUMN_LEFT_R * getWindowWidth() / 11520;
                gl::drawLine( Vec2f( tCenterX, 0), Vec2f( tCenterX, height) );
                //draw the columns
                gl::drawStrokedRect( Rectf(
                                           Vec2f( COLUMN_LEFT_L * getWindowWidth() / 11520, 0 ),
                                           Vec2f( COLUMN_LEFT_R * getWindowWidth() / 11520, getWindowHeight() )
                                           )
                                    );
                gl::drawStrokedRect( Rectf(
                                           Vec2f( COLUMN_CENTER_L * getWindowWidth() / 11520, 0 ),
                                           Vec2f( COLUMN_CENTER_R * getWindowWidth() / 11520, getWindowHeight() )
                                           )
                                    );
                gl::drawStrokedRect( Rectf(
                                           Vec2f( COLUMN_RIGHT_L * getWindowWidth() / 11520, 0 ),
                                           Vec2f( COLUMN_RIGHT_R * getWindowWidth() / 11520, getWindowHeight() )
                                           )
                                    );
            }
        }
    }
    
    
    ///////////////////
    drawInPixelSpace();
    ///////////////////
    
    
    
    gl::pushMatrices();
    gl::setMatrices( mCam );
    
    {
        // show guidlines for the 3D space
        if ( SHOW_GUIDELINE ){
            gl::color( Color(1,0,0) );
            gl::drawStrokedRect( Rectf (Vec2f(-spaceSize.x, -spaceSize.y),
                                        Vec2f( spaceSize.x,  spaceSize.y) ) );
            gl::drawSphere( Vec3f(SCREEN_CENTERPT_L_3D, 0, 0), 0.5 );
            gl::drawSphere( Vec3f(SCREEN_CENTERPT_C_3D, 0, 0), 0.5 );
            gl::drawSphere( Vec3f(SCREEN_CENTERPT_R_3D, 0, 0), 0.5 );
        }
        
        
        gl::color( 1.0, 1.0, 1.0 );
        
        ////////////////
        drawIn3DSpace();
        ////////////////
        
        
        // Set light position (in world-space):
        gLightPos = Vec3f( 5.0*cos(frame/50.0), 5.0, 5 + 2.0*sin(frame/50.0) );
        
        // Bind shader:
        mShader.bind();
        
        // Set shader uniform parameters:
        mShader.uniform( "mLightPosition",	gLightPos );
        mShader.uniform( "mAmbient",		ColorA( 0.2, 0.2, 0.2, 1.0 ) );
        mShader.uniform( "mShininess",		128.0f );
        mShader.uniform( "mDiffuse",		ColorA( 0.8, 0.8, 0.8, 1.0 ) );
        mShader.uniform( "mSpecular",		ColorA( 0.6, 0.4, 0.4, 1.0 ) );
        
        
        
        
        drawIn3DSpaceShader();
        
        
        
        
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
    objects.insert( objects.begin(), new ObjCube( Vec3f( x,y,z ), Vec3f::zero(), 1, "" ) );
}
void Space::createCube( Vec3f pos ) {
    objects.insert( objects.begin(), new ObjCube( pos, Vec3f::zero(), 1, "" ) );
}
void Space::createCube( Vec3f pos, string id ) {
    objects.insert( objects.begin(), new ObjCube( pos, Vec3f::zero(), 1, id ) );
}
void Space::createCube( Vec3f pos, Vec3f vel, float size ) {
    objects.insert( objects.begin(), new ObjCube( pos, vel, size, "" ) );
}
void Space::createCube( Vec3f pos, Vec3f vel, float size, string id ) {
    objects.insert( objects.begin(), new ObjCube( pos, vel, size, id ) );
}



void Space::createSphere( float x,float y, float z ) {
    objects.insert( objects.begin(), new ObjSphere( Vec3f( x,y,z ), Vec3f::zero(), 1, "" ) );
}
void Space::createSphere( Vec3f pos ) {
    objects.insert( objects.begin(), new ObjSphere( pos, Vec3f::zero(), 1, "" ) );
}
void Space::createSphere( Vec3f pos, string id ) {
    objects.insert( objects.begin(), new ObjSphere( pos, Vec3f::zero(), 1, id ) );
}
void Space::createSphere( Vec3f pos, Vec3f vel, float size ) {
    objects.insert( objects.begin(), new ObjSphere( pos, vel, size, "" ) );
}
void Space::createSphere( Vec3f pos, Vec3f vel, float size, string id ) {
    objects.insert( objects.begin(), new ObjSphere( pos, vel, size, id ) );
}


void Space::createObjMeshCube( Vec3f _pos, int _numU, int _numV, string _id ) {
    objMeshes.insert( objMeshes.begin(), ObjMesh( _pos, _numU, _numV, _id ) );
    for (int v=0; v<_numV*_numU; v++) {
        createCube( Vec3f::zero(), _id );
        objects[0]->immobility = true;
    }
}
void Space::createObjMeshSphere( Vec3f _pos, int _numU, int _numV, string _id ) {
    objMeshes.insert( objMeshes.begin(), ObjMesh( _pos, _numU, _numV, _id ) );
    for (int v=0; v<_numV*_numU; v++) {
        createSphere( Vec3f::zero(), _id );
        objects[0]->immobility = true;
    }
}

void Space::objMesh_changePosition( Vec3f _pos, string _id ){
    for ( auto& objMesh : objMeshes ) {
        if ( objMesh.id == _id) {
            objMesh.pos = _pos;
        }
    }
}
void Space::objMesh_rotate( Vec3f _axis, float _angle, string _id ){
    for ( auto& objMesh : objMeshes ) {
        if ( objMesh.id == _id) {
            objMesh.axis = _axis;
            objMesh.angle = _angle;
        }
    }
}
void Space::objMesh_changeUVSize( int _u, int _v, string _id ){
    for ( auto& objMesh : objMeshes ) {
        if ( objMesh.id == _id) {
            objMesh.numU = _u;
            objMesh.numV = _v;
        }
    }
}
void Space::objMesh_changeUnitSize( Vec3f _unitSize, string _id ){
    for ( auto& objMesh : objMeshes ) {
        if ( objMesh.id == _id) {
            objMesh.unitSize = _unitSize;
        }
    }
}
void Space::objMesh_changeGapSize( Vec2f _gap, string _id ){
    for ( auto& objMesh : objMeshes ) {
        if ( objMesh.id == _id) {
            objMesh.gap = _gap;
        }
    }
}

void Space::displayObjMesh( string _id ) {
    for ( auto& objMesh : objMeshes ) {
        int u = 0;
        int v = 0;
        gl::pushMatrices();
        gl::translate( objMesh.pos );
        gl::rotate( Quatf( objMesh.axis, objMesh.angle) );
        for ( auto& obj : objects ) {
            if ( obj->id == objMesh.id ){
                obj->size = objMesh.unitSize;
                obj->pos.x = (objMesh.unitSize.x + objMesh.gap.x) * (u-objMesh.numU/2);
                obj->pos.y = (objMesh.unitSize.y + objMesh.gap.y) * -(v-objMesh.numV/2);
                
                u++;
                if ( u == objMesh.numU ) {
                    u = 0;
                    v++;
                }
                mShader.uniform( "mLightPosition", Quatf( Vec3f( objMesh.axis ), -objMesh.angle) * gLightPos );
                obj->display();
            }
        }
        gl::popMatrices();
    }
}


void Space::updateEveryObject(){
    for ( auto& obj : objects ) {
        Vec3f tGravity = -Vec3f(0, gravity * obj->mass, 0);
        obj->applyForce( tGravity );
        obj->applyForce( wind );
        obj->update( velReduction );
        if (obj->bouncing) {
            obj->checkHittingBoundary( spaceSize );
        }
    }
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
                    objects[i]->stateCollision = true;
                } else objects[i]->stateCollision = false;
            }
        }
    }
}


void Space::setCamera() {
    //float zDist = 900 / camPersp;
    mCam.setPerspective( camPersp, getWindowAspectRatio(), 1, 1200 );
    mCam.lookAt( Vec3f( 0.0, 0.0, camZdist ), Vec3f::zero() );
    mCam.setLensShift( 0.118, 0);
}
void Space::setCamera( float perspective, float zDistance ) {
    mCam.setPerspective( perspective, getWindowAspectRatio(), 1, 1200 );
    mCam.lookAt( Vec3f( 0.0, 0.0, zDistance ), Vec3f::zero() );
    mCam.setLensShift( 0.118, 0);
}


void Space::saveImage(){
    // pull down the current window as a surface and pass it to writeImage
    writeImage( getHomeDirectory() / "_cinder" / "! savedImages" / ( toString( getElapsedFrames() ) + ".png" ), copyWindowSurface() );
    cout << "Saved Frame: " << getElapsedFrames() << endl;
}


bool Space::sequence( int frameNum ) {
    if ( getElapsedFrames() == frameNum ) return true;
    return false;
}

bool Space::sequence( int from, int until ) {
    // please note that the last frame is not included.
    if ( getElapsedFrames() >= from && getElapsedFrames() < until ) return true;
    return false;
}

void Space::toTest() {
    cout << "Working!" << endl;
}

void Space::consoleOut() {
    stringstream s;
    s << endl << endl;
//    s << noise << endl;
    s << "Objects: " << objects.size() << "  /  ObjMesh: " << objMeshes.size() << endl;
    s << "Frame: " << frame << "  /  Second: " << floor(frame/30*100)/100 << endl;
    cout << s.str() << endl;        // .str() to change the stringstream to a string
}


/////////////////////////////////////////////////////////////////////////// GUI /////

void Space::setGUIs() {
    GUI = params::InterfaceGl::create( "Parameters" , Vec2f( 200,300 ), ColorA( 0,0,0,.3 ) );
    GUI->setPosition( Vec2f( 10, 10 ) );
    
    GUI->addSeparator();
    GUI->addText( "Press 'R' to reset" );
    
    GUI->addSeparator();
    GUI->addParam("Gravity", &gravity,
                  "min=-0.05 max=0.05 step=0.001" );
    GUI->addParam("Wind", &wind);
    GUI->addParam("VelReduction", &velReduction,
                  "min=0.975 max=1.000 step=0.001" );
    
    GUI->addSeparator();
    GUI->addParam("Perspective", &camPersp,
                  "min=1.0 max=60.0 step=1.0" );
    GUI->addSeparator();
}











