#include "Sprite.h"
#include <d3dcompiler.h>
#include <cassert>

#pragma comment(lib, "d3dcompiler.lib")

using namespace DirectX;
using namespace std;
using namespace Microsoft::WRL;

Sprite::Common* Sprite::common = nullptr;
UINT Sprite::descriptorHandleIncrementSize;
ID3D12GraphicsCommandList* Sprite::commandList = nullptr;

void Sprite::Common::InitializeGraphicsPipeline(const std::wstring &directoryPath)
{
	HRESULT result;

	//�V�F�[�_�[�̓ǂݍ��݃R���p�C��
	ComPtr<ID3DBlob> vsBlob;
	ComPtr<ID3DBlob> psBlob;
	ComPtr<ID3DBlob> errorBlob;

	//���_�V�F�[�_�[
	wstring vsFile = directoryPath + L"/shader/SpriteVS.hlsl";
	result = D3DCompileFromFile(
		vsFile.c_str(),
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,	//�C���N���[�h�\
		"main", "vs_5_0",	//�G���g���[�|�C���g�A�V�F�[�_�[���f��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,	//�f�o�b�N�p�ݒ�
		0, &vsBlob, &errorBlob
	);
	if(FAILED(result)){
		string error;
		error.resize(errorBlob->GetBufferSize());
		copy_n((char*)errorBlob->GetBufferPointer(), errorBlob->GetBufferSize(), error.begin());
		error += "\n";
		//�G���[���e���o�͂ɕ\��
		OutputDebugStringA(error.c_str());
		assert(0);
	}

	//���_�V�F�[�_�[
	wstring psFile = directoryPath + L"/shader/SpritePS.hlsl";
	result = D3DCompileFromFile(
		psFile.c_str(),
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,	//�C���N���[�h�\
		"main", "ps_5_0",	//�G���g���[�|�C���g�A�V�F�[�_�[���f��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,	//�f�o�b�N�p�ݒ�
		0, &psBlob, &errorBlob
	);
	if(FAILED(result)){
		string error;
		error.resize(errorBlob->GetBufferSize());
		copy_n((char*)errorBlob->GetBufferPointer(), errorBlob->GetBufferSize(), error.begin());
		error += "\n";
		//�G���[���e���o�͂ɕ\��
		OutputDebugStringA(error.c_str());
		assert(0);
	}

	//���_���C�A�E�g
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{//xy���W
			"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{//uv���W
			"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
	};

	//�O���t�B�b�N�X�p�C�v���C��
	//�O���t�B�b�N�X�p�C�v���C���ݒ�
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipelineDesc{};
	gpipelineDesc.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());
	gpipelineDesc.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());

	//�T���v���}�X�N
	gpipelineDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;	//�W���ݒ�

	//���X�^���C�U�ݒ�		�w�ʃJ�����O�@�|���S�����h��Ԃ��@�[�x�N���b�s���O�L��
	gpipelineDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	gpipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;

	//�f�v�X�X�e���V���X�e�[�g�̐ݒ�	(�[�x�e�X�g���s���A�������݋��A�[�x������������΋���)
	gpipelineDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	gpipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS;
	gpipelineDesc.DepthStencilState.DepthEnable = false;

	//�[�x�o�b�t�@�t�H�[�}�b�g
	gpipelineDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;	//�[�x�l�t�H�[�}�b�g

	//���_���C�A�E�g
	gpipelineDesc.InputLayout.pInputElementDescs = inputLayout;
	gpipelineDesc.InputLayout.NumElements = _countof(inputLayout);

	//�}�`�ݒ�
	gpipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	//���̑��ݒ�
	gpipelineDesc.NumRenderTargets = 1;		//�`��Ώۂ͈��
	gpipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;	//0~255�w���RGBA
	gpipelineDesc.SampleDesc.Count = 1;	//1�s�N�Z���ɂ�1��T���v�����O

	//�f�X�N���v�^�����W
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV{};
	descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);

	//���[�g�p�����[�^
	////�萔�o�b�t�@ 0��
	CD3DX12_ROOT_PARAMETER rootParam[2] = {};
	rootParam[0].InitAsConstantBufferView(0);
	rootParam[1].InitAsDescriptorTable(1, &descRangeSRV);

	//�e�N�X�`���T���v���[
	CD3DX12_STATIC_SAMPLER_DESC samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0);

	//���[�g�V�O�l�`��
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Init_1_0(_countof(rootParam), rootParam,1, &samplerDesc,D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	//�V���A���C�Y
	//�o�[�W���������ݒ�
	ComPtr<ID3DBlob> rootSigBlob;
	result = D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob,&errorBlob);
	assert(SUCCEEDED(result));

	//���[�g�V�O�l�`������
	result = dxCommon->GetDevice()->CreateRootSignature(0,rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),IID_PPV_ARGS(&common->rootsignature));
	assert(SUCCEEDED(result));

	//�p�C�v���C���ɃV�O�l�`�����Z�b�g
	gpipelineDesc.pRootSignature = common->rootsignature.Get();

	//�u�����h�X�e�[�g
	// //�����_�[�^�[�Q�b�g�̃u�����h�ݒ�
	D3D12_RENDER_TARGET_BLEND_DESC blenddesc{};
	gpipelineDesc.BlendState.RenderTarget[0] = blenddesc;
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;	//RGBA���ׂẴ`�����l����`��
	//���ʐݒ�
	blenddesc.BlendEnable = true;						//�u�����h��L���ɂ���
	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;		//���Z
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;			//�\�[�X�̒l��100% �g��	(�\�[�X�J���[			 �F ������`�悵�悤�Ƃ��Ă���F)
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;		//�f�X�g�̒l��  0% �g��	(�f�X�e�B�l�[�V�����J���[�F ���ɃL�����o�X�ɕ`����Ă���F)
	//�e��ݒ�
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;	//�ݒ�
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;			//�\�[�X�̒l�� ��% �g��
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;	//�f�X�g�̒l�� ��% �g��

	//�O���t�B�b�N�p�C�v���C������
	result = dxCommon->GetDevice()->CreateGraphicsPipelineState(&gpipelineDesc, IID_PPV_ARGS(&common->pipelinestate));
	assert(SUCCEEDED(result));
}

void Sprite::StaticInitialize(DirectXCommon* dxCommon, int window_width, int window_height, const std::wstring &directoryPath)
{
	common = new Common();

	assert(dxCommon);

	common->dxCommon = dxCommon;
	descriptorHandleIncrementSize = dxCommon->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	//�p�C�v���C������
	common->InitializeGraphicsPipeline(directoryPath);

	//�ˉe�s�񐶐�
	common->matProjection = XMMatrixOrthographicOffCenterLH(
		0.f, (float)window_width,
		(float)window_height, 0.f,
		0.f, 1.f
	);
}

void Sprite::StaticFinalize()
{
	if(common != nullptr){
		delete common;
		common = nullptr;
	}
}

void Sprite::PreDraw(ID3D12GraphicsCommandList *commandList)
{
	assert(Sprite::commandList = nullptr);
	assert(commandList);

	Sprite::commandList = commandList;

	//�p�C�v���C���X�e�[�g�ݒ�
	Sprite::commandList->SetPipelineState(common->pipelinestate.Get());
	//���[�g�V�O�l�`���ݒ�
	Sprite::commandList->SetGraphicsRootSignature(common->rootsignature.Get());
	//�v���~�e�B�u�`��ݒ�
	Sprite::commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
}

void Sprite::PostDraw()
{
	Sprite::commandList = nullptr;
}

Sprite *Sprite::Create(uint32_t textureNumber, Vector2 pos, DirectX::XMFLOAT4 color, Vector2 anchorpoint, bool isFlipX, bool isFlipY)
{
	//���T�C�Y
	Vector2 size = {100,100};
	
	if(TextureManager::GetInstance()->GetTextureBuffer(textureNumber))
	{
		//�e�N�X�`�����擾
		const D3D12_RESOURCE_DESC& resourceDesc = TextureManager::GetInstance()->GetResoureDesc(textureNumber);
		//�X�v���C�g�̃T�C�Y���e�N�X�`���̃T�C�Y�ݒ�
		size = {(float)resourceDesc.Width, (float)resourceDesc.Height};
	}

	//�C���X�^���X����
	Sprite* sprite = new Sprite(textureNumber, pos, size, color, anchorpoint, isFlipX, isFlipY);
	if(!sprite) return nullptr;

	//������(���s�Ȃ�폜)
	if(!sprite->Initialize()){
		delete sprite;
		assert(0);
		return nullptr;
	}
	return sprite;
}

Sprite::Sprite()
{
}

Sprite::Sprite(uint32_t textureNumber, Vector2 pos, Vector2 size, DirectX::XMFLOAT4 color, Vector2 anchorpoint, bool isFlipX, bool isFlipY)
{
	this->textureHandle = textureNumber;
	this->position = pos;
	this->size = size;
	this->anchorPoint = anchorPoint;
	this->color = color;
	this->matWorld = XMMatrixIdentity();
	this->isFlipX = isFlipX;
	this->isFlipY = isFlipY;
	this->texSize = size;
}

bool Sprite::Initialize()
{
	HRESULT result;

	//���_�o�b�t�@
	{
		//�q�[�v�v���p�e�B
		CD3DX12_HEAP_PROPERTIES heapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
		//���\�[�X�ݒ�
		CD3DX12_RESOURCE_DESC resourceDesc = CD3DX12_RESOURCE_DESC::Buffer(sizeof(VertexPosUv)*VertNum);
		//����
		result = common->dxCommon->GetDevice()->CreateCommittedResource(
			&heapProps, D3D12_HEAP_FLAG_NONE, &resourceDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&vertBuffer)
		);
		assert(SUCCEEDED(result));
	}
	//�]��
	TransferVertices();
	
	//���_�o�b�t�@�r���[����
	vbView.BufferLocation = vertBuffer->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeof(VertexPosUv)*VertNum;
	vbView.StrideInBytes = sizeof(VertexPosUv);

	//�萔�o�b�t�@
	{
		//�q�[�v�v���p�e�B
		CD3DX12_HEAP_PROPERTIES heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
		//���\�[�X�ݒ�
		CD3DX12_RESOURCE_DESC resourceDesc = CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) + 0xff) & ~0xff);
		//����
		result = common->dxCommon->GetDevice()->CreateCommittedResource(
			&heapProp, D3D12_HEAP_FLAG_NONE, &resourceDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&constBuffer)
		);
		assert(SUCCEEDED(result));
	}
	//�]��
	result = constBuffer->Map(0, nullptr, (void**)&constMap);
	if(SUCCEEDED(result)){
		constMap->color = XMFLOAT4(1,1,1,1);
		constMap->mat = common->matProjection;
		this->constBuffer->Unmap(0, nullptr);
	}

	return true;
}

void Sprite::Draw()
{
	//���[���h���W�X�V
	matWorld = XMMatrixIdentity();
	matWorld *= XMMatrixRotationZ(XMConvertToRadians(rotation));
	matWorld *= XMMatrixTranslation(position.x, position.y, 0.f);

	//�萔�o�b�t�@�]��
	HRESULT result;
	result = constBuffer->Map(0, nullptr, (void**)&constMap);
	assert(SUCCEEDED(result));
	constMap->color = color;
	constMap->mat = matWorld * common->matProjection;
	constBuffer->Unmap(0, nullptr);

	//��\��
	if(IsInvisible){
		return;
	}

	//���_�o�b�t�@
	commandList->IASetVertexBuffers(0,1,&vbView);
	//�萔�o�b�t�@
	commandList->SetGraphicsRootConstantBufferView(0, constBuffer->GetGPUVirtualAddress());
	//�e�N�X�`���p�f�X�N���v�^�q�[�v
	TextureManager::GetInstance()->SetDescriptorHeaps(commandList);
	//�V�F�[�_�[���\�[�X�r���[
	TextureManager::GetInstance()->SetShaderResourceView(commandList, 1, textureHandle);
	//�`��
	commandList->DrawInstanced(4,1,0,0);
}

void Sprite::SetPosition(Vector2 pos)
{
	position = pos;

	TransferVertices();
}

void Sprite::SetRotation(float rotation)
{
	this->rotation = rotation;

	TransferVertices();
}

void Sprite::SetSize(Vector2 size)
{
	this->size = size;

	TransferVertices();
}

void Sprite::SetAnchorpoint(Vector2 pos)
{
	anchorPoint = pos;

	TransferVertices();
}

void Sprite::SetTextureRect(float tex_x, float tex_y, float tex_width, float tex_height)
{
	texBase = {tex_x, tex_y};
	texSize = {tex_width, tex_height};

	TransferVertices();
}

void Sprite::SetIsFlipX(bool IsFlipX)
{
	isFlipX = IsFlipX;

	TransferVertices();
}

void Sprite::SetIsFlipY(bool IsFlipY)
{
	isFlipY = isFlipY;

	TransferVertices();
}

void Sprite::SetColor(DirectX::XMFLOAT4 color)
{
	this->color = color;

	TransferVertices();
}

void Sprite::TransferVertices()
{
	HRESULT result = S_FALSE;

	enum {LB, LT, RB, RT};

	float left = (0.f - this->anchorPoint.x)* this->size.x;
	float right = (1.f - this->anchorPoint.x)* this->size.x;
	float top = (0.f - this->anchorPoint.y)* this->size.y;
	float bottom = (1.f - this->anchorPoint.y)* this->size.y;

	if(this->isFlipX)
	{//���E����ւ�
		left = -left;
		right = -right;
	}
	if(this->isFlipY)
	{//�㉺���]
		top = -top;
		bottom = -bottom;
	}

	//���_�f�[�^
	VertexPosUv vertices[VertNum];
	vertices[LB].pos = {left, bottom, 0.f};
	vertices[LT].pos = {left, top, 0.f};
	vertices[RB].pos = {right, bottom, 0.f};
	vertices[RT].pos = {right, top, 0.f};

	//UV�v�Z
	//�w��ԍ��̉摜���Ǎ��ς݂Ȃ�
	if(TextureManager::GetInstance()->GetTextureBuffer(this->textureHandle))
	{
		//�e�N�X�`�����擾
		D3D12_RESOURCE_DESC resDesc = TextureManager::GetInstance()->GetTextureBuffer(this->textureHandle)->GetDesc();

		float tex_left = this->texBase.x / resDesc.Width;
		float tex_right = (this->texBase.x + this->texSize.x) / resDesc.Width;
		float tex_top = this->texBase.y / resDesc.Height;
		float tex_bottom = (this->texBase.y + this->texSize.y) / resDesc.Height;
	
		vertices[LB].uv = {tex_left, tex_bottom};
		vertices[LT].uv = {tex_left, tex_top};
		vertices[RB].uv = {tex_right, tex_bottom};
		vertices[RT].uv = {tex_right, tex_top};
	}

	//���_�o�b�t�@�̃f�[�^�]��
	VertexPosUv* vertMap = nullptr;
	result = this->vertBuffer->Map(0, nullptr, (void**)&vertMap);
	memcpy(vertMap, vertices,sizeof(vertices));
	this->vertBuffer->Unmap(0,nullptr);
}
