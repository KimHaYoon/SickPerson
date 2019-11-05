#pragma once

#include "../Engine.h"
#include "fbxsdk.h"

Engine_BEGIN

// Bone
typedef struct Engine_DLL _tagFbxBone
{
	string	strName;
	int		iDepth;
	int		iParentIndex;
	FbxAMatrix	matOffset;
	FbxAMatrix	matBone;
}FBXBONE, *PFBXBONE;

// Key Frame
typedef	struct Engine_DLL _tagFbxKeyFrame
{
	FbxAMatrix	matTransform;
	double		dTime;
}FBXKEYFRAME, *PFBXKEYFRAME;

// Bone Key Frame
typedef	struct Engine_DLL _tagFbxBoneKeyFrame
{
	int		iBoneIndex;
	vector<FBXKEYFRAME>	vecKeyFrame;
}FBXBONEKEYFRAME, *PFBXBONEKEYFRAME;

// Fbx Animation Clip
typedef struct Engine_DLL _tagFbxAnimationClip
{
	string			strName;
	FbxTime			tStart;
	FbxTime			tEnd;
	FbxLongLong		lTimeLength;
	FbxTime::EMode	eTimeMode;
	vector<FBXBONEKEYFRAME>	vecBoneKeyFrame;
}FBXANIMATIONCLIP, *PFBXANIMATIONCLIP;

typedef struct Engine_DLL _tagFbxMaterial
{
	Vector4		vDif;
	Vector4		vAmb;
	Vector4		vSpc;
	Vector4		vEmv;
	float		fSpecularPower;
	float		fTransparencyFactor;
	float		fShininess;
	string		strDifTex;
	string		strBumpTex;
	string		strSpcTex;
}FBXMATERIAL, *PFBXMATERIAL;

typedef struct Engine_DLL _tagFbxWeight
{
	int		iIndex;
	double	dWeight;
}FBXWEIGHT, *PFBXWEIGHT;

typedef struct Engine_DLL _tagFBXMeshContainer
{
	vector<Vector3>	vecPos;
	vector<Vector3>	vecNormal;
	vector<Vector2>	vecUV;
	vector<Vector3>	vecTangent;
	vector<Vector3>	vecBinormal;
	vector<Vector4>	vecBlendWeight;
	vector<Vector4>	vecBlendIndex;
	vector<vector<UINT>>	vecIndices;
	unordered_map<int, vector<FBXWEIGHT>>	mapWeights;
	bool				bBump;
	bool				bAnimation;

	_tagFBXMeshContainer()
	{
		bBump = false;
		bAnimation = false;
	}
}FBXMESHCONTAINER, *PFBXMESHCONTAINER;

class Engine_DLL CFbxLoader
{
private:
	friend class CAnimation;
	friend class CMesh;

private:
	CFbxLoader();
	~CFbxLoader();

private:
	FbxManager*		m_pManager;
	FbxScene*		m_pScene;
	vector<vector<PFBXMATERIAL>>	m_vecMaterial;
	vector<PFBXMESHCONTAINER>		m_vecMeshContainer;
	vector<PFBXBONE>				m_vecBones;
	FbxArray<FbxString*>			m_NameArr;
	vector<PFBXANIMATIONCLIP>		m_vecClip;
	bool							m_bMixamo;

public:
	const vector<PFBXMESHCONTAINER>* GetMeshContainer()	const;
	const vector<vector<PFBXMATERIAL>>* GetMaterials()	const;
	const vector<PFBXBONE>* GetBones()	const;
	const vector<PFBXANIMATIONCLIP>* GetClips()	const;

public:
	bool LoadFbx(const char* pFullPath);
	bool LoadAnimation(const char* pFullPath);

private:
	void Triangulate(FbxNode* pNode);
	void LoadMaterial(FbxSurfaceMaterial * pMtrl);
	Vector4 GetMaterialColor(FbxSurfaceMaterial * pMtrl,
		const char * pPropertyName, const char * pPropertyFactorName);
	double GetMaterialFactor(FbxSurfaceMaterial * pMtrl,
		const char * pPropertyName);
	string GetMaterialTexture(FbxSurfaceMaterial * pMtrl, const char * pPropertyName);

	void LoadMesh(FbxNode* pNode);
	void LoadMesh(FbxMesh* pMesh);
	void LoadAnimation(FbxNode* pNode);
	void LoadAnimation(FbxMesh* pMesh);
	void LoadNormal(FbxMesh * pMesh, PFBXMESHCONTAINER pContainer, int iVtxID,
		int iControlIndex);
	void LoadUV(FbxMesh * pMesh, PFBXMESHCONTAINER pContainer,
		int iUVID, int iControlIndex);
	void LoadTangent(FbxMesh * pMesh,
		PFBXMESHCONTAINER pContainer, int iVtxID, int iControlIndex);
	void LoadBinormal(FbxMesh * pMesh, PFBXMESHCONTAINER pContainer,
		int iVtxID, int iControlIndex);

	void LoadAnimationClip();
	void LoadSkeleton(FbxNode* pNode);
	void LoadSkeletonRecursive(FbxNode* pNode, int iDepth, int iIndex, int iParentIndex);
	void LoadAnimation(FbxMesh* pMesh, PFBXMESHCONTAINER pContainer);
	void LoadWeightAndIndex(FbxCluster * pCluster, int iBoneIndex,
		PFBXMESHCONTAINER pContainer);
	void LoadOffsetMatrix(FbxCluster * pCluster,
		const FbxAMatrix & matTransform, int iBoneIndex,
		PFBXMESHCONTAINER pContainer);
	void LoadTimeTransform(FbxNode * pNode,
		FbxCluster * pCluster, const FbxAMatrix & matTransform,
		int iBoneIndex);
	void ChangeWeightAndIndices(PFBXMESHCONTAINER pContainer);

private:
	FbxAMatrix GetTransform(FbxNode* pNode);
	int FindBoneFromName(const string & strName);
};

Engine_END
