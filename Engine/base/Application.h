#pragma once
#include "Window.h"
#include "DirectXCommon.h"

#include "TextureManager.h"


#include "ParticleManager.h"
#include "../Engine/3D/ParticleObject.h"

#include "../Engine/loader/FbxLoader.h"
#include "FbxModelManager.h"
#include "../Engine/3D/FbxModelObject.h"


#include "BaseScene.h"
#include "../Engine/2D/Sprite.h"




class Application
{
private:
	//�V���O���g��
	static Application* app;

	Application();
	~Application();

public:
	//�V���O���g��(Application�𕡐����Ӗ��͂Ȃ����߁A�P��̑���)
	static Application* GetInstance();
	static void Delete();

	void Initialize();
	void Run();
	void Finalize();

private:
	void Update();
	void Draw();

private:
#pragma region �ėp�@�\
	Window* window;
	DirectXCommon* dxCommon;
#pragma endregion

	std::unique_ptr<BaseScene> scene;
	std::string sceneName = "";
};