#pragma once

#include <wrl.h>
#include <d3d12.h>
#include <vector>
#include <d3dx12.h>
#include <string>
#include <unordered_map>

#include "../math/Vector/Vector2.h"
#include "../math/Vector/Vector3.h"
#include "../math/Vector/Vector4.h"
#include "DirectXCommon.h"

class ObjModelManager{
private: // �G�C���A�X
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public:
	// ���_�f�[�^�\����
	struct VertexPosNormalUv
	{
		Vector3 pos; // xyz���W
		Vector3 normal; // �@���x�N�g��
		Vector2 uv;  // uv���W
	};

	//�}�e���A��
	struct Material
	{
		std::string name;//�}�e���A����
		Vector3 ambient;
		Vector3 diffuse;
		Vector3 specular;
		float alpha;
		std::string textureFilename;	//�e�N�X�`���t�@�C��
		//�R���X�g���N�^
		Material(){
			ambient = {0.3f, 0.3f, 0.3f};
			diffuse = {0.0f, 0.0f, 0.0f};
			specular = {0.0f, 0.0f, 0.0f};
			alpha = 1.0f;
		}
	};

public:
	static void StaticInitialize(DirectXCommon* dxCommon_);

public:
	~ObjModelManager();

	void CreateModel(std::string filePath, bool smmothing = false);

	void Draw(ID3D12GraphicsCommandList* commandList);

	/// <summary>
	/// �G�b�W���R���f�[�^�̒ǉ�
	/// </summary>
	/// <param name="indexPosition">���W�C���f�b�N�X</param>
	/// <param name="indexVertex">���_�C���f�b�N�X</param>
	void AddSmmpthData(unsigned short indexPosition, unsigned short indexVertex);

	/// <summary>
	/// ���R�����ꂽ���_�@���̌v�Z
	/// </summary>
	void CalculateSmoothedVertexNormals();

	//Getter
	Material GetMaterial()	{return material;}

	/// <summary>
	/// ���_�f�[�^�����擾
	/// </summary>
	/// <returns>���_�f�[�^�̐�</returns>
	inline size_t GetVertexCount()	{return vertices.size();}

private:
	/// <summary>
	/// �e�N�X�`���ǂݍ���
	/// </summary>
	void LoadTexture(const std::string& directoryPath, const std::string& filename);

	/// <summary>
	/// �}�e���A���ǂݍ���
	/// </summary>
	void LoadMaterial(const std::string& directoryPath, const std::string& filename);

private:

private:
	// �f�o�C�X
	static DirectXCommon* dxCommon;

private:
	// ���_�f�[�^�z��
	std::vector<VertexPosNormalUv> vertices;
	// ���_�C���f�b�N�X�z��
	std::vector<unsigned short> indices;

	// ���_�o�b�t�@
	ComPtr<ID3D12Resource> vertBuff;
	// �C���f�b�N�X�o�b�t�@
	ComPtr<ID3D12Resource> indexBuff;
	// �e�N�X�`���o�b�t�@
	ComPtr<ID3D12Resource> texbuff;

	// ���_�o�b�t�@�r���[
	D3D12_VERTEX_BUFFER_VIEW vbView;
	// �C���f�b�N�X�o�b�t�@�r���[
	D3D12_INDEX_BUFFER_VIEW ibView;

	// �f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> descHeap;

	//�}�e���A��
	Material material;

	//���_�p�@���X���[�V���O�p�f�[�^
	std::unordered_map<unsigned short, std::vector<unsigned short>> smoothData;
};