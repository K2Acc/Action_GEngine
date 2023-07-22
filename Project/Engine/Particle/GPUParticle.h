#pragma once
#include "Vector3.h"
#include "Vector4.h"

#include <DirectXMath.h>
#include <d3dx12.h>
#include <DirectXCommon.h>

class GPUParticle{
public:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public:
	//���_
	struct Vertex{
		Vector3 pos;
	};

	//�萔
	struct Const{
		Vector4 velocity;
		Vector4 offset;
		Vector4 color;
		DirectX::XMMATRIX projection;

		float padding[36];
	};

	//�R���s���[�g
	struct Compute{
		float offSetX;
		float offSetY;
		float offSetCull;
		float commandCount;
	};


	//�O���t�B�b�N�X���[�g�V�O�l�`��
	enum GraphicsRootParameters{
		Cbv,
		GraphicsRootParamtersCount
	};

	//�R���s���[�g�V�O�l�`��
	enum ComputeRootParameters{
		SrvUavTable,
		RootConstants,
		ComputeRootParametersCount,
	};


	//�f�X�N���v�^�q�[�v(CBV/SRV/UAV)�쐬�̃I�t�Z�b�g
	enum HeapOffset{
		CbvSrvOffset = 0,
		CommandsOffset = CbvSrvOffset + 1,
		ProcessedCommandsOffset = CommandsOffset + 1,
		CbvSrvUavDescriptorCountPerFrame = ProcessedCommandsOffset + 1
	};

public:
	void Initialize();
	void Update();
	void Draw();
	void Finalize();

private:
	void InitializeRootSignature();
	void InitializeDescriptorHeap();
	void InitializePipeline();

private:
	//�|���S����
    static const UINT FrameCount = 3;

private:
	DirectXCommon* dxCommon_ = nullptr;

	//���[�g�V�O�l�`��
	ComPtr<ID3D12RootSignature> rootSignature;
	ComPtr<ID3D12RootSignature> computeRootSignature;

	//�p�C�v���C��
	ComPtr<ID3D12PipelineState> pipelineState;
	ComPtr<ID3D12PipelineState> computePipelineState;

	//�f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> rtvHeap;
	ComPtr<ID3D12DescriptorHeap> dsvHeap;
	ComPtr<ID3D12DescriptorHeap> cbvSrvUavHeap;
	//�f�X�N���v�^�T�C�Y
	UINT rtvDescriptorSize;
	UINT cbvSrvUavDescriptorSize;


	//GPU�p�[�e�B�N���p�̃R�}���h�A���P�[�^
	ComPtr<ID3D12CommandAllocator> commandAllocators[FrameCount];
	ComPtr<ID3D12CommandAllocator> computeCommandAllocators[FrameCount];

	//GPU�p�[�e�B�N���p�̃R�}���h�L���[
	ComPtr<ID3D12CommandQueue> commandQueue;
    ComPtr<ID3D12CommandQueue> computeCommandQueue;

	//GPU�p�[�e�B�N���p�̃t�F���X
	ComPtr<ID3D12Fence> fence;
    ComPtr<ID3D12Fence> computeFence;

	//GPU�p�[�e�B�N���p�̃R�}���h���X�g
	ComPtr<ID3D12GraphicsCommandList> commandList;
	ComPtr<ID3D12GraphicsCommandList> computeCommandList;
};

