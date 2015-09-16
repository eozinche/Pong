//
//  SpaceDisplay.cpp
//  Throughline
//
//  Created by MOQN on 11/14/14.
//
//


#include "_main.h"
#include "space.h"




/////////////////////////////////////////////////////////////////////////// SetUp /////

void Space::displaySetup()
{
    
    for (int i=0; i<20; i++){
        createSphere(Vec3f( 0, 10, 0 ),
                     Vec3f( randFloat(-0.3, 0.3), randFloat(-0.3, 0.0), randFloat(-0.3, 0.3) ),
                     1);
        objects[0]->bouncing = true;
        objects[0]->colliding = true;
    }
    
    for (int i=0; i<20; i++){
        createCube(
                   Vec3f( 0, 10, 0 ),
                   Vec3f( randFloat(-0.3, 0.3), randFloat(-0.3, 0.0), randFloat(-0.3, 0.3) ),
                   1
                   );
        objects[0]->bouncing = true;
        objects[0]->colliding = true;
    }
    
    for (int i=0; i<10; i++){
        
        //createCube( 0,0,0 );
        //objects[0]->changeSize( randFloat()*10., randFloat()*10., randFloat()*10. );
        
        createCube(
                   randFloat(-spaceSize.x/2,spaceSize.x/2),
                   randFloat(-spaceSize.y/2,spaceSize.y/2),
                   randFloat(-spaceSize.z/2,spaceSize.z/2)
                   );
        objects[0]->changeSize( randFloat()*5., randFloat()*5., randFloat()*5. );
        
        objects[0]->axis = Vec3f( randFloat(), randFloat(), randFloat() ); // 0 ~ 1
        objects[0]->angle = randFloat()*360; // degree
        objects[0]->immobility = false;
    }
    
}



/////////////////////////////////////////////////////////////////////////// Update /////

void Space::displayUpdate()
{
    
    //update the whole objects
    for ( auto& obj : objects ) {
        Vec3f tGravity = -Vec3f(0, gravity * obj->mass, 0);
        obj->applyForce( tGravity );
        obj->applyForce( wind );
        obj->update( velReduction );
        if (obj->bouncing) {
            bool hit = obj->checkHitBoundary( spaceSize );
            if ( hit ) {
                // do somthing
                // call any function
            }
        }
    }
    
    
}






/////////////////////////////////////////////////////////////////////////// Draw /////

void Space::drawInNormal() {
    
    
    
    
    
    //////////     draw here     //////////
    
    
    
    

}





void Space::drawIntoShader( Vec3f tLightPos ) {
    
    for ( auto& obj : objects ){
        mShader.uniform( "mLightPosition",	Quatf( Vec3f( obj->axis ), - obj->angle) * tLightPos );
        obj->display();
    }
    
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








