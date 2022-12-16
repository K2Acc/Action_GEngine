#include "Player.h"
#include "../../../Engine/input/Input.h"
#include "../../Collision/SphereCollider.h"
#include "../../../Engine/base/ParticleManager.h"

using namespace DirectX;

Player::~Player()
{
	Finalize();
}

void Player::Initialize(std::string filePath, bool IsSmoothing)
{
	BaseObjObject::Initialize(filePath, IsSmoothing);

	//�R���C�_�[�̒ǉ�
	float radius = 0.6f;
	//���a�������������畂�������W�����̒��S�ɂ���
	SetCollider(new SphereCollider(XMVECTOR{0,radius,0,0}, radius));
}

void Player::Update(Camera *camera)
{
	//����
	Input* input = Input::GetInstance();
	//A,D�Ő���
	if(input->Push(DIK_LEFT)){
		world.rotation.y -= 0.2f;
	}
	else if(input->Push(DIK_RIGHT)){
		world.rotation.y += 0.2f;
	}

	// �ړ��x�N�g����Y������̊p�x�ŉ�]
    XMVECTOR move = { 0,0,0.1f,0 };
    XMMATRIX matRot = XMMatrixRotationY(world.rotation.y);
    move = XMVector3TransformNormal(move, matRot);

	//�����Ă�������Ɉړ�
	if(input->Push(DIK_DOWN)){
		world.translation.x -= move.m128_f32[0]*4.f;
		world.translation.y -= move.m128_f32[1]*4.f;
		world.translation.z -= move.m128_f32[2]*4.f;
	}
	else if(input->Push(DIK_UP)){
		world.translation.x += move.m128_f32[0]*4.f;
		world.translation.y += move.m128_f32[1]*4.f;
		world.translation.z += move.m128_f32[2]*4.f;
	}

	this->camera = camera;

	BaseObjObject::Update(this->camera);
}

void Player::Draw3D()
{
	BaseObjObject::Draw();
}


void Player::Finalize()
{
	BaseObjObject::Finalize();
}

void Player::OnCollision(const CollisionInfo &info)
{
	//�Փ˓_�Ƀp�[�e�B�N������
	for(int i = 0; i < 1; i++){
		Vector3 pos{};
		pos.x = info.inter.m128_f32[0];
		pos.y = info.inter.m128_f32[1];
		pos.z = info.inter.m128_f32[2];
		
		const float md_vel = 0.1f;
		Vector3 vel{};
		vel.x = (float)rand() / RAND_MAX * md_vel - md_vel/2.0f;
		vel.y = (float)rand() / RAND_MAX * md_vel - md_vel/2.0f;
		vel.z = (float)rand() / RAND_MAX * md_vel - md_vel/2.0f;

		Vector3 acc{};
		acc.x = 0.f;
		acc.y = 0.f;
		acc.z = 0.f;

		ParticleManager::GetInstance()->Add(10, pos, vel, acc, 0.0f, 1.0f, 2);
	}
}

