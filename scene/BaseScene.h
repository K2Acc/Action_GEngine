#pragma once
#include "../Engine/base/DirectXCommon.h"
#include "../Engine/base/Window.h"
#include "../Engine/input/Input.h"
#include "../camera/Camera.h"

#include "../Game/3D/SampleObjObject/SampleObjObject.h"

#include "../Game/Light/Lighting.h"

#ifdef _DEBUG
#include "../Engine/debugProcess/DebugText.h"
#include "../Engine/base/imguiManager.h"

#include <imgui.h>
#endif // _DEBUG


//�O���錾
class SceneManager;

class BaseScene
{
//�����o�֐�
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	BaseScene(DirectXCommon* dxCommon, Window* window);

	/// <summary>
	/// �N����
	/// </summary>
	virtual void Application();

	/// <summary>
	/// ������
	/// </summary>
	virtual void Initialize();

	/// <summary>
	/// �X�V
	/// </summary>
	virtual void Update();

	/// <summary>
	/// �`��
	/// </summary>
	virtual void Draw();

	/// <summary>
	/// �㏈��
	/// </summary>
	virtual void Finalize();

	//�V�[���J��
	virtual void NextSceneChange();

	/// <summary>
	/// ��X�V
	/// </summary>
	void EndUpdate();

#ifdef _DEBUG
	void SetDebugText(DebugText* text)	{this->debugText = text;}
	void SetImGui(imguiManager* imgui)	{this->imgui = imgui;}

	DebugText* GetDebugText()	{return debugText;}
	imguiManager* GetImGui()	{return imgui;}
#endif // _DEBUG

	//Setter
	void SetSceneManager(SceneManager* lsceneManager)	{sceneManager = lsceneManager;}

//�����o�ϐ�
protected:

#pragma region �ėp�@�\
	//�؂����
	DirectXCommon* dxCommon = nullptr;
	Window* window = nullptr;
	
	//�쐬
	Input* input = nullptr;
	Camera* camera = nullptr;

	//�؂蕨
	//�V�[���}�l�[�W���[
	SceneManager* sceneManager = nullptr;


	//skydome
    std::unique_ptr<SampleObjObject> skydome;

	//Light
	std::unique_ptr<Lighting> lightObject;

#ifdef _DEBUG
	//�؂蕨
	DebugText* debugText = nullptr;
	imguiManager* imgui = nullptr;
	bool show_demo_window = false;
#endif // _DEBUG

};

