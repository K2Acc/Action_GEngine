#pragma once
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <DirectXMath.h>
#include <forward_list>

#include "../Engine/math/Vector/Vector2.h"
#include "../Engine/math/Vector/Vector3.h"

class ParticleManager
{
public://�G�C���A�X
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public:	//�萔
	//�e�N�X�`���̍ő喇��
	static const int maxObjectCount = 512;
	//���_��
	static const int vertexCount = 1024;
public:
	struct VertexPos{
		Vector3 pos;
		float scale;
	};

	//�p�[�e�B�N����
	struct Particle
	{
		//���W
		Vector3 position = {};
		//���x
		Vector3 velocity = {};
		//�����x
		Vector3 accel = {};

		//�X�P�[��
		float scale = 1.f;
		//�����l
		float start_scale = 1.f;
		//�ŏI�l
		float end_scale = 0.f;

		//���݂̃t���[��
		int frame = 0;
		//�I���t���[��
		int num_frame = 0;
	};

public:
	static ParticleManager* GetInstance();

public:
	/// <summary>
	/// �o�b�t�@����
	/// </summary>
	void CreateBuffer();

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �p�[�e�B�N���ǉ�
	/// </summary>
	/// <param name="life">��������</param>
	/// <param name="position">�������W</param>
	/// <param name="velocity">���x</param>
	/// <param name="accel">�����x</param>
	/// <param name="start_scale">�J�n���X�P�[��</param>
	/// <param name="end_scale">�I�����X�P�[��</param>
	void Add(int life, Vector3 position, Vector3 velocity, Vector3 accel, float start_scale, float end_scale);

	//Get
	int GetMaxObjectCount()	{return maxObjectCount;}
	//���_���
	int Getvertices()	{return _countof(vertices);}
	//���_�o�b�t�@�r���[
	D3D12_VERTEX_BUFFER_VIEW GetvbView()	{return vbView;}
	//�p�[�e�B�N���`�搔
	UINT GetParticle()	{return (UINT)std::distance(particle.begin(), particle.end());}

private:
	//���_�o�b�t�@
	ComPtr<ID3D12Resource> vertBuffer;
	//���_�}�b�v
	VertexPos* vertMap = nullptr;

	//���_�o�b�t�@�r���[
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	//���_�f�[�^
	VertexPos vertices[vertexCount];

	//�p�[�e�B�N���z��
	std::forward_list<Particle> particle;
};

