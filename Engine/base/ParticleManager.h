#pragma once
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <DirectXMath.h>


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
	static const int vertexCount = 30;
public:
	struct VertexPos{
		Vector3 pos;
	};

public:
	static ParticleManager* GetInstance();

public:
	/// <summary>
	/// �o�b�t�@����
	/// </summary>
	void CreateBuffer();

	//Get
	int GetMaxObjectCount()	{return maxObjectCount;}
	//���_���
	int Getvertices()	{return _countof(vertices);}
	//���_�o�b�t�@�r���[
	D3D12_VERTEX_BUFFER_VIEW GetvbView()	{return vbView;}


private:
	//���_�o�b�t�@
	ComPtr<ID3D12Resource> vertBuffer;
	//���_�}�b�v
	VertexPos* vertMap = nullptr;

	//���_�o�b�t�@�r���[
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	//���_�f�[�^
	VertexPos vertices[vertexCount];
};

