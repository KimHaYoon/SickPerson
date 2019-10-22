#include "Shader.h"
#include "../Core/PathManager.h"
#include "../Device.h"
#include "../RootSignature.h"

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

D3D12_SHADER_BYTECODE CShader::GetVertexShader() const
{
	return m_tVSByteCode;
}

D3D12_SHADER_BYTECODE CShader::GetPixelShader() const
{
	return m_tPSByteCode;
}

D3D12_SHADER_BYTECODE CShader::GetGeoShader() const
{
	return m_tGSByteCode;
}

bool CShader::LoadShader( const string & strKey, wchar_t* pFileName, char * pEntry[ST_MAX], const string & strPathKey )
{
	m_strKey = strKey;

	if ( !LoadVertexShader( strKey, pFileName, pEntry[ST_VERTEX], strPathKey ) )
		return false;

	if ( !LoadPixelShader( strKey, pFileName, pEntry[ST_PIXEL], strPathKey ) )
		return false;

	if ( pEntry[ST_GEOMETRY] )
	{
		if ( !LoadGeometryShader( strKey, pFileName, pEntry[ST_GEOMETRY],	strPathKey ) )
			return false;
	}

	return true;
}

bool CShader::LoadVertexShader( const string & strKey, wchar_t* pFileName, char * pEntry, const string & strPathKey )
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

bool CShader::LoadPixelShader( const string & strKey, wchar_t * pFileName, char * pEntry, const string & strPathKey )
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

	char*	pTarget = "ps_5_0";
	ID3DBlob*	pErr = NULL;

	if ( FAILED( D3DCompileFromFile( strFile.c_str(), NULL,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, pEntry, pTarget, iFlag,
		0, &m_pPSBlob, &pErr ) ) )
		return false;

	m_tPSByteCode.BytecodeLength = m_pPSBlob->GetBufferSize();
	m_tPSByteCode.pShaderBytecode = m_pPSBlob->GetBufferPointer();

	return true;
}

bool CShader::LoadGeometryShader( const string & strKey, wchar_t * pFileName, char * pEntry, const string & strPathKey )
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

	char*	pTarget = "gs_5_0";
	ID3DBlob*	pErr = NULL;

	if ( FAILED( D3DCompileFromFile( strFile.c_str(), NULL,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, pEntry, pTarget, iFlag,
		0, &m_pGSBlob, &pErr ) ) )
		return false;

	m_tGSByteCode.BytecodeLength = m_pGSBlob->GetBufferSize();
	m_tGSByteCode.pShaderBytecode = m_pGSBlob->GetBufferPointer();

	return true;
}

void CShader::SetShader( D3D12_GRAPHICS_PIPELINE_STATE_DESC * pDesc )
{
	pDesc->VS = m_tVSByteCode;
	pDesc->PS = m_tPSByteCode;
	pDesc->GS = m_tGSByteCode;
}
