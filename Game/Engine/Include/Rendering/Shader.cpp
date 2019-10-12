#include "Shader.h"
#include "../Core/PathManager.h"
#include "../Device.h"

Engine_USING

CShader::CShader():
	m_pVSBlob( NULL ),
	m_pPSBlob( NULL ),
	m_pGSBlob( NULL )
{
	SetTag( "Shader" );
	SetTypeName( "CShader" );
	SetTypeID<CShader>();
}


CShader::~CShader()
{
	SAFE_RELEASE( m_pVSBlob );
	SAFE_RELEASE( m_pPSBlob );
	SAFE_RELEASE( m_pGSBlob );
}

string CShader::GetKey() const
{
	return m_strKey;
}

void * CShader::GetShaderByteCode()
{
	return m_pVSBlob->GetBufferPointer();
}

int CShader::GetShaderByteCodeLength()
{
	return m_pVSBlob->GetBufferSize();
}

bool CShader::LoadShader( const string & strKey, TCHAR * pFileName, char * pEntry[ST_MAX], const string & strPathKey )
{
	return false;
}

bool CShader::LoadVertexShader( const string & strKey, TCHAR * pFileName, char * pEntry, const string & strPathKey )
{
	UINT	iFlag = 0;

#ifdef _DEBUG
	iFlag = D3DCOMPILE_DEBUG;
#endif // _DEBUG

	const wchar_t* pPath = GET_SINGLE( CPathManager )->FindPath( strPathKey );
	wstring	strFile;

	if ( pPath )
		strFile = pPath;

	strFile += pFileName;

	char*	pTarget = "vs_5_0";
	ID3DBlob*	pErr = NULL;

	if ( FAILED( D3DCompileFromFile( strFile.c_str(), NULL,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, pEntry, pTarget, iFlag,
		0, &m_pVSBlob, &pErr ) ) )
		return false;

	m_tVSByteCode.BytecodeLength = m_pVSBlob->GetBufferSize();
	m_tVSByteCode.pShaderBytecode = m_pVSBlob->GetBufferPointer();
	
	return true;
}

bool CShader::LoadPixelShader( const string & strKey, TCHAR * pFileName, char * pEntry, const string & strPathKey )
{
	return false;
}

bool CShader::LoadGeometryShader( const string & strKey, TCHAR * pFileName, char * pEntry, const string & strPathKey )
{
	return false;
}

void CShader::SetShader()
{
}
