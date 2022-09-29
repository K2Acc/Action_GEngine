#include "GeometryObject.h"
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")

using namespace DirectX;

GeometryObject::Common* GeometryObject::common = nullptr;

void GeometryObject::StaticInitialize(DirectXCommon *dxCommon)
{
	common = new Common();
	common->dxCommon = dxCommon;

	common->InitializeGraphicsPipeline();
	common->InitializeDescriptorHeap();

	common->geometryObjectManager = GeometryObjectManager::GetInstance();
	common->textureManager = TextureManager::GetInstance();
}

void GeometryObject::StaticFinalize()
{
	if(common != nullptr) return ;
	delete common;
	common = nullptr;
}

GeometryObject *GeometryObject::Create(UINT texNumber, XMFLOAT4 color)
{
	GeometryObject* object = new GeometryObject(texNumber, color);
	if(object == nullptr){
		return nullptr;
	}

	//������
	if(!object->Initialize(texNumber)){
		delete object;
		assert(0);
		return nullptr;
	}

	return object;
}

GeometryObject::GeometryObject()
{
}

GeometryObject::GeometryObject(UINT texNumber, XMFLOAT4 color)
{
	this->texNumber = texNumber;
	this->color = color;
}

bool GeometryObject::Initialize(UINT texNumber)
{
	HRESULT result;
	this->texNumber = texNumber;

	//�萔�o�b�t�@����
	{
		// �q�[�v�v���p�e�B
		CD3DX12_HEAP_PROPERTIES heapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
		// ���\�[�X�ݒ�
		CD3DX12_RESOURCE_DESC resourceDesc =
		  CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) + 0xff) & ~0xff);

		//����
		result = common->dxCommon->GetDevice()->CreateCommittedResource(
			&heapProps,
			D3D12_HEAP_FLAG_NONE,
			&resourceDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&constBuffer)
		);
		assert(SUCCEEDED(result));

		//�萔�o�b�t�@�̓]��
		result = constBuffer->Map(0, nullptr, (void**)&constMap);
		assert(SUCCEEDED(result));
		constBuffer->Unmap(0, nullptr);
	}
	return true;
}

void GeometryObject::Update(WorldTransform worldTransform, Camera* camera)
{
	//�J�����̍s��擾
	const XMMATRIX& matWorld = worldTransform.matWorld;
	const XMMATRIX& matView = camera->GetMatView();
	const XMMATRIX& matProjection = camera->GetMatProjection();

	constMap->color = color;
	constMap->mat = matWorld * matView * matProjection;
}

void GeometryObject::Draw()
{
#pragma region ���ʕ`��R�}���h
	//�p�C�v���C���X�e�[�g�̐ݒ�
	common->dxCommon->GetCommandList()->SetPipelineState(common->pipelineState.Get());
	//���[�g�V�O�l�`���̐ݒ�
	common->dxCommon->GetCommandList()->SetGraphicsRootSignature(common->rootSignature.Get());
	//�v���~�e�B�u�`���ݒ�
	common->dxCommon->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
#pragma endregion

#pragma region �ʕ`��R�}���h
	//�f�X�N���v�^�q�[�v�̃Z�b�g
	ID3D12DescriptorHeap* ppHeaps[] = {common->basicDescHeap.Get()};
	common->dxCommon->GetCommandList()->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	//�萔�o�b�t�@�r���[(CBV�̐ݒ�R�}���h)
	common->dxCommon->GetCommandList()->SetGraphicsRootConstantBufferView(0, constBuffer->GetGPUVirtualAddress());
	//�f�X�N���v�^�q�[�v�̔z��
	common->textureManager->SetDescriptorHeaps(common->dxCommon->GetCommandList());
	//���_�o�b�t�@�̐ݒ�
	common->dxCommon->GetCommandList()->IASetVertexBuffers(0, 1, &common->geometryObjectManager->GetvbView());
	//�C���f�b�N�X�o�b�t�@�̐ݒ�
	common->dxCommon->GetCommandList()->IASetIndexBuffer(&common->geometryObjectManager->GetibView());

	//�V�F�[�_���\�[�X�r���[���Z�b�g
	common->textureManager->SetShaderResourceView(common->dxCommon->GetCommandList(), 1, texNumber);

	//�`��R�}���h
	common->dxCommon->GetCommandList()->DrawIndexedInstanced(common->geometryObjectManager->GetIndices(),1, 0, 0, 0);
#pragma endregion
}

