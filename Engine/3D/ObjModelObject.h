#pragma once

#include <DirectXMath.h>
#include <string>

#include "WorldTransform.h"
#include "../../camera/Camera.h"
#include "../base/ObjModelManager.h"
#include "../base/DirectXCommon.h"
#include "../light/DirectionalLight.h"


/// <summary>
/// 3Dオブジェクト
/// </summary>
class ObjModelObject
{
private: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMMATRIX = DirectX::XMMATRIX;

public: // サブクラス

	class CommonObj{
		friend class ObjModelObject;
	public:
		 void InitializeGraphicsPipeline();

	private:
		DirectXCommon* dxCommon;
		// ルートシグネチャ
		ComPtr<ID3D12RootSignature> rootsignature;
		// パイプラインステートオブジェクト
		ComPtr<ID3D12PipelineState> pipelinestate;
		//ライト
		DirectionalLight* light;
	};

	// 定数バッファ用データ構造体
	struct ConstBufferDataB0
	{
		XMMATRIX viewproj;		//ビュープロジェクション
		XMMATRIX world;			//ワールド
		Vector3 cameraPos;		//カメラ座標
	};
	struct ConstBufferDataB1
	{
		Vector3 ambient;	//アンビエント係数
		float pad1;	//パディング
		Vector3 diffuse;	//ディヒューズ係数
		float pad2;	//パディング
		Vector3 specular;	//スペキュラ係数
		float alpha;	//アルファ
	};


public: // 静的メンバ関数
	static void StaticInitialize( DirectXCommon* dxCommon);
	static void StaticFinalize();


	/// <summary>
	/// 3Dオブジェクト生成
	/// </summary>
	/// <returns></returns>
	static ObjModelObject* Create(ObjModelManager* model);

	//Setter
	static void SetLight(DirectionalLight* light)	{common->light = light;}


private: // 静的メンバ変数
	static CommonObj* common;

public: // メンバ関数

	ObjModelObject(ObjModelManager* model);

	/// <summary>
	/// 初期化
	/// </summary>
	bool Initialize();
	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update(WorldTransform world, Camera* camera);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private: // メンバ変数
	ObjModelManager* model = nullptr;

	ComPtr<ID3D12Resource> constBuffB0; // 定数バッファ
	ComPtr<ID3D12Resource> constBuffB1; // 定数バッファ
	// 色
	Vector4 color = { 1,1,1,1 };
};

