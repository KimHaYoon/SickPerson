#pragma once

#define		SAFE_DELETE(p)	if(p)	{delete p; p = NULL;}
#define		SAFE_DELETE_ARRAY(p)	if(p)	{delete[]p;	p = NULL;}
#define		SAFE_RELEASE(p)	if(p)	{p->Release(); p = NULL;}

#define		DECLARE_SINGLE(Type)\
		private:\
			Type();\
			~Type();\
		private:\
			static Type*		m_pInst;\
		public:\
			static	Type*	GetInst()\
			{\
					if(!m_pInst)\
							m_pInst = new Type;\
					return m_pInst;\
			}\
			static void		DestroyInst()\
			{\
					SAFE_DELETE(m_pInst);\
			}

#define		DEFINITION_SINGLE(Type)	Type*	Type::m_pInst	=	NULL;
#define		GET_SINGLE(Type)		Type::GetInst() 
#define		DESTROY_SINGLE(Type)		Type::DestroyInst()