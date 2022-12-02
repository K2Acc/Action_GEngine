#pragma once
#include <wrl.h>
#include <DirectXMath.h>
#include <d3d12.h>
#include <d3dx12.h>

#include "../math//Vector/Vector2.h"
#include "../math//Vector/Vector3.h"
#include "../math//Vector/Vector4.h"
#include "../base/DirectXCommon.h"

class Light
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

//�T�u�N���X
public:
	struct ConstBufferData
	{
		DirectX::XMVECTOR lightv;	//���C�g����
		Vector3 lightcolor;			//���C�g�F
	};

//�ÓI�����o�֐�
public:
	static Light* Create();

	static void StaticInitialize(DirectXCommon* dxCommon);

//�����o�֐�
public:
	void Initialize();

	void Update();

	void Draw(UINT rootParameterIndex);

	void TransferConstBuffer();

	//Setter
	void SetLightDir(const DirectX::XMVECTOR& lightdir);
	void SetLightColor(const Vector3 lightcolor);

//�ÓI�����o�ϐ�
private:
	static DirectXCommon* dxCommon;

//�����o�ϐ�
private:
	//�萔�o�b�t�@
	ComPtr<ID3D12Resource> constBuff;
	//���C�g��������
	DirectX::XMVECTOR lightdir = {1,0,0,0};
	//���C�g�F
	Vector3 lightcolor = {1,1,1};
	//�_�[�e�B�t���O
	bool IsDirty = false;
};

