#include "ParticleManager.h"
#include <cassert>
#include "DirectXCommon.h"

using namespace std;
using namespace DirectX;

ParticleManager *ParticleManager::GetInstance()
{
	static ParticleManager instance;
	return &instance;
}

void ParticleManager::CreateBuffer()
{
	HRESULT result;
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

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

void ParticleManager::Update()
{
	//�����������p�[�e�B�N���̑S�폜
	particle.remove_if([](Particle& x){
		return x.frame >= x.num_frame;
		}
	);

	//�S�p�[�e�B�N���X�V
	for(std::forward_list<Particle>::iterator it = particle.begin(); it != particle.end(); it++)
	{
		//�o�߃t���[�������J�E���g
		it->frame++;
		//���x�ɉ����x�����Z
		it->velocity = it->velocity + it->accel;
		//���x�ɂ��ړ�
		it->position = it->position + it->velocity;

		//�i�s�x��0~1�͈̔͂ŃX�P�[���v�Z
		float f = (float)it->frame / it->num_frame;
		//���`�⊮
		it->scale = (it->end_scale - it->start_scale)*f;
		it->scale += it->start_scale;
	}

	//���_�o�b�t�@�ւ̃f�[�^�]��
	HRESULT result = vertBuffer->Map(0,nullptr,(void**)&vertMap);
	if(SUCCEEDED(result)){
		//�p�[�e�B�N���̏���1�����f
		for(std::forward_list<Particle>::iterator it = particle.begin(); it != particle.end(); it++){
			//���W
			vertMap->pos = it->position;
			//�X�P�\��
			vertMap->scale = it->scale;
			//���̒��_
			vertMap++;
		}
		vertBuffer->Unmap(0,nullptr);
	}
}

void ParticleManager::Add(int life, Vector3 position, Vector3 velocity, Vector3 accel, float start_scale, float end_scale)
{
	//���X�g�ɗv�f��ǉ�
	particle.emplace_front();
	//�ǉ������v�f�̎Q��
	Particle& p = particle.front();
	//�l�̃Z�b�g
	p.position = position;
	p.velocity = velocity;
	p.accel = accel;
	p.start_scale = start_scale;
	p.end_scale = end_scale;
	p.num_frame = life;
}