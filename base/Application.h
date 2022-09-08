#pragma once
#include "Window.h"
#include "DirectXCommon.h"
#include "C:\Users\GSMAnager\Desktop\GorillaEngine\scene\SceneManager.h"

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
	SceneManager* sceneManager;
};