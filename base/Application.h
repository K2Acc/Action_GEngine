#pragma once
#include "Window.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "SceneManager.h"

#include "TextureManager.h"
#include "Sprite.h"

#include "Camera.h"
#include "WorldTransform.h"

#include "GeometryObjectManager.h"
#include "GeometryObject.h"

#include "FbxLoader.h"
#include "FbxModelManager.h"
#include "FbxModelObject.h"

#ifdef _DEBUG
#include "SceneStopper.h"
#endif // _DEBUG




class Application
{
private:
	//シングルトン
	static Application* app;

	Application();
	~Application();

public:
	//シングルトン(Applicationを複数持つ意味はないため、単一の存在)
	static Application* GetInstance();
	static void Delete();

	void Initialize();
	void Run();
	void Finalize();

private:
	void Update();
	void Draw();

private:
#pragma region 汎用機能
	Window* window;
	DirectXCommon* dxCommon;
	Input* input;
	Camera* camera = nullptr;
#pragma endregion

#pragma region スプライト関連

	Sprite* sprite = nullptr;

#pragma endregion

#pragma region オブジェクト関連
	GeometryObject* object = nullptr;
	WorldTransform worldTransform;


	std::unique_ptr<FbxModelManager> modelFbx;
	FbxModelObject* objectFbx = nullptr;
	WorldTransform worldTransforFbx;
#pragma endregion


#ifdef _DEBUG
	SceneStopper* sceneStopper;
#endif // _DEBUG
};