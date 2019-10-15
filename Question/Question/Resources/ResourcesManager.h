#pragma once
#include "../Engine.h"

class CResourcesManager
{
private:
	unordered_map<string, class CMesh*>	m_mapMesh;
	unordered_map<string, class CSampler*>	m_mapSampler;

public:
	bool Init();
	class CMesh* CreateMesh(const string& strKey, UINT iVtxCount, UINT iVtxSize, D3D12_USAGE eVtxUsage,
		D3D12_PRIMITIVE_TOPOLOGY ePrimitive, void* pVtxData,
		UINT iIdxCount = 0, UINT iIdxSize = 0,
		D3D12_USAGE eIdxUsage = D3D12_USAGE_DEFAULT,
		DXGI_FORMAT eFmt = DXGI_FORMAT_R32_UINT,
		void* pIdxData = NULL);
	class CMesh* CreateSphere(const string& strKey, float radius, UINT numSubdivisions);
	class CMesh* FindMesh(const string& strKey);

	class CSampler* CreateSampler(const string& strKey, D3D12_FILTER eFilter = D3D12_FILTER_MIN_MAG_MIP_LINEAR,
		D3D12_TEXTURE_ADDRESS_MODE eAddrU = D3D12_TEXTURE_ADDRESS_MODE_WRAP,
		D3D12_TEXTURE_ADDRESS_MODE eAddrV = D3D12_TEXTURE_ADDRESS_MODE_WRAP,
		D3D12_TEXTURE_ADDRESS_MODE eAddrW = D3D12_TEXTURE_ADDRESS_MODE_WRAP );

	class CSampler* FindSampler(const string& strKey);

	DECLARE_SINGLE(CResourcesManager)
};