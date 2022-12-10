#pragma once
#include "BaseScene.h"
#include "../Engine/light/LightGroup.h"

#include "../Game/3D/SampleParticleObject/SampleParticleObject.h"
#include "../Game/3D/SampleFbxObject/SampleFbxObject.h"
#include "../Game/3D/SampleObjObject/SampleObjObject.h"

#include "../Game/2D/SampleSprite/SampleSprite.h"

#include "../Game/Collision/CollisionPrimitive.h"

#ifdef _DEBUG
#include "../Engine/base/imguiManager.h"
#endif // _DEBUG


class SampleScene : public BaseScene
{
public:
	
	//�R���X�g���N�^
	SampleScene(DirectXCommon* dxCommon, Window* window);

	/// <summary>
	/// �N����
	/// </summary>
	void Application() override;

	/// <summary>
	/// ������
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// �X�V
	/// </summary>
	void Update() override;

	/// <summary>
	/// �`��
	/// </summary>
	void Draw() override;

	/// <summary>
	/// �㏈��
	/// </summary>
	void Finalize() override;

private:
	float time = 0.f;

	LightGroup* lightGroup = nullptr;

	//�ۉe
	float circleShadowDir[3] = { 0,-1,0 };
	float circleShadowAtten[3] = { 0.5f,0.6f,0.0f };
	float circleShadowFactorAngle[2] = { 0.0f, 0.5f };

	//�v���C���[���W�A�ۉe
	Vector3 fighterPos = { 1, 0.0f, 0 };

	//std::unique_ptr<SampleFbxObject> obj;

	//std::unique_ptr<SampleParticleObject> obj2;

	std::unique_ptr<SampleObjObject> obj3_1;
	std::unique_ptr<SampleObjObject> obj3_2;
	std::unique_ptr<SampleObjObject> obj3_3;
	std::unique_ptr<SampleObjObject> obj3_4;

	//std::unique_ptr<SampleSprite> sp;


	//�����蔻�� ��
	Sphere sphere;
	//�����蔻�� �O�p�`
	Triangle triangle;

#ifdef _DEBUG
	imguiManager* imgui;

	bool show_demo_window = false;
#endif // _DEBUG
};
