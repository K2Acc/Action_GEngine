﻿#pragma once
#include "Window.h"
#include "DirectXCommon.h"
#include "Vector4.h"
#include "Vector3.h"
#include "Vector2.h"

#include <wrl.h>
#include <d3d12.h>

class imguiManager
{
public:
	//初期化
	void Initialize(Window* winApp, DirectXCommon* dxCommon);

	//ImGui受付開始
	void Begin();

	//ImGui受付終了
	void End();

	//描画
	void Draw();

	//後処理
	void Finalize();

	Vector4 ImGuiDragVector4(const char* name, Vector4 value, float speed = 1.0f, float min = -100, float max = 100);
	Vector3 ImGuiDragVector3(const char* name, Vector3 value, float speed = 1.0f, float min = -100, float max = 100);
	Vector2 ImGuiDragVector2(const char* name, Vector2 value, float speed = 1.0f, float min = -100, float max = 100);
	float ImGuiDragFloat(const char* name, float* value, float speed = 1.0f, float min = -100, float max = 100);
	int ImGuiDrawInt(const char* name, int* value, float speed = 1, int min = -100, int max = 100);

private:
	Window* winApp = nullptr;
	DirectXCommon* dxCommon = nullptr;

	//SRV用デスクリプタヒープ
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> srvHeap;
};

