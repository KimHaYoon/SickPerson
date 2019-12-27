#pragma once

#define GAME_BEGIN	namespace GAME{
#define GAME_END	}
#define GAME_USING	using namespace GAME;

#ifdef GAME_EXPORT
#define GAME_DLL __declspec(dllexport)
#else
#define GAME_DLL __declspec(dllimport)
#endif //GAME_EXPORT

#define	SAFE_DELETE(p)	if(p)	{ delete p; p = NULL; }
#define	SAFE_RELEASE(p)	if(p)	{ p->Release(); p = NULL; }
#define	SAFE_DELETE_ARRAY(p)	if(p)	{ delete p; p = NULL; }

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
#define	CONTEXT		GET_SINGLE(CDevice)->GetContext()
#define	SWAPCHAIN	GET_SINGLE(CDevice)->GetSwapChain()
#define	DEVICE_RESOLUTION	GET_SINGLE(CDevice)->GetResolution()

#define	KEYDOWN(key)	CInput::GetInst()->KeyDown(key)
#define	KEYPUSH(key)	CInput::GetInst()->KeyPush(key)
#define	KEYUP(key)		CInput::GetInst()->KeyUp(key)