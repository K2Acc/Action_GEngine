#pragma once

#include <DirectXMath.h>
#include <string>

#include "WorldTransform.h"
#include "../../camera/Camera.h"
#include "../base/ObjModelManager.h"
#include "../base/DirectXCommon.h"


/// <summary>
/// 3D�I�u�W�F�N�g
/// </summary>
class ObjModelObject
{
private: // �G�C���A�X
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMMATRIX = DirectX::XMMATRIX;

public: // �T�u�N���X

	class CommonObj{
		friend class ObjModelObject;
	public:
		 void InitializeGraphicsPipeline();

	private:
		DirectXCommon* dxCommon;
		// ���[�g�V�O�l�`��
		ComPtr<ID3D12RootSignature> rootsignature;
		// �p�C�v���C���X�e�[�g�I�u�W�F�N�g
		ComPtr<ID3D12PipelineState> pipelinestate;
	};

	// �萔�o�b�t�@�p�f�[�^�\����
	struct ConstBufferDataB0
	{
		XMMATRIX mat;	// �R�c�ϊ��s��
	};
	struct ConstBufferDataB1
	{
		Vector3 ambient;	//�A���r�G���g�W��
		float pad1;	//�p�f�B���O
		Vector3 diffuse;	//�f�B�q���[�Y�W��
		float pad2;	//�p�f�B���O
		Vector3 specular;	//�X�y�L�����W��
		float alpha;	//�A���t�@
	};


public: // �ÓI�����o�֐�
	static void StaticInitialize( DirectXCommon* dxCommon);
	static void StaticFinalize();


	/// <summary>
	/// 3D�I�u�W�F�N�g����
	/// </summary>
	/// <returns></returns>
	static ObjModelObject* Create(ObjModelManager* model);


private: // �ÓI�����o�ϐ�
	static CommonObj* common;

public: // �����o�֐�

	ObjModelObject(ObjModelManager* model);

	/// <summary>
	/// ������
	/// </summary>
	bool Initialize();
	/// <summary>
	/// ���t���[������
	/// </summary>
	void Update(WorldTransform world, Camera* camera);

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

private: // �����o�ϐ�
	ObjModelManager* model = nullptr;

	ComPtr<ID3D12Resource> constBuffB0; // �萔�o�b�t�@
	ComPtr<ID3D12Resource> constBuffB1; // �萔�o�b�t�@
	// �F
	Vector4 color = { 1,1,1,1 };
};
