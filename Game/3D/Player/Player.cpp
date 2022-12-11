#include "Player.h"
#include "../../../Engine/input/Input.h"
#include "../../Collision/SphereCollider.h"
#include "../SampleParticleObject/SampleParticleObject.h"

using namespace DirectX;

Player::~Player()
{
	Finalize();
}

void Player::Initialize(std::string filePath, bool IsSmoothing)
{
	BaseObjObject::Initialize(filePath, IsSmoothing);

	//�p�[�e�B�N��
	particle = std::make_unique<SampleParticleObject>();
	//�Փ˓_�Ƀp�[�e�B�N������
	for(int i = 0; i < 100; i++){
		const float md_vel = 0.1f;
		Vector3 vel{};
		vel.x = (float)rand() / RAND_MAX * md_vel - md_vel/2.0f;
		vel.y = (float)rand() / RAND_MAX * md_vel - md_vel/2.0f;
		vel.z = (float)rand() / RAND_MAX * md_vel - md_vel/2.0f;

		particle->Add(60, world.translation, vel, Vector3(), 0.0f, 1.0f);
	}
	particle->Initialize(1);

	//�R���C�_�[�̒ǉ�
	float radius = 1.0f;
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
    XMMATRIX matRot = XMMatrixRotationY(XMConvertToRadians(world.rotation.y));
    move = XMVector3TransformNormal(move, matRot);

	//�����Ă�������Ɉړ�
	if(input->Push(DIK_DOWN)){
		world.translation.x -= move.m128_f32[0];
		world.translation.y -= move.m128_f32[1];
		world.translation.z -= move.m128_f32[2];
	}
	else if(input->Push(DIK_UP)){
		world.translation.x += move.m128_f32[0];
		world.translation.y += move.m128_f32[1];
		world.translation.z += move.m128_f32[2];
	}


	this->camera = camera;

	particle->Update(this->camera);
	BaseObjObject::Update(this->camera);
}

void Player::Draw3D()
{
	BaseObjObject::Draw();
}

void Player::Draw2D()
{
	particle->Draw();
}

void Player::Finalize()
{
	particle->Finalize();
	BaseObjObject::Finalize();
}

void Player::OnCollision(const CollisionInfo &info)
{
	////�Փ˓_�Ƀp�[�e�B�N������
	//for(int i = 0; i < 100; i++){
	//	const float md_vel = 0.1f;
	//	Vector3 vel{};
	//	vel.x = (float)rand() / RAND_MAX * md_vel - md_vel/2.0f;
	//	vel.y = (float)rand() / RAND_MAX * md_vel - md_vel/2.0f;
	//	vel.z = (float)rand() / RAND_MAX * md_vel - md_vel/2.0f;

	//	particle->Add(10, Vector3{info.inter.m128_f32[0], info.inter.m128_f32[1], info.inter.m128_f32[2]}, vel, Vector3(), 0.0f, 1.0f);
	//}
}

