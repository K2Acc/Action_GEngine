#pragma once
#include "DirectionalLight.h"
#include "PointLight.h"

class LightGroup
{
public:
	//���s�����̐�
	static const int DirectionLightNum = 1;
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

//�����o�֐�
public:
	
	//Setter
	void SetPointLightActive(int index, bool IsActive);
	void SetPointLightPos(int index, const Vector3& lightpos);
	void SetPointLightColor(int index, const Vector3& lightcolor);
	void SetPointLightAtten(int index, const Vector3& lightAtten);

//�����o�ϐ�
private:
	//�_�����̔z��
	PointLight pointLights[PointLightNum];
};

