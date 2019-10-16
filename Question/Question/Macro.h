#pragma once

#define	SAFE_DELETE(p)	if(p)	{ delete p; p = NULL; }
#define	SAFE_RELEASE(p)	if(p)	{ p->Release(); p = NULL; }
#define	SAFE_DELETE_ARRAY(p)	if(p)	{ delete[] p; p = NULL; }

#define	DECLARE_SINGLE(Type)	\
	private:\
		static Type*	m_pInst;\
	public:\
		static Type* GetInst()\
		{\
			if(!m_pInst)\
				m_pInst	= new Type;\
			return m_pInst;\
		}\
		static void DestroyInst()\
		{\
			SAFE_DELETE(m_pInst);\
		}\
	private:\
		Type();\
		~Type();


#define	DEFINITION_SINGLE(Type)	Type* Type::m_pInst	= NULL;
#define	GET_SINGLE(Type)		Type::GetInst()
#define	DESTROY_SINGLE(Type)	Type::DestroyInst()

#define	DEVICE		GET_SINGLE(CDevice)->GetDevice()
#define	CMDLIST		GET_SINGLE(CDevice)->GetCmdList()
#define	SWAPCHAIN	GET_SINGLE(CDevice)->GetSwapChain()
#define	FENCE	GET_SINGLE(CDevice)->GetFence()
#define	DEVICE_RESOLUTION	GET_SINGLE(CDevice)->GetResolution()
#define ROOTSIGNATURE	GET_SINGLE(CRootSignature)->GetRootSignature()

#define	KEYDOWN(key)	CInput::GetInst()->KeyDown(key)
#define	KEYPUSH(key)	CInput::GetInst()->KeyPush(key)
#define	KEYUP(key)		CInput::GetInst()->KeyUp(key)