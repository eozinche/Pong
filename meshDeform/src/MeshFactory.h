#pragma once

#include <algorithm>
#include <vector>

#include "cinder/gl/gl.h"
#include "cinder/gl/Vbo.h"

struct ProtoMesh
{
		struct Vertex
	{
		ci::Vec3f mPosition;
		ci::Vec3f mNormal;
		ci::Vec2f mUV;
		
		
		Vertex() {}
		
		Vertex(const ci::Vec2f& iUv) : mUV( iUv ) {}
	};

	
	void drawDebug(const float& iNormalLength)
	{
		glPointSize( 5.0 );
		glLineWidth( 2.0 );
		glColor3f( 0.0, 0.0, 1.0 );
        
		glBegin(GL_LINES);
		for(std::vector<Vertex>::const_iterator it = mVertices.cbegin(); it != mVertices.cend(); it++) {
			glVertex3f((*it).mPosition.x,
					   (*it).mPosition.y,
					   (*it).mPosition.z);
			glVertex3f((*it).mPosition.x + (*it).mNormal.x * iNormalLength,
					   (*it).mPosition.y + (*it).mNormal.y * iNormalLength,
					   (*it).mPosition.z + (*it).mNormal.z * iNormalLength);
		}
		glEnd();
	}
	
	std::vector<uint32_t>	mIndices;
    std::vector<Vertex>		mVertices;
};


static void initializeGenericMesh(const uint32_t& iDimensionU, const uint32_t& iDimensionV, ProtoMesh& oMesh)
{
	
	uint32_t tDimU = std::max<uint32_t>( iDimensionU, 2 );
	uint32_t tDimV = std::max<uint32_t>( iDimensionV, 2 );
	
	
	for(uint32_t v = 0; v < tDimV; v++) {
		for(uint32_t u = 0; u < tDimU; u++) {
			oMesh.mVertices.push_back( ProtoMesh::Vertex( ci::Vec2f( static_cast<float>( u ) / (tDimU - 1), static_cast<float>( v ) / (tDimV - 1 ) ) ) );
		}
	}
	
	
	for(uint32_t v = 0; v < tDimV - 1; v++) {
		
		bool goingRight = (v % 2 == 0);
		
		uint32_t firstCol = 0;
		uint32_t lastCol  = tDimU - 1;
    
		uint32_t u = ( goingRight ) ? ( firstCol ) : ( lastCol );
		
		bool rowComplete = false;
		while( !rowComplete ) {
			
            uint32_t iA, iB, iC, iD;
			if( goingRight ) {
                iA = (v) * (tDimU) + (u);
				iB = (v + 1) * (tDimU) + (u);
				iC = (v) * (tDimU) + (u + 1);
				iD = (v + 1) * (tDimU) + (u + 1);
			}
			else {
                iA = (v) * (tDimU) + (u);
				iB = (v + 1) * (tDimU) + (u);
				iC = (v) * (tDimU) + (u - 1);
				iD = (v + 1) * (tDimU) + (u - 1);
			}
			
			oMesh.mIndices.push_back( iA );
			oMesh.mIndices.push_back( iB );
			oMesh.mIndices.push_back( iC );
			oMesh.mIndices.push_back( iD );
			
			if( goingRight ) {
				u++;
			}
			else {
				u--;
			}
			
			if( ( goingRight && u == lastCol ) || ( !goingRight && u == firstCol ) ) {
                oMesh.mIndices.push_back( iD );
				oMesh.mIndices.push_back( iD );
				rowComplete = true;
			}
		}
	}
}

static void createSphere(const uint32_t& iDimensionU, const uint32_t& iDimensionV, const float& iRadius, ProtoMesh& oMesh)
{
	initializeGenericMesh( iDimensionU, iDimensionV, oMesh );
	
	for(std::vector<ProtoMesh::Vertex>::iterator it = oMesh.mVertices.begin(); it != oMesh.mVertices.end(); it++) {
		float thetaU  = M_PI * 2.0 * (*it).mUV.x;
		float thetaV  = (M_PI * (*it).mUV.y) - (M_PI / 2.0);
		
		float x = iRadius * cos(thetaV) * cos(thetaU);
		float y = iRadius * cos(thetaV) * sin(thetaU);
		float z = iRadius * sin(thetaV);
		
		(*it).mPosition = ci::Vec3f( x, y, z );
        (*it).mNormal = ci::Vec3f( x, y, z ).normalized();
	}
}

static void updateMeshVbo(ProtoMesh& iMesh, ci::gl::VboMesh& oMeshVbo)
{
	ci::gl::VboMesh::VertexIter itvbo = oMeshVbo.mapVertexBuffer();
	for(std::vector<ProtoMesh::Vertex>::const_iterator itv = iMesh.mVertices.cbegin(); itv != iMesh.mVertices.cend(); itv++) {
		itvbo.setPosition( (*itv).mPosition );
		itvbo.setNormal( (*itv).mNormal );
		itvbo.setTexCoord2d0( (*itv).mUV );
		++itvbo;
	}
	oMeshVbo.bufferIndices( iMesh.mIndices );
}

static void createMeshVbo(ProtoMesh& iMesh, ci::gl::VboMesh& oMeshVbo)
{
    ci::gl::VboMesh::Layout tLayout;
	tLayout.setStaticIndices();
	tLayout.setDynamicPositions();
	tLayout.setDynamicNormals();
	tLayout.setDynamicTexCoords2d();
	
	oMeshVbo = ci::gl::VboMesh( iMesh.mVertices.size(), iMesh.mIndices.size(), tLayout, GL_TRIANGLE_STRIP );

	updateMeshVbo( iMesh, oMeshVbo );
}
