#pragma once
#include "DirectionalLight.h"
#include "PointLight.h"

class LightGroup
{
private: // �G�C���A�X
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public:
	//���s�����̐�
	static const int DirectionLightNum = 3;
	//�_�����̐�
	static const int PointLightNum = 3;

public:
	//�萔�o�b�t�@�p�f�[�^�\����
	struct ConstBufferData
	{
		//�����̐F
		Vector3 ambientColor;
		float pad1;
		//���s�����p
		DirectionalLight::ConstBufferData dirLights[DirectionLightNum];
		//�_�����p
		PointLight::ConstBufferData pointLights[PointLightNum];
	};

//�ÓI�����o�֐�
public:
	/// <summary>
	/// �ÓI������
	/// </summary>
	/// <param name="device">�f�o�C�X</param>
	static void StaticInitialize(ID3D12Device* device);

	/// <summary>
	/// �C���X�^���X����
	/// </summary>
	/// <returns>�C���X�^���X</returns>
	static LightGroup* Create();


private: // �ÓI�����o�ϐ�
	// �f�o�C�X
	static ID3D12Device* device;

public: // �����o�֐�
	/// <summary>
	/// ������
	/// </summary>
	void Initialize();

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();
	
	/// <summary>
	/// �`��
	/// </summary>
	void Draw(ID3D12GraphicsCommandList* cmdList, UINT rootParameterIndex);

	/// <summary>
	/// �萔�o�b�t�@�]��
	/// </summary>
	void TransferConstBuffer();

	/// <summary>
	/// �W���̃��C�g�ݒ�
	/// </summary>
	void DefaultLightSetting();

	//Setter
	void SetAmbientColor(const Vector3& color);

	void SetDirLightActive(int index, bool active);
	void SetDirLightDir(int index, const DirectX::XMVECTOR& lightdir);
	void SetDirLightColor(int index, const Vector3& lightcolor);

	void SetPointLightActive(int index, bool active);
	void SetPointLightPos(int index, const Vector3& lightpos);
	void SetPointLightColor(int index, const Vector3& lightcolor);
	void SetPointLightAtten(int index, const Vector3& lightAtten);

private: // �����o�ϐ�
	// �萔�o�b�t�@
	ComPtr<ID3D12Resource> constBuff;

	// �����̐F
	Vector3 ambientColor = { 1,1,1 };

	// ���s�����̔z��
	DirectionalLight dirLights[DirectionLightNum];

	// �_�����̔z��
	PointLight pointLights[PointLightNum];

	// �_�[�e�B�t���O
	bool dirty = false;
};