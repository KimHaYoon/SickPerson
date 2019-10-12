#pragma once
#include "../Obj.h"

Engine_BEGIN

class Engine_DLL Pipeline : public CObj
{
private:
	D3D12_INPUT_LAYOUT_DESC* m_pInputLayout;
public:
	Pipeline();
	~Pipeline();
};

Engine_END