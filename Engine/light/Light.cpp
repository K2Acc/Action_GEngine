#include "Light.h"

DirectXCommon* Light::dxCommon = nullptr;

using namespace DirectX;

Light *Light::Create()
{
	//3D�I�u�W�F�N�g�̃C���X�^���X��
	Light* instance = new Light();
	//������
	instance->Initialize();
	//���������C���X�^���X��Ԃ�
	return instance;
}

void Light::StaticInitialize(DirectXCommon* dxCommon)
{
	assert(!Light::dxCommon);
	assert(dxCommon);
	Light::dxCommon = dxCommon;
}

void Light::Initialize()
{
	HRESULT result;
	//�萔�o�b�t�@����
	result = dxCommon->GetDevice()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) + 0xff)&~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff)
	);
	assert(SUCCEEDED(result));

	//�]��
	TransferConstBuffer();
}

void Light::Update()
{
	//�l�̍X�V���������������ɒ萔�o�b�t�@�ɓ]������
	if(IsDirty){
		TransferConstBuffer();
		IsDirty = false;
	}
}

void Light::Draw(UINT rootParameterIndex)
{
	//�萔�o�b�t�@�r���[���Z�b�g
	dxCommon->GetCommandList()->SetGraphicsRootConstantBufferView(
		rootParameterIndex, 
		constBuff->GetGPUVirtualAddress()
	);
}

void Light::TransferConstBuffer()
{
	HRESULT result;
	//�萔�o�b�t�@�]��
	ConstBufferData* constMap = nullptr;
	result = constBuff->Map(0,nullptr, (void**)&constMap);
	if(SUCCEEDED(result))
	{
		constMap->lightv = {-lightdir.m128_f32[0], -lightdir.m128_f32[1], -lightdir.m128_f32[2],-lightdir.m128_f32[3]};
		constMap->lightcolor = lightcolor;
		constBuff->Unmap(0,nullptr);
	}
}

void Light::SetLightDir(const DirectX::XMVECTOR &lightdir)
{
	this->lightdir = XMVector3Normalize(lightdir);
	IsDirty = true;
}

void Light::SetLightColor(const Vector3 lightcolor)
{
	this->lightcolor = lightcolor;
	IsDirty = true;
}
