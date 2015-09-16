//
//  SceneDot.cpp
//  _PONG
//
//
//

#include "_main.h"
#include "Space.h"


void Space::scenePong2D_createBalls() {
    createSphere(Vec3f(SCREEN_CENTERPT_C_3D,0,0), "sphere|center");
    {
        objects[0]->vel = Vec3f( 0.2,randFloat(-0.5,0.5),0 );
    }
    
    createSphere(Vec3f(SCREEN_CENTERPT_L_3D,0,0), "sphere|left");
    {
        objects[0]->vel = Vec3f( -0.4,randFloat(-0.5,0.5),0 );
    }
    
    createSphere(Vec3f(SCREEN_CENTERPT_R_3D,0,0), "sphere|right");
    {
        objects[0]->vel = Vec3f( 0.3, randFloat(-0.5,0.5), 0 );
    }
    for ( auto& obj : objects ) {
        obj->immobility = true;
        obj->colliding = true;
        obj->size = Vec3f( 0.1, 0.1, 0.1 );
        obj->sizeVel = Vec3f( 0.005, 0.005, 0.005 );
    }
}

void Space::scenePong2D_runBalls() {
    
    for ( auto& obj : objects ) {
        // update properties
        int ballStartFrame = 300;

        if ( obj->id == "sphere|left" ){
            if ( frame < ballStartFrame ) obj->size += obj->sizeVel*0.5;
            else {
                if ( obj->size.x < 3 || obj->size.x > 4) obj->sizeVel *= -1;
                obj->size += obj->sizeVel;
            }
            // limit the balls in the boundaries
            if (obj->pos.x-obj->size.x < -spaceSize.x) {
                obj->pos.x = -spaceSize.x + obj->size.x*1.1;
                obj->vel.x *= -1.01;
            } else if ( obj->pos.x+obj->size.x > COLUMN_LEFT_L_3D ){
                obj->pos.x = COLUMN_LEFT_L_3D - obj->size.x*1.1;
                obj->vel.x *= -1.01;
            }
            // draw the balls
            obj->display();
        }
        else if (obj->id == "sphere|center"){
            if ( frame < ballStartFrame ) obj->size += obj->sizeVel;
            else {
                if ( obj->size.x < 4 || obj->size.x > 5) obj->sizeVel *= -1;
                obj->size += obj->sizeVel;
            }
            // limit the balls in the boundaries
            if (obj->pos.x-obj->size.x < COLUMN_LEFT_R_3D) {
                obj->pos.x = COLUMN_LEFT_R_3D + obj->size.x*1.1;
                obj->vel.x *= -1.01;
            } else if ( obj->pos.x+obj->size.x > COLUMN_CENTER_L_3D ){
                obj->pos.x = COLUMN_CENTER_L_3D - obj->size.x*1.1;
                obj->vel.x *= -1.01;
            }
            // draw the balls
            obj->display();
        }
        else if ( obj->id == "sphere|right" ){
            if ( frame < ballStartFrame ) obj->size += obj->sizeVel*0.5;
            else {
                if ( obj->size.x < 3 || obj->size.x > 4) obj->sizeVel *= -1;
                obj->size += obj->sizeVel;
            }
            // limit the balls in the boundaries
            if (obj->pos.x-obj->size.x < COLUMN_CENTER_R_3D) {
                obj->pos.x = COLUMN_CENTER_R_3D + obj->size.x*1.1;
                obj->vel.x *= -1.01;
            } else if ( obj->pos.x+obj->size.x > spaceSize.x ){
                obj->pos.x = spaceSize.x - obj->size.x*1.1;
                obj->vel.x *= -1.01;
            }
            // draw the balls
            obj->display();
        }
        
        // check the collision to paddles and the space boundary
        if ( obj->getId( "sphere" ) && frame > ballStartFrame ) {
            
            // apply the velocity
            obj->pos += obj->vel;
            
            // limit the balls in the boundaries
            if (obj->pos.y-obj->size.y < -spaceSize.y + SCREEN_BOTTOM_AREA ){
                obj->pos.y = -spaceSize.y + SCREEN_BOTTOM_AREA + obj->size.y*1.1;
                obj->vel.y *= -1.01;
            }
            else if ( obj->pos.y+obj->size.y > spaceSize.y ){
                obj->pos.y = spaceSize.y - obj->size.y*1.1;
                obj->vel.y *= -1.01;
            }
            
            // Constrain the velocity
            if (obj->vel.x > BALLSPEED_CONSTRAIN) obj->vel.x = BALLSPEED_CONSTRAIN;
            else if (obj->vel.x < -BALLSPEED_CONSTRAIN) obj->vel.x = -BALLSPEED_CONSTRAIN;
            if (obj->vel.x > BALLSPEED_CONSTRAIN) obj->vel.x = BALLSPEED_CONSTRAIN;
            else if (obj->vel.x < -BALLSPEED_CONSTRAIN) obj->vel.x = -BALLSPEED_CONSTRAIN;
            
            if (obj->vel.y > BALLSPEED_CONSTRAIN) obj->vel.y = BALLSPEED_CONSTRAIN;
            else if (obj->vel.y < -BALLSPEED_CONSTRAIN) obj->vel.y = -BALLSPEED_CONSTRAIN;
            if (obj->vel.y > BALLSPEED_CONSTRAIN) obj->vel.y = BALLSPEED_CONSTRAIN;
            else if (obj->vel.y < -BALLSPEED_CONSTRAIN) obj->vel.y = -BALLSPEED_CONSTRAIN;
            
            // check collision to paddles
            for ( auto& pdl : objects ) {
                if ( pdl->id == "paddle" ){
                    if (abs(obj->pos.x-pdl->pos.x) < obj->size.x+pdl->size.x ){
                        if (obj->pos.y < pdl->pos.y+pdl->size.y*0.45 &&
                            obj->pos.y > pdl->pos.y-pdl->size.y*0.45) {
                            // change the position first
                            if ( obj->pos.x < pdl->pos.x ) obj->pos.x = pdl->pos.x - pdl->size.x - obj->size.x*1.1;
                            else obj->pos.x = pdl->pos.x + pdl->size.x + obj->size.x*1.1;
                            // switch the velocity
                            obj->vel.x *= -1.1;
                            obj->vel.y += (obj->pos.y - pdl->pos.y)*0.2;
                        }
                    }
                }
            }
            // the end of the collision test
        }
        // the end of checking the collision to paddles and the space boundary
    }
}


void Space::scenePong2D_createPaddles() {
    
    // Left
    createCube( Vec3f( -spaceSize.x + 1.0, 0, 0 ), "paddle|left" );
    createCube( Vec3f( COLUMN_LEFT_L_3D - 1.0, 0, 0 ), "paddle|left" );
    // Center
    createCube( Vec3f( COLUMN_LEFT_R_3D + 1.0, 0, 0 ), "paddle|center" );
    createCube( Vec3f( COLUMN_CENTER_L_3D - 1.0, 0, 0 ), "paddle|center" );
    // Right
    createCube( Vec3f( COLUMN_CENTER_R_3D + 1.0, 0, 0 ), "paddle|right" );
    createCube( Vec3f( COLUMN_RIGHT_L_3D - 1.0, 0, 0 ), "paddle|right" );
    // The rest
    createCube( Vec3f( spaceSize.x - 1.0, 0, 0 ), "paddle|rest" );
    
    
    
    for ( auto& obj : objects ) {
        if ( obj->getId( "paddle" ) ){
            obj->immobility = true;
            obj->size = Vec3f( PADDLE_WIDTH, PADDLE_HEIGHT, 1 );
            obj->vel = Vec3f( 0, randFloat(-0.5,0.5), 0 );
        }
    }
    
}

void Space::scenePong2D_createMorePaddles() {
    float gap = 5;
    for ( int i=1; i<4; i++ ){
        // Left
        createCube( Vec3f( -spaceSize.x + 1.0 + i*gap, 0, 0 ), "paddle|left" );
        objects[0]->immobility = true;
        objects[0]->size = Vec3f( PADDLE_WIDTH, PADDLE_HEIGHT+randFloat(-3,3), 1 );
        objects[0]->vel = Vec3f( 0, randFloat(-0.5,0.5), 0 );
        
        createCube( Vec3f( COLUMN_LEFT_L_3D - 1.0 - i*gap, 0, 0 ), "paddle|left" );
        objects[0]->immobility = true;
        objects[0]->size = Vec3f( PADDLE_WIDTH, PADDLE_HEIGHT+randFloat(-3,3), 1 );
        objects[0]->vel = Vec3f( 0, randFloat(-0.5,0.5), 0 );
        
        // Center
        createCube( Vec3f( COLUMN_LEFT_R_3D + 1.0 + i*gap, 0, 0 ), "paddle|center" );
        objects[0]->immobility = true;
        objects[0]->size = Vec3f( PADDLE_WIDTH, PADDLE_HEIGHT+randFloat(-3,3), 1 );
        objects[0]->vel = Vec3f( 0, randFloat(-0.5,0.5), 0 );
        
        createCube( Vec3f( COLUMN_CENTER_L_3D - 1.0 - i*gap, 0, 0 ), "paddle|center" );
        objects[0]->immobility = true;
        objects[0]->size = Vec3f( PADDLE_WIDTH, PADDLE_HEIGHT+randFloat(-3,3), 1 );
        objects[0]->vel = Vec3f( 0, randFloat(-0.5,0.5), 0 );
        
        // Right
        createCube( Vec3f( COLUMN_CENTER_R_3D + 1.0 + i*gap, 0, 0 ), "paddle|right" );
        objects[0]->immobility = true;
        objects[0]->size = Vec3f( PADDLE_WIDTH, PADDLE_HEIGHT+randFloat(-3,3), 1 );
        objects[0]->vel = Vec3f( 0, randFloat(-0.5,0.5), 0 );
        
        createCube( Vec3f( COLUMN_RIGHT_L_3D - 1.0 - i*gap, 0, 0 ), "paddle|right" );
        objects[0]->immobility = true;
        objects[0]->size = Vec3f( PADDLE_WIDTH, PADDLE_HEIGHT+randFloat(-3,3), 1 );
        objects[0]->vel = Vec3f( 0, randFloat(-0.5,0.5), 0 );
        
        // The rest
        createCube( Vec3f( spaceSize.x - 1.0 - i*gap, 0, 0 ), "paddle|rest" );
        objects[0]->immobility = true;
        objects[0]->size = Vec3f( PADDLE_WIDTH, PADDLE_HEIGHT+randFloat(-3,3), 1 );
        objects[0]->vel = Vec3f( 0, randFloat(-0.5,0.5), 0 );
    }
    
}


void Space::scenePong2D_runPaddles() {
    
    for ( auto& obj : objects ) {
        if (obj->getId( "paddle" )){
            // update properties
            obj->pos += obj->vel;
            
            // limit the balls in the boundaries
            if (obj->pos.y-obj->size.y/2 < -spaceSize.y + SCREEN_BOTTOM_AREA ||
                obj->pos.y+obj->size.y/2 > spaceSize.y ){
                obj->vel.y *= -1;
            }
            obj->display();
        }
    }
    
}


void Space::scenePong2D_arrangePaddles() {
    
    float tSpeed = 0.2;
    float tSize = 10;
    float tGap = tSpeed + 0.05;
    
    for ( auto& obj : objects ) {
        if (obj->id == ( "paddle|left" )){
            if (obj->pos.x < SCREEN_CENTERPT_L_3D-tGap) obj->pos.x += tSpeed;
            else if (obj->pos.x > SCREEN_CENTERPT_L_3D+tGap) obj->pos.x -= tSpeed;
            else obj->pos.x = SCREEN_CENTERPT_L_3D;
        } else if (obj->id == ( "paddle|center" )){
            if (obj->pos.x < SCREEN_CENTERPT_C_3D-tGap) obj->pos.x += tSpeed;
            else if (obj->pos.x > SCREEN_CENTERPT_C_3D+tGap) obj->pos.x -= tSpeed;
            else obj->pos.x = SCREEN_CENTERPT_C_3D;
        } else if (obj->id == ( "paddle|right" )){
            if (obj->pos.x < SCREEN_CENTERPT_R_3D-tGap) obj->pos.x += tSpeed;
            else if (obj->pos.x > SCREEN_CENTERPT_R_3D+tGap) obj->pos.x -= tSpeed;
            else obj->pos.x = SCREEN_CENTERPT_R_3D;
        } else if (obj->id == ( "paddle|rest" )){
            if (obj->pos.x < SCREEN_CENTERPT_RR_3D-tGap) obj->pos.x += tSpeed;
            else if (obj->pos.x > SCREEN_CENTERPT_RR_3D+tGap) obj->pos.x -= tSpeed;
            else obj->pos.x = SCREEN_CENTERPT_RR_3D;
        }
        if ( obj->getId("paddle") ) {
            if (obj->pos.y < 0-tGap) obj->pos.y += tSpeed;
            else if (obj->pos.y > 0+tGap) obj->pos.y -= tSpeed;
            else obj->pos.y = 0;
            
            if (obj->size.y < tSize-tGap) obj->size.y += tSpeed;
            else if (obj->size.y > tSize+tGap) obj->size.y -= tSpeed;
            else obj->size.y = tSize;
            
            obj->display();
        }
    }
    
}

void Space::scenePong2D_changeCameraToPerspective(){
    if ( camPersp < 50 ) {
        camPersp *= 1.04;
        camZdist = 1140/camPersp;
    }
    else camPersp = 50;
}







