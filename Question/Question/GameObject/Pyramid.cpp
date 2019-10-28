#include "Pyramid.h"
#include "GameObject.h"
#include "../Component/Renderer.h"
#include "../Component/Transform.h"


CPyramid::CPyramid()
{
}

CPyramid::CPyramid( const CPyramid & pPyramid )
{
}


CPyramid::~CPyramid()
{
}

bool CPyramid::Init()
{
	CRenderer* pRenderer = m_pGameObject->AddComponent<CRenderer>( "PyramidRenderer" );
	pRenderer->SetMesh( "Pyramid" );
	pRenderer->SetShader( STANDARD_COLOR_NORMAL_SHADER );
	pRenderer->SetInputLayout( "VertexColorNormal" );

	pRenderer->SetRenderState( ALPHA_BLEND );
	pRenderer->SetRenderState( DEPTH_DISABLE );
	pRenderer->SetRenderState( CULLING_DEFAULT );
	SAFE_RELEASE( pRenderer );

	m_pTransform->SetWorldScale( 10.f, 10.f, 10.f );

	return true;
}

void CPyramid::Input( float fTime )
{
}

int CPyramid::Update( float fTime )
{
	return 0;
}

int CPyramid::LateUpdate( float fTime )
{
	return 0;
}

void CPyramid::Collision( float fTime )
{
}

void CPyramid::Render( float fTime )
{
}

CPyramid * CPyramid::Clone()
{
	return new CPyramid(*this);
}
