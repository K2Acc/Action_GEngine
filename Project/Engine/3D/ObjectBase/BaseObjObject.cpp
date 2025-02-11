#include "BaseObjObject.h"
#include "BaseCollider.h"
#include "CollisionManager.h"
#include "Easing.h"

BaseObjObject::~BaseObjObject()
{
	//if(collider){
	//	//コリジョンマネージャーから登録を解除する
	//	CollisionManager::GetInstance()->RemoveCollider(collider);
	//	delete collider;
	//}
}

void BaseObjObject::Initialize(std::string filePath, bool IsSmmothing)
{
	model_ = new ObjModelManager();
	model_->CreateModel(filePath, IsSmmothing);
	object_ = ObjModelObject::Create(model_);
	world_.Initialize();
	world_.UpdateMatrix();

	//クラス名の文字列を取得
	name = typeid(*this).name();
}

void BaseObjObject::Initialize(ObjModelManager *model)
{
	isLendModel_ = true;
	this->model_ = model;
	object_ = ObjModelObject::Create(model);
	world_.Initialize();
	world_.UpdateMatrix();

	//クラス名の文字列を取得
	name = typeid(*this).name();
}


void BaseObjObject::Update(Camera *camera)
{
	this->camera_ = camera;
	world_.UpdateMatrix();
	object_->Update(world_, this->camera_);

	if(!isAlive_) return;
	//当たり判定更新
	if(baseCollider_){
		baseCollider_->Update();
	}
}

void BaseObjObject::Draw()
{
	if(!isAlive_) return;
	object_->Draw();

#ifdef _DEBUG
	if(baseCollider_ && isWireObjectDraw){
		wireObject_ = object_;
		wireObject_->TogglePipeline();
		wireObject_->Draw();
	}
#endif // _DEBUG
}

void BaseObjObject::Finalize()
{
	if(baseCollider_){
		//コリジョンマネージャーから登録を解除する
		CollisionManager::GetInstance()->RemoveCollider(baseCollider_);
		delete baseCollider_;
	}

	if(!isLendModel_){
		delete model_;
		model_ = nullptr;
	}

	delete object_;
	object_ = nullptr;

	world_ = {};
}

void BaseObjObject::ContactUpdate()
{
}

bool BaseObjObject::GetIsContactTrigger()
{
	if(isContactTrigger_){
		isContactTrigger_ = false;
		return true;
	}
	return false;
}

void BaseObjObject::SetPosition(const Vector3 &position)
{
	world_.translation = position;
	world_.UpdateMatrix();
}

void BaseObjObject::SetCollider(BaseCollider *collider)
{
	collider->SetObjObject(this);
	this->baseCollider_ = collider;
	//コリジョンマネージャーに登録
	CollisionManager::GetInstance()->AddCollider(collider);
	//行列の更新
	world_.UpdateMatrix();
	//コライダーを更新しておく
	collider->Update();
}
