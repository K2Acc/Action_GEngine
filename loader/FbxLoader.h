#pragma once

#include "fbxsdk.h"

#include <d3d12.h>
#include <d3dx12.h>

#include <string>
#include "FbxModelManager.h"

class FbxLoader
{
/// <summary>
/// �G�C���A�X
/// </summary>
private:
	using string = std::string;

/// <summary>
/// �萔
/// </summary>
public:
	//���f���i�[���[�g�p�X
	static const string baseDirectory;

	//�e�N�X�`�����Ȃ��ꍇ�̕W���e�N�X�`���t�@�C����
	static const string defaultTextureFileName;

public:
	/// <summary>
	/// �V���O���g���C���X�^���X�̎擾
	/// </summary>
	/// <returns>�C���X�^���X</returns>
	static FbxLoader* GetInstance();

	/// <summary>
	/// FBX�̍s���XMAtrix�ɕϊ�
	/// </summary>
	/// <param name="dst">�������ݐ�</param>
	/// <param name="src">���ƂƂȂ�FBX�s��</param>
	static void ConvertMatrixFromFbx(DirectX::XMMATRIX* dst, const FbxAMatrix& src);

private:
	// private�ȃR���X�g���N�^�i�V���O���g���p�^�[���j
	FbxLoader() = default;
	// private�ȃf�X�g���N�^�i�V���O���g���p�^�[���j
	~FbxLoader() = default;
	// �R�s�[�R���X�g���N�^���֎~�i�V���O���g���p�^�[���j
	FbxLoader(const FbxLoader& obj) = delete;
	// �R�s�[������Z�q���֎~�i�V���O���g���p�^�[���j
	void operator=(const FbxLoader& obj) = delete;

public:
	/// <summary>
	/// ������
	/// </summary>
	/// <param name="device">�f�o�C�X</param>
	void Initialize(ID3D12Device* device);

	/// <summary>
	/// �t�@�C������FBX���f���Ǎ�
	/// </summary>
	/// <param name="modelName">���f����</param>
	FbxModelManager* LoadModeFromFile(const string& modelName);

	/// <summary>
	/// ���
	/// </summary>
	void Finalize();

private:
	/// <summary>
	/// �ċA�I�Ƀm�[�h�\�������
	/// </summary>
	/// <param name="model">�Ǎ��惂�f���I�u�W�F�N�g</param>
	/// <param name="fbxNode">��͑Ώۂ̃m�[�h</param>
	void ParseNodeRecursive(FbxModelManager* model,FbxNode* fbxNode, Node* parent = nullptr);

	/// <summary>
	/// ���b�V���ǂݎ��
	/// </summary>
	/// <param name="model">�ǂݍ��ݐ惂�f���I�u�W�F�N�g</param>
	/// <param name="fbxNode">��͑Ώۂ̃m�[�h</param>
	void ParseMesh(FbxModelManager* model, FbxNode* fbxNode);

	//���_���W�ǂݎ��
	void ParseMeshVertices(FbxModelManager* model, FbxMesh* fbxMesh);

	//�ʏ��ǂݎ��
	void ParseMeshFaces(FbxModelManager* model,FbxMesh* fbxMesh);

	//�}�e���A���ǂݎ��
	void ParseMaterial(FbxModelManager* model, FbxNode* fbxNode);

	//�e�N�X�`���ǂݍ���
	void LoadTexture(FbxModelManager* model, const std::string& fullpath);

	//�X�L�j���O���̓ǂݎ��
	void ParseSkin(FbxModelManager* model,FbxMesh* fbxMesh);

	//�f�B���N�g�����܂񂾃t�@�C���p�X����t�@�C�����𒊏o����
	std::string ExtractFileName(const std::string& path);

private:
	//�f�o�C�X
	ID3D12Device* device = nullptr;
	
	//FBX�}�l�[�W���[
	FbxManager* fbxManager = nullptr;

	//FBX�C���|�[�^
	FbxImporter* fbxImporter = nullptr;
};