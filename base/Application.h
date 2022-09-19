#pragma once
#include "Window.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "SceneManager.h"

#include "TextureManager.h"
#include "Sprite.h"

#include "ViewProjection.h"
#include "WorldTransform.h"
#include "GeometryObjectManager.h"
#include "GeometryObject.h"



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
	Window* window;
	DirectXCommon* dxCommon;
	Input* input;

	Sprite* sprite = nullptr;

	GeometryObject* object = nullptr;
	WorldTransform worldTransform;
	ViewProjection viewProjection;
};