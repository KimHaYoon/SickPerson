#pragma once

#include "Engine.h"

Engine_BEGIN

class Engine_DLL CDevice
{
private:
	ID3D12Device*					m_pDevice;				// Direct3D ����̽� �������̽�
	IDXGIFactory4*				m_pFactory;			// DXGI ���丮 �������̽�

	IDXGISwapChain3*			m_pSwapChain;		// ���� ü�� �������̽�
	UINT									m_iSwapChainIdx;	// ���� ü�� �ε���
	UINT									m_iSwapChainSize;

	ID3D12Resource**			m_pRTView;				// ���� Ÿ�� ��
	ID3D12DescriptorHeap*	m_pRTVHeap;			// ���� Ÿ�� ������ �� �������̽� ������
	UINT									m_iRTVSize;				// ���� Ÿ�� ������ ������ ũ��

	ID3D12Resource*				m_pDSView;				// ���� ���ٽ� ��
	ID3D12DescriptorHeap*	m_pDSVHeap;			// ���� ���ٽ� �� �������̽� ������
	UINT									m_iDSVSize;				// ���� ���ٽ� ������ ������ ũ��

	ID3D12CommandQueue*				m_pCommandQueue;				// ���ť
	ID3D12CommandAllocator*			m_pCommandAllocator;			// ��� �Ҵ���
	ID3D12GraphicsCommandList*	m_pCommandList;						// ��� ����Ʈ �������̽� ������

	ID3D12PipelineState*			m_pPipelineState;		// �׷��Ƚ� ���������� ���� ��ü�� ���� �������̽� ������

	ID3D12Fence*	m_pFence;					// �潺 �������̽� ������
	UINT64				m_iFenceValue;			// �潺 ��
	HANDLE				m_hFenceEvent;			// �̺�Ʈ �ڵ�
	
	float					m_fClearColor[4];
	RESOLUTION		m_tRS;
	HWND					m_hWnd;

	ID3D12Debug* m_pDebugController;

	bool					m_bMsaa4xEnable;
	UINT					m_iMsaa4xQualityLevels;

	D3D12_VIEWPORT		m_tViewport;

public:
	ID3D12Device* GetDevice()	const;
	//ID3D12DeviceContext* GetContext()	const;
	IDXGISwapChain3* GetSwapChain()	const;
	RESOLUTION GetResolution()	const;

public:
	bool Init(HWND hWnd, UINT iWidth, UINT iHeight, bool bWindowMode);
	//void ClearTarget();
	void Present();
	void WaitForGpuComplete();

public:
	Vector2 GetWindowDeviceResolution()	const;
	Vector2 GetWindowResolution()	const;

	DECLARE_SINGLE(CDevice)
};

Engine_END