#pragma once
#include "BaseObjObject.h"
#include "Input.h"

#include "MeshCollider.h"

#include <optional>

#ifdef _DEBUG
#include "imguiManager.h"
#include <imgui.h>
#endif // _DEBUG



class Action : public BaseObjObject
{
private:
	enum class Behavior{
		kRoot,	//通常
		kAttack,//攻撃
	};

public:
	~Action();

	void Initialize();
	void Update(Camera* camera);
	void Draw();

private:
	void Rotation();
	void Movement();

	//通常行動
	void BehaviorRootInitialize();
	void BehaviorRootUpdate();
	void BehaviorRootDraw();
	//攻撃行動
	void BehaviorAttackInitialize();
	void BehaviorAttackUpdate();
	void BehaviorAttackDraw();

private:
	Input* input = nullptr;
	//状態
	Behavior behavior = Behavior::kRoot;
	std::optional<Behavior> behaviorRequest_ = std::nullopt;

	Vector3 moveValue = {};

	MeshCollider* meshCollider = nullptr;

	//武器
	BaseObjObject* weapon = nullptr;
	float weapon_easing = 0;

#ifdef _DEBUG
	imguiManager* imgui = nullptr;
#endif // _DEBUG

};

