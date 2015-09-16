//
//  _main.h
//  _PONG
//
//  Created by MOQN on 11/14/14.
//
//




//////////////////////////////////////////////////////////////////////////////// TO DO LIST
/*
Sequencing
 Timeline + Animation

SimpleOBJ
*/



#pragma once


#define SAVE_IMAGE_ON           1
#define SHOW_GUIDELINE          0
#define SHOW_PARAMETERS         0

#define WINDOW_REDUCTION_WIDTH  1
#define WINDOW_REDUCTION_SIZE   1

#define WINDOW_WIDTH            ( 3840 *3   /WINDOW_REDUCTION_WIDTH /WINDOW_REDUCTION_SIZE)
#define WINDOW_HEIGHT           ( 1080      /WINDOW_REDUCTION_SIZE )         //973


// 3D Physics
#define CAMERA_PERSPECTIVE      10
#define SPACE_SIZE              106/WINDOW_REDUCTION_WIDTH, 10, 10        // x, y, z

#define GRAVITY_INIT            0.02
#define VELOCITY_REDUCTION      0.999
#define COLLISION_IMPACT        0.0002

#define OBJECT_SIZE_MIN         0.05


// Scene Pong2D
#define PADDLE_WIDTH            1.0
#define PADDLE_HEIGHT           5.5

#define BALLSPEED_CONSTRAIN     1.2




#define LENSSHIFT_ADJ           12.5

// Column Information
#define COLUMN_LEFT_L           3376
#define COLUMN_LEFT_R           3736
#define COLUMN_CENTER_L         6401
#define COLUMN_CENTER_R         6815
#define COLUMN_RIGHT_L          9788
#define COLUMN_RIGHT_R          10087

#define COLUMN_LEFT_L_3D        (-43.8722)  + LENSSHIFT_ADJ            // (3376 * 106*2 / 11520 - 106)
#define COLUMN_LEFT_R_3D        (-37.2472)  + LENSSHIFT_ADJ           // (3736 * 106*2 / 11520 - 106)
#define COLUMN_CENTER_L_3D      11.7961     + LENSSHIFT_ADJ              // (6401 * 106*2 / 11520 - 106)
#define COLUMN_CENTER_R_3D      19.4149     + LENSSHIFT_ADJ              // (6815 * 106*2 / 11520 - 106)
#define COLUMN_RIGHT_L_3D       74.1263     + LENSSHIFT_ADJ              // (9788  * 106*2 / 11520 - 106)
#define COLUMN_RIGHT_R_3D       729.6288    + LENSSHIFT_ADJ              // (10087 * 106*2 / 11520 - 106)

#define SCREEN_CENTERPT_L       1688                 // ( COLUMN_LEFT_L/2 )
#define SCREEN_CENTERPT_C       5068                 // ( COLUMN_LEFT_R + (COLUMN_CENTER_L-COLUMN_LEFT_R)/2 )
#define SCREEN_CENTERPT_R       8302                 // ( COLUMN_CENTER_R + (COLUMN_RIGHT_L-COLUMN_CENTER_R)/2)

#define SCREEN_CENTERPT_L_3D    (-71.9361)  + LENSSHIFT_ADJ           // ( (-100-43.8722)/2 )
#define SCREEN_CENTERPT_C_3D    (-12.7255)  + LENSSHIFT_ADJ           // ( -37.2472 + (11.7961+37.2472)/2 )
#define SCREEN_CENTERPT_R_3D    46.7708     + LENSSHIFT_ADJ              // ( 19.4149 + (74.1263-19.4149)/2 )
#define SCREEN_CENTERPT_RR_3D   92.8144     + LENSSHIFT_ADJ              // (106 - 79.6288)/2 + 79.6288

#define SCREEN_BOTTOM_AREA      2              // ( 19.4149 + (74.1263-19.4149)/2 )

// 3376 : 11520(3840*3) = x : 106
// x = 3376 * 106 / 11520



#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/GlslProg.h"
#include "cinder/gl/Texture.h"
#include "cinder/Camera.h"
#include "cinder/Rand.h"
#include "cinder/Timeline.h"
#include "cinder/params/Params.h"
#include "cinder/MayaCamUI.h"
#include "cinder/Perlin.h"
#include "cinder/ImageIo.h"
#include "cinder/Utilities.h"

#include <sstream>

using namespace ci;
using namespace ci::app;
using namespace std;




