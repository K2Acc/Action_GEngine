#include "Action/Action.h"
#include "Utility.h"
#include <DirectXMath.h>
#include <cmath>

#include "CollisionAttribute.h"
#include "CollisionManager.h"

#include "Easing.h"

using namespace std;
using namespace DirectX;

Action::~Action()
{
	CollisionManager::GetInstance()->RemoveCollider(meshCollider);
	
	weapon->Finalize();
	delete weapon;
}

void Action::Initialize()
{
	input = Input::GetInstance();

	BaseObjObject::Initialize("sphere", true);

	meshCollider = new MeshCollider();
	SetCollider(meshCollider);
	meshCollider->SetAttribute(COLLISION_ATTR_ALLIES);
	meshCollider->ConstructTriangles(model_);

	SetPosition({0,0,-3});


	weapon = new BaseObjObject();
	weapon->Initialize("weapon", false);
}

void Action::Update(Camera *camera)
{
	this->camera_ = camera;

	//状態行為
	switch (behavior)
	{
	case Action::Behavior::kRoot:
	default:
		BehaviorRootUpdate();
		break;
	case Action::Behavior::kAttack:
		BehaviorAttackUpdate();
		break;
	}

	//状態遷移
	if(behaviorRequest_){
		//ふるまい変更
		behavior = behaviorRequest_.value();
		//書くふるまいごとに初期化実行
		switch (behavior)
		{
		case Action::Behavior::kRoot:
		default:
			BehaviorRootInitialize();
			break;
		case Action::Behavior::kAttack:
			BehaviorAttackInitialize();
			break;
		}
		//ふるまリクエストをリセット
		behaviorRequest_ = std::nullopt;
	}

	BaseObjObject::Update(this->camera_);
}

void Action::Draw()
{
	BaseObjObject::Draw();

	//状態行為
	switch (behavior)
	{
	case Action::Behavior::kRoot:
	default:
		BehaviorRootDraw();
		break;
	case Action::Behavior::kAttack:
		BehaviorAttackDraw();
		break;
	}
}


void Action::Rotation()
{
	//回転
	if(moveValue == Vector3{0,0,0}) return;
	//解法2
	world_.rotation.y = std::atan2(moveValue.x, moveValue.z);
	Vector3 velocityXZ = Vector3{moveValue.x, 0, moveValue.z};
	world_.rotation.x = std::atan2(-moveValue.y, velocityXZ.length());
}

void Action::Movement()
{
	if(!input->GetIsPadConnect()) return;

	float speed = 0.5f;
	moveValue = {input->PadLStick().x, 0, input->PadLStick().y};

	moveValue = moveValue.normalize() * speed;

	//カメラの方向へと動く
	XMMATRIX matRot;
	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(camera_->GetWorld().rotation.z);
	matRot *= XMMatrixRotationX(0);
	matRot *= XMMatrixRotationY(camera_->GetWorld().rotation.y);
	moveValue = TransformNormal(moveValue, matRot);

	//移動
	world_.translation+=moveValue;
}


void Action::BehaviorRootInitialize()
{
}

void Action::BehaviorRootUpdate()
{
	Rotation();
	Movement();

	if(input->PadButtonPush(XINPUT_GAMEPAD_A)){
		behaviorRequest_ = Behavior::kAttack;
	}
}

void Action::BehaviorRootDraw()
{
}


void Action::BehaviorAttackInitialize()
{
	weapon->SetRotation({-0.5f,0,0});
	weapon_easing = 0;
}

void Action::BehaviorAttackUpdate()
{
	float time = Time_OneWay(weapon_easing, 0.25f);

	//武器回転
	Vector3 weaponRot = (Vector3)Easing_Point3_Linear(Vector3{0,0,0}, Vector3{-0.25f,0,0}, Vector3{1.5f,0,0}, time);
	weaponRot += GetRotation();
	weapon->SetRotation(weaponRot);
	weapon->SetPosition(GetPosition());
	weapon->Update(this->camera_);

	//攻撃移動
	Vector3 playerMove = (Vector3)Easing_Point2_Linear(Vector3{0,0,0}, Vector3{0,0,0.01f}, time);
	//カメラの方向へと動く
	XMMATRIX matRot;
	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(world_.rotation.z);
	matRot *= XMMatrixRotationX(0);
	matRot *= XMMatrixRotationY(world_.rotation.y);
	playerMove = TransformNormal(playerMove, matRot);
	//移動
	world_.translation+=playerMove;

#ifdef _DEBUG
	{
		//座標
		ImGui::SetNextWindowPos(ImVec2{0,160});
		//サイズ
		ImGui::SetNextWindowSize(ImVec2{300,100});
		ImGui::Begin(weapon->GetName());
		weapon->SetPosition(imgui->ImGuiDragVector3("Pos", weapon->GetPosition(),0.1f));
		weapon->SetRotation(imgui->ImGuiDragVector3("Rot", weapon->GetRotation(), 0.1f));
		weapon->SetScale(imgui->ImGuiDragVector3("Sca", weapon->GetScale(),0.1f));

		imgui->ImGuiDragFloat("Easing", &weapon_easing, 0.01f, 0, 1);

		ImGui::End();
	}
#endif // _DEBUG

	if(weapon_easing >= 1.0f){
		behaviorRequest_ = Behavior::kRoot;
	}
}

void Action::BehaviorAttackDraw()
{
	weapon->Draw();
}
