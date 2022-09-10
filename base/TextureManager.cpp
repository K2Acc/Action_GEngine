#include "TextureManager.h"
#include <cassert>

using namespace DirectX;
using namespace std;

TextureManager *TextureManager::GetInstance()
{
	static TextureManager instance;
	return &instance;
}

void TextureManager::Load(uint32_t number, const string &fileName)
{
	TextureManager::GetInstance()->LoadTexture(number, fileName);
}

void TextureManager::Initialize(DirectXCommon *dxCommon, string directoryPath)
{
	assert(dxCommon);

	this->dxCommon = dxCommon;
	this->directoryPath = directoryPath;

	//�f�X�N���v�^�T�C�Y�ݒ�
	descriptorHandleIncrementSize = dxCommon->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	//���Z�b�g
	ResetAll();
}

void TextureManager::LoadTexture(uint32_t number, const string filename)
{
	assert(number <= maxTextureCount - 1);

	HRESULT result;
	uint32_t handle = indexNextDescriptorHeap;
	
	//�f�B���N�g���p�X�ƃt�@�C�����̘A��
	string fullPath = directoryPath + filename;

	//���[�h�p��string�����j�R�[�h������ɕϊ�
	wchar_t wfilwPath[256];
	MultiByteToWideChar(CP_ACP, 0, fullPath.c_str(), -1, wfilwPath, _countof(wfilwPath));

	//�摜�t�@�C���p��
	TexMetadata metadata{};
	ScratchImage scratchImage{};
	//WIC�e�N�X�`���f�[�^�̃��[�h
	result = LoadFromWICFile(wfilwPath, WIC_FLAGS_NONE, &metadata, scratchImage);
	assert(SUCCEEDED(result));

	//�~�b�v�}�b�v�̐���
	ScratchImage mipChain{};
	result = GenerateMipMaps(scratchImage.GetImages(), scratchImage.GetImageCount(), 
		scratchImage.GetMetadata(), TEX_FILTER_DEFAULT, 0, mipChain);
	if(SUCCEEDED(result)){
		scratchImage = move(mipChain);
		metadata = scratchImage.GetMetadata();
	}

	//�ǂݍ���ǃf�B�q���[�Y�e�N�X�`����SRGB�Ƃ��Ĉ���
	metadata.format = MakeSRGB(metadata.format);

	//�e�N�X�`���o�b�t�@�ݒ�
	//�q�[�v�ݒ�
	D3D12_HEAP_PROPERTIES textureHandleProp{};
	textureHandleProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	textureHandleProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	textureHandleProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
	//���\�[�X�ݒ�
	CD3DX12_RESOURCE_DESC textureResourceDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		metadata.format,
		metadata.width,
		(UINT)metadata.height,
		(UINT16)metadata.arraySize,
		(UINT16)metadata.mipLevels
	);

	//�e�N�X�`���o�b�t�@����
	result = dxCommon->GetDevice()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0),
		D3D12_HEAP_FLAG_NONE,
		&textureResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&textures[number].resource)
	);
	assert(SUCCEEDED(result));

	//�e�N�X�`���o�b�t�@�ւ̃f�[�^�]��
	//�S�~�b�v�}�b�v
	for(size_t i = 0; i < metadata.mipLevels; i++){
		//�~�b�v�}�b�v���x�����w�肵�ăC���[�W���擾
		const Image* img = scratchImage.GetImage(i,0,0);
		//�e�N�X�`���o�b�t�@�ɓ]��
		result = textures[number].resource->WriteToSubresource(
			(UINT)i,
			nullptr,				//�S�̈�R�s�[
			img->pixels,			//�f�[�^�A�h���X
			(UINT)img->rowPitch,	//1���C���T�C�Y
			(UINT)img->slicePitch	//1���T�C�Y
		);
		assert(SUCCEEDED(result));
	}

	//�V�F�[�_���\�[�X�r���[����
	textures[number].cpuDescHandleSRV = CD3DX12_CPU_DESCRIPTOR_HANDLE(
		descriptorHeap->GetCPUDescriptorHandleForHeapStart(), number, descriptorHandleIncrementSize
	);
	textures[number].gpuDescHandleSRV = CD3DX12_GPU_DESCRIPTOR_HANDLE(
		descriptorHeap->GetGPUDescriptorHandleForHeapStart(), number, descriptorHandleIncrementSize
	);

	//���\�[�X�ݒ�
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	D3D12_RESOURCE_DESC resourceDesc = textures[number].resource->GetDesc();
	srvDesc.Format = resourceDesc.Format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = textureResourceDesc.MipLevels;

	//�n���h���̎w���ʒu�ɃV�F�[�_���\�[�X�r���[�쐬
	dxCommon->GetDevice()->CreateShaderResourceView(
		textures[number].resource.Get(),
		&srvDesc,
		textures[number].cpuDescHandleSRV
	);
	indexNextDescriptorHeap++;
}

void TextureManager::ResetAll()
{
	HRESULT result;

	//�f�X�N���v�^�q�[�v����
	D3D12_DESCRIPTOR_HEAP_DESC descriptorHeapDesc = {};
	descriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	descriptorHeapDesc.NumDescriptors = maxTextureCount;

	result = dxCommon->GetDevice()->CreateDescriptorHeap(&descriptorHeapDesc,IID_PPV_ARGS(&descriptorHeap));
	assert(SUCCEEDED(result));

	indexNextDescriptorHeap = 0;

	//�S�e�N�X�`��������
	for(size_t i = 0; i < maxTextureCount; i++){
		textures[i].resource.Reset();
		textures[i].cpuDescHandleSRV.ptr = 0;
		textures[i].gpuDescHandleSRV.ptr = 0;
		textures[i].name.clear();
	}
}

ID3D12Resource *TextureManager::GetTextureBuffer(uint32_t number)
{
	assert(number < maxTextureCount);
	return textures[number].resource.Get();
}

const D3D12_RESOURCE_DESC TextureManager::GetResoureDesc(uint32_t number)
{
	assert(number < textures.size());
	Texture& texture = textures.at(number);
	return texture.resource->GetDesc();
}


void TextureManager::SetDescriptorHeaps(ID3D12GraphicsCommandList *commandList)
{
	//�e�N�X�`���p�f�X�N���v�^�q�[�v�̐ݒ�
	ID3D12DescriptorHeap* ppHeaps[] = {descriptorHeap.Get()};
	commandList->SetDescriptorHeaps(_countof(ppHeaps),ppHeaps);
}

void TextureManager::SetShaderResourceView(ID3D12GraphicsCommandList *commandList, UINT RootParameterIndex, uint32_t texnumber)
{
	//SRV�̃T�C�Y���擾
	UINT sizeSRV = dxCommon->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	//�f�X�N���v�^�e�[�u���̐擪���擾
	D3D12_GPU_DESCRIPTOR_HANDLE start = descriptorHeap->GetGPUDescriptorHandleForHeapStart();

	//SRV��GPU�n���h���擾
	CD3DX12_GPU_DESCRIPTOR_HANDLE handle = CD3DX12_GPU_DESCRIPTOR_HANDLE(start, texnumber,sizeSRV);

	//�V�F�[�_�[���\�[�X�r���[(SRV)���Z�b�g
	commandList->SetGraphicsRootDescriptorTable(RootParameterIndex, handle);
}
