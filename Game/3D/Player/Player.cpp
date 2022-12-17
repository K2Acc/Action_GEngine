#include "Player.h"
#include "../../../Engine/input/Input.h"
#include "../../Collision/SphereCollider.h"
#include "../../../Engine/base/ParticleManager.h"

#include "../../Collision/CollisionSystem/CollisionManager.h"
#include "../../Collision/CollisionSystem/CollisionAttribute.h"

using namespace DirectX;

class PlayerQueryCallback : public QueryCallback
{
public:
	PlayerQueryCallback(Sphere* sphere): sphere(sphere){};

	//�Փˎ��R�[���o�b�N�֐�
	bool OnQueryHit(const QueryHit& info){
		//���[���h�̏����
		const XMVECTOR up = {0,1,0,0};
		//�r�˕���
		XMVECTOR rejectDir = XMVector3Normalize(info.reject);
		//������Ɣr�˕����̊p�x���̃R�T�C���l
		float cos = XMVector3Dot(rejectDir, up).m128_f32[0];

		//�n�ʔ���臒l�p�x
		const float threshold = cosf(XMConvertToRadians(30.0f));	//30�x�ȏ���z���ƕǂƂ��ĔF������
		//�p�x���ɂ���ēV��܂��͒n�ʂƔ��肳���ꍇ��������
		if(-threshold < cos && cos < threshold){
			//����r��(�����o��)
			sphere->center += info.reject;
			move += info.reject;
		}
		return true;
	}

	//�N�G���\�Ɏg�p���鋅
	Sphere* sphere = nullptr;
	//�r�˂ɂ��ړ���(�ݐϒn)
	DirectX::XMVECTOR move = {};
};

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

	//�����蔻�葮��
	collider->SetAttribute(COLLISION_ATTR_ALLIES);
}

void Player::Update(Camera *camera)
{
	this->camera = camera;

	//����
	Input* input = Input::GetInstance();
	//A,D�Ő���
	if(input->Push(DIK_LEFT)){
		world.rotation.y -= 0.1f;
	}
	else if(input->Push(DIK_RIGHT)){
		world.rotation.y += 0.1f;
	}

	// �ړ��x�N�g����Y������̊p�x�ŉ�]
    XMVECTOR move = { 0,0,0.1f,0};
    XMMATRIX matRot = XMMatrixRotationY(world.rotation.y);
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

	//��������
	if(!IsGround){
		//�����������x
		const float fallAcc = -0.01f;
		const float fallVYMin = -0.5f;
		//����
		fallV.y = max(fallV.y + fallAcc, fallVYMin);
		//�ړ�
		world.translation.x += fallV.x;
		world.translation.y += fallV.y;
		world.translation.z += fallV.z;
	}
	//�W�����v����
	else if(input->Trigger(DIK_SPACE)){
		IsGround = false;
		const float jumpVYFist = 0.2f;	//�W�����v�����������
		fallV = {0,jumpVYFist, 0};
	}

	//���R���C�_�[�擾
	SphereCollider* sphereCollider = dynamic_cast<SphereCollider*>(collider);
	assert(sphereCollider);


	//�N�G���\�R�T�C���̊֐��I�u�W�F�N�g
	PlayerQueryCallback callback(sphereCollider);
	//���ƒn�`�̌�����S����
	CollisionManager::GetInstance()->QuerySphere(*sphereCollider, &callback, COLLISION_ATTR_LANDSHAPE);
	//�����ɂ��r�˕�������
	world.translation.x += callback.move.m128_f32[0];
	world.translation.y += callback.move.m128_f32[1];
	world.translation.z += callback.move.m128_f32[2];


	//�s��A�J�����X�V
	BaseObjObject::Update(this->camera);
	//�R���C�_�[�X�V
	collider->Update();


	//���̏�[���狅�̉��[�܂ł̗�L���X�g�p���C������
	Ray ray;
	ray.start = sphereCollider->center;
	ray.start.m128_f32[1] += sphereCollider->GetRadius();
	ray.dir = {0,-1,0,0};
	RaycastHit raycastHit;

	//�ڒn���
	if(IsGround){
		//�X���[�Y�ɍ������ׂ̋z������
		const float adsDistance = 0.2f;
		//�ڒn���ێ�
		if(CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_LANDSHAPE, &raycastHit, sphereCollider->GetRadius() * 2.0f + adsDistance)){
			IsGround = true;
			world.translation.y -= (raycastHit.distance - sphereCollider->GetRadius()*2.0f);
			//�s��̍X�V�Ȃ�
			BaseObjObject::Update(this->camera);
		}
		//�n�ʂ��Ȃ��̂ŗ���
		else{
			IsGround = false;
			fallV = {};
		}
	}
	//�������
	else if(fallV.y <= 0.0f){
		if(CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_LANDSHAPE, &raycastHit, sphereCollider->GetRadius()*2.0f)){
			//���n
			IsGround = true;
			world.translation.y -= (raycastHit.distance - sphereCollider->GetRadius()*2.0f);
			//�s��̍X�V�Ȃ�
			BaseObjObject::Update(this->camera);
		}
	}
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

