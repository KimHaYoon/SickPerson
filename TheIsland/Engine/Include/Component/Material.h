#pragma once
#include "Component.h"

GAME_BEGIN

typedef struct GAME_DLL _tagTextureInfo
{
	class CTexture*		pTexture;
	int					iRegisterTex;
	class CSampler*		pSampler;
	int					iRegisterSmp;

	_tagTextureInfo() :
		pTexture( NULL ),
		pSampler( NULL ),
		iRegisterTex( 0 ),
		iRegisterSmp( 0 )
	{
	}
}TEXTUREINFO, *PTEXTUREINFO;

class GAME_DLL CMaterial :
	public CComponent
{
private:
	friend class CRenderer2D;

private:
	CMaterial();
	CMaterial( const CMaterial& material );
	~CMaterial();

private:
	TEXTUREINFO		m_tDifInfo;

public:
	void SetDiffuseTexInfo( const string& strSmpKey,
		const string& strDifKey, int iRegTex = 0, int iRegSmp = 0,
		const TCHAR* pFileName = NULL,
		const string& strPathKey = TEXTURE_PATH );
	void SetDiffuseTexInfo( const string& strSmpKey,
		const string& strDifKey, int iRegTex = 0, int iRegSmp = 0,
		const vector<wstring>* vecFileName = NULL,
		const string& strPathKey = TEXTURE_PATH );
	void SetDiffuseTex( class CTexture* pTex );
	void SetDiffuseTex( const string& strKey );
	void SetDiffuseTex( const string& strKey, const char* pFullPath );
	void SetDiffuseTex( const string& strKey, const vector<string>& vecFullPath );
	void SetMaterial( int iShaderConstantType );

public:
	virtual bool Init();
	virtual void Input( float fTime );
	virtual int Update( float fTime );
	virtual int LateUpdate( float fTime );
	virtual void Collision( float fTime );
	virtual void Render( float fTime );
	virtual CMaterial* Clone();
	virtual void Save( FILE* pFile );
	virtual void Load( FILE* pFile );
};

GAME_END
