#include "Texture.h"
#include "../Device.h"
#include "../Core/PathManager.h"

GAME_USING

CTexture::CTexture() :
	m_pSRView( NULL ),
	m_bArray( false )
{
}

CTexture::~CTexture()
{
	Safe_Delete_VecList( m_vecImage );
	SAFE_RELEASE( m_pSRView );
}

bool CTexture::LoadTexture( const string & strKey, const wchar_t * pFileName,
	const string & strPathKey )
{
	m_strKey = strKey;
	char	strPath[MAX_PATH] = {};

	WideCharToMultiByte( CP_ACP, 0, pFileName, -1, strPath,
		lstrlen( pFileName ), 0, 0 );

	return LoadTexture( strKey, strPath, strPathKey );
}

bool CTexture::LoadTexture( const string & strKey,
	const char * pFileName, const string & strPathKey )
{
	m_strKey = strKey;
	const char*	pPath = GET_SINGLE( CPathManager )->FindPathToMultiByte( strPathKey );

	string	strPath;

	if ( pPath )
		strPath = pPath;

	strPath += pFileName;

	return LoadTextureFromFullPath( strKey, strPath.c_str() );
}

bool CTexture::LoadTexture( const string & strKey, const vector<wstring>& vecFileName,
	const string & strPathKey )
{
	m_strKey = strKey;
	m_bArray = true;
	vector<string>	vecFullPath;

	const char*	pPath = GET_SINGLE( CPathManager )->FindPathToMultiByte( strPathKey );

	for ( size_t i = 0; i < vecFileName.size(); ++i )
	{
		string	strPath;

		if ( pPath )
			strPath = pPath;

		char	strMultiPath[MAX_PATH] = {};

		WideCharToMultiByte( CP_ACP, 0, vecFileName[i].c_str(), -1, strMultiPath,
			vecFileName[i].length(), 0, 0 );

		strPath += strMultiPath;

		vecFullPath.push_back( strPath.c_str() );
	}

	return LoadTextureFromFullPath( strKey, vecFullPath );
}

bool CTexture::LoadTextureFromFullPath( const string & strKey,
	const char * pFullPath )
{
	m_vecFullPath.push_back( pFullPath );
	m_strKey = strKey;

	char	cExt[_MAX_EXT] = {};

	_splitpath_s( pFullPath, 0, 0, 0, 0, 0, 0, cExt, _MAX_EXT );

	_strupr_s( cExt );

	ScratchImage*	pImage = new ScratchImage;

	TCHAR	strFullPath[MAX_PATH] = {};

	MultiByteToWideChar( CP_ACP, 0, pFullPath, -1, strFullPath,
		strlen( pFullPath ) );

	if ( strcmp( cExt, ".DDS" ) == 0 )
	{
		if ( FAILED( LoadFromDDSFile( strFullPath, DDS_FLAGS_NONE, NULL,
			*pImage ) ) )
		{
			delete	pImage;
			return false;
		}
	}

	else if ( strcmp( cExt, ".TGA" ) == 0 )
	{
		if ( FAILED( LoadFromTGAFile( strFullPath, NULL, *pImage ) ) )
		{
			delete	pImage;
			return false;
		}
	}

	else
	{
		if ( FAILED( LoadFromWICFile( strFullPath, WIC_FLAGS_NONE, NULL,
			*pImage ) ) )
		{
			delete	pImage;
			return false;
		}
	}

	m_vecImage.push_back( pImage );

	if ( FAILED( CreateShaderResourceView( DEVICE, pImage->GetImages(),
		pImage->GetImageCount(), pImage->GetMetadata(),
		&m_pSRView ) ) )
		return false;

	return true;
}

bool CTexture::LoadTextureFromFullPath( const string & strKey,
	const vector<string>& vecFullPath )
{
	m_strKey = strKey;
	m_bArray = true;

	for ( size_t i = 0; i < vecFullPath.size(); ++i )
	{
		m_vecFullPath.push_back( vecFullPath[i] );
		char	cExt[_MAX_EXT] = {};

		_splitpath_s( vecFullPath[i].c_str(), 0, 0, 0, 0, 0, 0, cExt, _MAX_EXT );

		_strupr_s( cExt );

		ScratchImage*	pImage = new ScratchImage;

		TCHAR	strFullPath[MAX_PATH] = {};

		MultiByteToWideChar( CP_ACP, 0, vecFullPath[i].c_str(), -1, strFullPath,
			vecFullPath[i].length() );

		if ( strcmp( cExt, ".DDS" ) == 0 )
		{
			if ( FAILED( LoadFromDDSFile( strFullPath, DDS_FLAGS_NONE, NULL,
				*pImage ) ) )
			{
				delete	pImage;
				return false;
			}
		}

		else if ( strcmp( cExt, ".TGA" ) == 0 )
		{
			if ( FAILED( LoadFromTGAFile( strFullPath, NULL, *pImage ) ) )
			{
				delete	pImage;
				return false;
			}
		}

		else
		{
			if ( FAILED( LoadFromWICFile( strFullPath, WIC_FLAGS_NONE, NULL,
				*pImage ) ) )
			{
				delete	pImage;
				return false;
			}
		}

		m_vecImage.push_back( pImage );
	}

	vector<ID3D11Texture2D*>	vecTex;

	for ( size_t i = 0; i < m_vecImage.size(); ++i )
	{
		ID3D11Texture2D*	pTex = NULL;

		if ( FAILED( CreateTextureEx( DEVICE, m_vecImage[i]->GetImages(),
			m_vecImage[i]->GetImageCount(), m_vecImage[i]->GetMetadata(),
			D3D11_USAGE_STAGING, 0,
			D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ, 0, FALSE, ( ID3D11Resource** )&pTex ) ) )
		{
			Safe_Release_VecList( vecTex );
			return false;
		}

		vecTex.push_back( pTex );
	}

	D3D11_TEXTURE2D_DESC	tTexDesc = {};
	vecTex[0]->GetDesc( &tTexDesc );

	// 읽어온 이미지들로 TextureArray를 생성한다.
	D3D11_TEXTURE2D_DESC	tTexArr = {};

	tTexArr.Width = tTexDesc.Width;
	tTexArr.Height = tTexDesc.Height;
	tTexArr.MipLevels = tTexDesc.MipLevels;
	tTexArr.ArraySize = vecTex.size();
	tTexArr.Format = tTexDesc.Format;
	tTexArr.SampleDesc.Count = 1;
	tTexArr.SampleDesc.Quality = 0;
	tTexArr.Usage = D3D11_USAGE_DEFAULT;
	tTexArr.BindFlags = D3D11_BIND_SHADER_RESOURCE;

	ID3D11Texture2D*	pTexArr = NULL;

	if ( FAILED( DEVICE->CreateTexture2D( &tTexArr, NULL, &pTexArr ) ) )
	{
		Safe_Release_VecList( vecTex );
		return false;
	}

	for ( size_t i = 0; i < vecTex.size(); ++i )
	{
		for ( int iMipLevel = 0; iMipLevel < tTexDesc.MipLevels;
			++iMipLevel )
		{
			D3D11_MAPPED_SUBRESOURCE	tMap = {};

			CONTEXT->Map( vecTex[i], iMipLevel, D3D11_MAP_READ,
				0, &tMap );

			CONTEXT->UpdateSubresource( pTexArr,
				D3D11CalcSubresource( iMipLevel, i, tTexDesc.MipLevels ),
				NULL, tMap.pData, tMap.RowPitch, tMap.DepthPitch );

			CONTEXT->Unmap( vecTex[i], iMipLevel );
		}
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC	tViewDesc = {};

	tViewDesc.Format = tTexArr.Format;
	tViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
	tViewDesc.Texture2DArray.MipLevels = tTexArr.MipLevels;
	tViewDesc.Texture2DArray.ArraySize = vecTex.size();

	if ( FAILED( DEVICE->CreateShaderResourceView( pTexArr, &tViewDesc,
		&m_pSRView ) ) )
	{
		Safe_Release_VecList( vecTex );
		return false;
	}

	Safe_Release_VecList( vecTex );
	SAFE_RELEASE( pTexArr );

	return true;
}

void CTexture::SetTexture( int iRegister, int iShaderConstantType )
{
	if ( iShaderConstantType & SCT_VERTEX )
		CONTEXT->VSSetShaderResources( iRegister, 1, &m_pSRView );

	if ( iShaderConstantType & SCT_PIXEL )
		CONTEXT->PSSetShaderResources( iRegister, 1, &m_pSRView );
}

void CTexture::SetTexturePathKey( const string & strPathKey )
{
	m_strPathKey = strPathKey;
}

void CTexture::ChangeTexturePath( const string & strPath )
{
	vector<string>	vecFullPath;

	for ( size_t i = 0; i < m_vecFullPath.size(); ++i )
	{
		// 풀경로에서 파일 이름만 얻어온다.
		char	strFileName[MAX_PATH] = {};
		char	strExt[_MAX_EXT] = {};
		_splitpath_s( m_vecFullPath[i].c_str(), 0, 0, 0, 0, strFileName, MAX_PATH,
			strExt, _MAX_EXT );
		strcat_s( strFileName, strExt );
		string	strFullPath = strPath + strFileName;

		vecFullPath.push_back( strFullPath );
	}

	m_vecFullPath.clear();
	m_vecFullPath = vecFullPath;
}

void CTexture::SetShaderResourceView( ID3D11ShaderResourceView * pSRV )
{
	pSRV->AddRef();
	m_pSRView = pSRV;
}

void CTexture::Save( FILE * pFile )
{
	int	iLength = m_strKey.length();
	fwrite( &iLength, sizeof( int ), 1, pFile );
	fwrite( m_strKey.c_str(), sizeof( char ), iLength, pFile );

	iLength = m_strPathKey.length();
	fwrite( &iLength, sizeof( int ), 1, pFile );
	fwrite( m_strPathKey.c_str(), sizeof( char ), iLength, pFile );

	int	iCount = m_vecFullPath.size();
	fwrite( &iCount, sizeof( int ), 1, pFile );

	for ( size_t i = 0; i < m_vecFullPath.size(); ++i )
	{
		iLength = m_vecFullPath[i].length();
		fwrite( &iLength, sizeof( int ), 1, pFile );
		fwrite( m_vecFullPath[i].c_str(), sizeof( char ), iLength, pFile );
	}
}

void CTexture::Load( FILE * pFile )
{
}
