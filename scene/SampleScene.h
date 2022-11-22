#pragma once
#include "BaseScene.h"
#include "../Game/3D/SampleFbxObject/SampleFbxObject.h"
#include "../Game/2D/SampleSprite/SampleSprite.h"

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

	std::unique_ptr<SampleFbxObject> obj;
	std::unique_ptr<SampleSprite> sp;
};

