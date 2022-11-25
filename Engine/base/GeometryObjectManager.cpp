#include "GeometryObjectManager.h"
#include <cassert>
#include "DirectXCommon.h"

using namespace std;
using namespace DirectX;

GeometryObjectManager *GeometryObjectManager::GetInstance()
{
	static GeometryObjectManager instance;
	return &instance;
}

void GeometryObjectManager::CreateBuffer()
{
	HRESULT result;
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

#pragma region �l�p�`���
	//���_�f�[�^
	VertexPos vertices[] = {
		//�O
		{{0.0f, 0.0f, 0.0f}},//����
	};
	copy(begin(vertices), end(vertices), this->vertices);

#pragma endregion

	//���_�o�b�t�@
	//�T�C�Y
	UINT sizeVB = static_cast<UINT>(sizeof(VertexPos)*_countof(vertices));
	{
		//�q�[�v�v���p�e�B
		CD3DX12_HEAP_PROPERTIES heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
		//���\�[�X�ݒ�
		CD3DX12_RESOURCE_DESC resourceDesc = CD3DX12_RESOURCE_DESC::Buffer(sizeVB);
		//����
		result = dxCommon->GetDevice()->CreateCommittedResource(
			&heapProp,
			D3D12_HEAP_FLAG_NONE,
			&resourceDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&vertBuffer)
		);
		assert(SUCCEEDED(result));

		//�]��
		result = vertBuffer->Map(0, nullptr, (void**)&vertMap);
		if(SUCCEEDED(result)){
			memcpy(vertMap, vertices,sizeof(vertices));
			vertBuffer->Unmap(0,nullptr);
		}
	}

	//���_�o�b�t�@�r���[�̐���
	vbView.BufferLocation = vertBuffer->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeVB;
	vbView.StrideInBytes = sizeof(VertexPos);
}
