#include "FbxModelManager.h"
#include "DirectXCommon.h"

using namespace DirectX;
using namespace std;

FbxModelManager *FbxModelManager::GetInstance()
{
	static FbxModelManager instance;
	return &instance;
}

FbxModelManager::~FbxModelManager()
{
}

void FbxModelManager::CreateBuffers()
{
	HRESULT result = S_FALSE;
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	//���_�f�[�^�̑S�̃T�C�Y
	UINT sizeVB = static_cast<UINT>(sizeof(VertexPosNormalUvSkin)*vertices.size());
	{
		//���_�o�b�t�@�̐���
		result = dxCommon->GetDevice()->CreateCommittedResource
		(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer(sizeVB),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&verticesBuffer)
		);
		assert(SUCCEEDED(result));
		//���_�o�b�t�@�ւ̃f�[�^�]��
		result = verticesBuffer->Map(0, nullptr, (void**)&verticesMap);
		if(SUCCEEDED(result))
		{
			copy(vertices.begin(), vertices.end(), verticesMap);
			verticesBuffer->Unmap(0, nullptr);
		}
	}
	//���_�o�b�t�@�r���[(VBV)�̍쐬
	vbView.BufferLocation = verticesBuffer->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeVB;
	vbView.StrideInBytes = sizeof(vertices[0]);

	//���_�C���f�b�N�X�S�̂̃T�C�Y
	UINT sizeIB = static_cast<UINT>(sizeof(unsigned short) * indices.size());
	{
		//�C���f�b�N�X�o�b�t�@����
		result = dxCommon->GetDevice()->CreateCommittedResource
		(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer(sizeIB),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&indexBuffer)
		);
		assert(SUCCEEDED(result));
		//�C���f�b�N�X�o�b�t�@�ւ̃f�[�^�]��
		result = indexBuffer->Map(0, nullptr,(void**)&indicesMap);
		if(SUCCEEDED(result))
		{
			copy(indices.begin(),indices.end(),indicesMap);
			indexBuffer->Unmap(0, nullptr);
		}
	}
	//�C���f�b�N�X�o�b�t�@�r���[(IBV)�̍쐬
	ibView.BufferLocation = indexBuffer->GetGPUVirtualAddress();
	ibView.Format = DXGI_FORMAT_R16_UINT;
	ibView.SizeInBytes= sizeIB;

	//�e�N�X�`���摜�f�[�^
	const Image* img = scratchImage.GetImage(0, 0, 0);//���f�[�^���o
	assert(img);
	//���\�[�X�ݒ�
	CD3DX12_RESOURCE_DESC texresDesc = CD3DX12_RESOURCE_DESC::Tex2D
		(
			metaData.format,
			metaData.width,
			(UINT)metaData.height,
			(UINT)metaData.arraySize,
			(UINT)metaData.mipLevels
		);
	//�e�N�X�`���p�o�b�t�@�̐���
	result = dxCommon->GetDevice()->CreateCommittedResource
		(
			&CD3DX12_HEAP_PROPERTIES
			(
				D3D12_CPU_PAGE_PROPERTY_WRITE_BACK,
				D3D12_MEMORY_POOL_L0
			),
			D3D12_HEAP_FLAG_NONE,
			&texresDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,	//�e�N�X�`���p�w��
			nullptr,
			IID_PPV_ARGS(&textureBuffer)
		);
	assert(SUCCEEDED(result));
	//�e�N�X�`���o�b�t�@�Ƀf�[�^�]��
	result = textureBuffer->WriteToSubresource
		(
			0,
			nullptr,				//�S�̈�փR�s�[
			img->pixels,			//���f�[�^�A�h���X
			(UINT)img->rowPitch,	//1���C���T�C�Y
			(UINT)img->slicePitch	//1���T�C�Y
		);
	assert(SUCCEEDED(result));
	
	//SRV�p�f�X�N���v�^�q�[�v�𐶐�
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;	//�V�F�[�_�[���猩����悤��
	descHeapDesc.NumDescriptors = 1;	//�e�N�X�`������
	result = dxCommon->GetDevice()->CreateDescriptorHeap(&descHeapDesc,IID_PPV_ARGS(&descriptorHeapSRV));//����
	assert(SUCCEEDED(result));
	//�V�F�[�_�[���\�[�X�r���[(SRV)����
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};	//�ݒ�\����
	D3D12_RESOURCE_DESC resDesc= textureBuffer->GetDesc();

	srvDesc.Format = resDesc.Format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension =D3D12_SRV_DIMENSION_TEXTURE2D;	//2D�e�N�X�`��
	srvDesc.Texture2D.MipLevels = 1;

	dxCommon->GetDevice()->CreateShaderResourceView
	(
		textureBuffer.Get(),	//�r���[�Ɗ֘A�t����o�b�t�@
		&srvDesc,	//�e�N�X�`���ݒ���
		descriptorHeapSRV->GetCPUDescriptorHandleForHeapStart()	//�q�[�v�̐擪�A�h���X
	);
}

