#pragma once
#include "BaseScene.h"
#include "../Engine/light/LightGroup.h"

#include "../Game/3D/SampleParticleObject/SampleParticleObject.h"
#include "../Game/3D/SampleFbxObject/SampleFbxObject.h"
#include "../Game/3D/SampleObjObject/SampleObjObject.h"

#include "../Game/2D/SampleSprite/SampleSprite.h"

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
	float pointLightPos[3] = {0,0,0};
	float pointLightColor[3] = {1,1,1};
	float pointLightAtten[3] = {0.3f,0.3f,0.1f};

	//std::unique_ptr<SampleFbxObject> obj;

	//std::unique_ptr<SampleParticleObject> obj2;

	std::unique_ptr<SampleObjObject> obj3_1;
	std::unique_ptr<SampleObjObject> obj3_2;
	std::unique_ptr<SampleObjObject> obj3_3;
	std::unique_ptr<SampleObjObject> obj3_4;

	std::unique_ptr<SampleSprite> sp;


#ifdef _DEBUG
	imguiManager* imgui;

	bool show_demo_window = false;
#endif // _DEBUG
};
