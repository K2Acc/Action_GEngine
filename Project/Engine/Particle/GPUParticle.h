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

public:
	void Initialize();
	void Update();
	void Draw();
	void Finalize();

private:
	DirectXCommon* dxCommon_ = nullptr;

	//���[�g�V�O�l�`��
	ComPtr<ID3D12RootSignature> rootSignature;
	ComPtr<ID3D12RootSignature> computeRootSignature;

	//�p�C�v���C��
	ComPtr<ID3D12PipelineState> pipelineState;
	ComPtr<ID3D12PipelineState> computePipelineState;
};

