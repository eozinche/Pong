//
//  _main.h
//  Throughline
//
//  Created by MOQN on 11/14/14.
//
//

#pragma once


#define SAVE_IMAGE_ON           0

#define WINDOW_WIDTH_REDUCTION  3
#define WINDOW_SIZE_REDUCTION   3

#define WINDOW_WIDTH            ( 3840 *3   /WINDOW_WIDTH_REDUCTION /WINDOW_SIZE_REDUCTION)
#define WINDOW_HEIGHT           ( 1080      /WINDOW_SIZE_REDUCTION )         //973


#define CAMERA_PERSPECTIVE      10
#define SPACE_SIZE              106/WINDOW_WIDTH_REDUCTION, 10, 10        // x, y, z
#define GRAVITY_INIT            0.002
#define VELOCITY_REDUCTION      0.999
#define COLLISION_IMPACT        0.0002






#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/GlslProg.h"
#include "cinder/gl/Texture.h"
#include "cinder/Camera.h"
#include "cinder/Rand.h"
#include "cinder/params/Params.h"

#include "cinder/ImageIo.h"
#include "cinder/Utilities.h"

#include <sstream>

using namespace ci;
using namespace ci::app;
using namespace std;