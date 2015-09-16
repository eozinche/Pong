//
//  _SceneCubeMesh.cpp
//  Pong
//
//
//

#include "_main.h"
#include "Space.h"


void Space::sceneCubemesh_createCubemesh() {
    
    createObjMeshCube( Vec3f(0,0,0), 120, 60, "CubeMesh" );
    objMesh_changeUnitSize( Vec3f( 0.8,0.8,0.8 ) , "CubeMesh" );
    objMesh_changeGapSize( Vec2f( 3.2,0.8 ), "CubeMesh" );

}


void Space::sceneCubemesh_runCubemesh() {
    
    objMesh_rotate( Vec3f( 1,0,0 ), -1.0+frame/360, "CubeMesh" );
    //objMesh_rotate( Vec3f( 0,1,0 ), frame*0.05, "CubeMesh" ); //
    displayObjMesh( 1.0, "CubeMesh" );

}

void Space::sceneCubemesh_changeCamera() {
    camZdist *= 0.992;
    for ( auto& objMesh : objMeshes ) {
        objMesh.gap.x *= 0.997;
        if (frame <180) objMesh.unitSize *= 1.01;
        else objMesh.unitSize *= 0.97;
    }
}


void Space::displayObjMesh( float z, string _id ) {
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
                // add curve
                obj->pos.z = ( sinArray[u*180/objMesh.numU] + sinArray[v*180/objMesh.numV] ) *-90 + 90;
                // add wave
                obj->pos.z += cos((u*6+frame*0.1))*0.3 + sin((v*6+frame*0.2))*0.6;;
                
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




