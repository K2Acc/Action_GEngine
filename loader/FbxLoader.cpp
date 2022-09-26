#include "FbxLoader.h"
#include <cassert>

/// <summary>
/// �ÓI�����o�ϐ��̎���
/// </summary>
const std::string FbxLoader::baseDirectory = "Resources/";

const std::string FbxLoader::defaultTextureFileName = "white1x1.png";

using namespace DirectX;

FbxLoader* FbxLoader::GetInstance()
{
    static FbxLoader instance;
    return &instance;
}

void FbxLoader::ConvertMatrixFromFbx(DirectX::XMMATRIX *dst, const FbxAMatrix &src)
{
    //�s
    for(int i = 0; i < 4; i++)
    {
        //��
        for(int j = 0; j < 4; j++)
        {
            //1�v�f�R�s�[
            dst->r[i].m128_f32[j] = (float)src.Get(i, j);
        }
    }
}

void FbxLoader::Initialize(ID3D12Device *device)
{
    //�ď������`�F�b�N
    assert(!fbxManager);

    this->device = device;

    //�}�l�[�W���[����
    fbxManager = FbxManager::Create();

    //fbx�}�l�[�W���̓��o�͐ݒ�
    FbxIOSettings* ios = FbxIOSettings::Create(fbxManager,IOSROOT);
    fbxManager->SetIOSettings(ios);

    //FBX�C���|�[�^�̐���
    fbxImporter = FbxImporter::Create(fbxManager, "");
}

FbxObjectManager* FbxLoader::LoadModeFromFile(const string &modelName)
{
    //���f���Ɠ������O�Ƀt�@�C������Ǎ�
    const string directoryPath = baseDirectory + modelName + "/";

    //�g���q.fbx��t��
    const string fileName = modelName + ".fbx";

    //�A�������t���p�X�𓾂�
    const string fullpath = directoryPath + fileName;

    //�t�@�C�������w�肵��FBX�t�@�C����ǂݍ���
    auto result = fbxImporter->Initialize(fullpath.c_str(), -1, fbxManager->GetIOSettings());
    assert(result);

    //�V�[������
    FbxScene* fbxScene = FbxScene::Create(fbxManager, "fbxScene");

    //�t�@�C�����烍�[�h����FBX�̏����V�[���ɃC���|�[�g
    fbxImporter->Import(fbxScene);

    //���f������
    FbxObjectManager* model = new FbxObjectManager();
    model->name = modelName;

    //FBX�m�[�h�̐����擾
    int nodeCount = fbxScene->GetNodeCount();
    //���炩���ߕK�v�����̃��������m�ۂ��邱�ƂŁA�A�h���X�������̂�\�h
    model->nodes.reserve(nodeCount);

    //���[�g�m�[�h���珇�ɉ�͂��ă��f���ɗ�������
    ParseNodeRecursive(model,fbxScene->GetRootNode());
    
    model->fbxScene = fbxScene;

    //�o�b�t�@����
    model->CreateBuffers(device);

    return model;
}

void FbxLoader::ParseNodeRecursive(FbxObjectManager *model, FbxNode *fbxNode, Node* parent)
{
    //���f���Ƀm�[�h��ǉ�
    model->nodes.emplace_back();
    Node& node = model->nodes.back();

    //�m�[�h�����擾
    string name = fbxNode->GetName();

    //FBX�m�[�h�̃��[�J���ړ����
    FbxDouble3 rotation = fbxNode->LclRotation.Get();
    FbxDouble3 scaling = fbxNode->LclScaling.Get();
    FbxDouble3 translation = fbxNode->LclTranslation.Get();

    //�`���ϊ����đ��
    node.rotation = {(float)rotation[0], (float)rotation[1], (float)rotation[2], 0.0f};
    node.scaling = {(float)scaling[0], (float)scaling[1], (float)scaling[2], 0.0f};
    node.translation = {(float)translation[0], (float)translation[1], (float)translation[2], 0.0f};

    //��]�p��Degree(�x)���烉�W�A���ɕϊ�
    node.rotation.m128_f32[0] = XMConvertToRadians(node.rotation.m128_f32[0]);
    node.rotation.m128_f32[1] = XMConvertToRadians(node.rotation.m128_f32[1]);
    node.rotation.m128_f32[2] = XMConvertToRadians(node.rotation.m128_f32[2]);

    //�X�P�[���A��]�A���s�ړ��s��̌v�Z
    XMMATRIX matScaling, matRotation, matTranslation;
    matScaling = XMMatrixScalingFromVector(node.scaling);
    matRotation = XMMatrixRotationRollPitchYawFromVector(node.rotation);
    matTranslation = XMMatrixTranslationFromVector(node.translation);

    //���[�J���ό`�s��̌v�Z
    node.transform = XMMatrixIdentity();
    node.transform *= matScaling;
    node.transform *= matRotation;
    node.transform *= matTranslation;

    //���f���Ƀm�[�h��ǉ�(Todo)
     
    //�O���[�o���ό`�s��̌v�Z
    node.globalTransform =node.transform;
    if(parent)
    {
        node.parent= parent;
        //�e�̕ό`�s�����Z
        node.globalTransform *= parent->globalTransform;
    }
    
    //FBX�m�[�h�̏�����͂��ăm�[�h�ɋL�^(Todo)
    //FBX�m�[�h�̃��b�V���������
    FbxNodeAttribute* fbxNodeAttribute = fbxNode->GetNodeAttribute();

    if(fbxNodeAttribute)
    {
        if(fbxNodeAttribute->GetAttributeType() == FbxNodeAttribute::eMesh)
        {
            model->meshNode = &node;
            ParseMesh(model, fbxNode);
        }
    }

    //�q�m�[�h�ɑ΂��čċA�Ăяo��
    for(int i = 0; i < fbxNode->GetChildCount(); i++)
    {
        ParseNodeRecursive(model, fbxNode->GetChild(i), &node);
    }
}

void FbxLoader::ParseMesh(FbxObjectManager *model, FbxNode *fbxNode)
{
    //�m�[�h�̃��b�V�����擾
    FbxMesh* fbxMesh = fbxNode->GetMesh();

    //���_���ǂݎ��
    ParseMeshVertices(model, fbxMesh);

    //�ʂ��\������f�[�^�̓ǂݎ��
    ParseMeshFaces(model,fbxMesh);

    //�}�e���A���̓ǂݎ��
    ParseMaterial(model, fbxNode);

    //�X�L�j���O���̓ǂݎ��
    ParseSkin(model, fbxMesh);
}

void FbxLoader::ParseMeshVertices(FbxObjectManager *model, FbxMesh *fbxMesh)
{
    auto& vertices = model->vertices;

    //���_�f�[�^�̐�
    const int controlPointCount = fbxMesh->GetControlPointsCount();
    //�K�v���������_�f�[�^�z����m��
    FbxObjectManager::VertexPosNormalUvSkin vert{};
    model->vertices.resize(controlPointCount, vert);

    //FBX���b�V���̒��_���W�z����擾
    FbxVector4* pCoord = fbxMesh->GetControlPoints();

    //FBX���b�V���̑O���X���W�����f�����̔z��ɃR�s�[����
    for(int i = 0; i < controlPointCount; i++)
    {
        FbxObjectManager::VertexPosNormalUvSkin& vertex = vertices[i];

        //���W�R�s�[
        vertex.pos.x = (float)pCoord[i][0];
        vertex.pos.y = (float)pCoord[i][1];
        vertex.pos.z = (float)pCoord[i][2];
    }
}

void FbxLoader::ParseMeshFaces(FbxObjectManager *model, FbxMesh *fbxMesh)
{
    auto& vertices = model->vertices;
    auto& indices = model->indices;

    //1�t�@�C���ɕ������b�V���̃��f���͔�Ή�
    assert(indices.size() == 0);
    //�ʂ̐�
    const int polygonCount = fbxMesh->GetPolygonCount();
    //UV�f�[�^�̐�
    const int textureUVCount = fbxMesh->GetTextureUVCount();

    //UV�����X�g
    FbxStringList uvNames;
    fbxMesh->GetUVSetNames(uvNames);

    //�ʂ��Ƃ̏��ǂݎ��
    for(int i = 0; i < polygonCount; i++)
    {
        //�ʂ��\�����钸�_���̐����擾(3�Ȃ�O�p�`�|���S��)
        const int polygonSize = fbxMesh->GetPolygonSize(i);
        assert(polygonSize <= 4);

        //1���_������
        for(int j = 0; j < polygonSize; j++)
        {
            //FBX���_�z��̃C���f�b�N�X
            int index = fbxMesh->GetPolygonVertex(i,j);
            assert(index >= 0);


            //���_�@���ǂݍ���
            FbxObjectManager::VertexPosNormalUvSkin& vertex = vertices[index];
            FbxVector4 normal;
            if(fbxMesh->GetPolygonVertexNormal(i,j,normal))
            {
                vertex.normal.x = (float)normal[0];
                vertex.normal.y = (float)normal[1];
                vertex.normal.z = (float)normal[2];
            }

            //�e�N�X�`��UV�ǂݍ���
            if(textureUVCount > 0)
            {
                FbxVector2 uvs;
                bool lUnmappedUV;
                //0�Ԍ��ߑł��œǂݍ���
                if(fbxMesh->GetPolygonVertexUV(i,j, uvNames[0], uvs, lUnmappedUV))
                {
                    vertex.uv.x = (float)uvs[0];
                    vertex.uv.y = 1 - (float)uvs[1];
                }
            }

            //�C���f�b�N�X�z��ɒ��_�C���f�b�N�X�ǉ�
            //3���_�ڂ܂łȂ�
            if(j < 3)
            {
                //1�_�ǉ����A����2�_�ƎO�p�`���\�z����
                indices.push_back(index);
            }
            //4���_��
            else
            {
                //3���_�ǉ����A�l�p�`��0,1,2,3�̓�,2,3,0�ŎO�p�`���\�z����
                int index2 = indices[indices.size() - 1];
                int index3 = index;
                int index0 = indices[indices.size() - 3];
                indices.push_back(index2);
                indices.push_back(index3);
                indices.push_back(index0);
            }
        }
    }
}

void FbxLoader::ParseMaterial(FbxObjectManager *model, FbxNode *fbxNode)
{
    const int materialCount = fbxNode->GetMaterialCount();
    if(materialCount > 0)
    {
        //�擪�̃}�e���A�����擾
        FbxSurfaceMaterial* material = fbxNode->GetMaterial(0);
        //�e�N�X�`����ǂݍ��񂾂��ǂ�����\���t���O
        bool textureLoaded = false;

        if(material)
        {
            //FbxSurfaseLambert�N���X���ǂ����𒲂ׂ�
            if(material->GetClassId().Is(FbxSurfaceLambert::ClassId))
            {
                FbxSurfaceLambert* lambert = static_cast<FbxSurfaceLambert*>(material);

                //�����W��
                FbxPropertyT<FbxDouble3> ambient = lambert->Ambient;
                model->ambient.x = (float)ambient.Get()[0];
                model->ambient.y = (float)ambient.Get()[1];
                model->ambient.z = (float)ambient.Get()[2];

                //�g�U���ˌ��W��
                FbxPropertyT<FbxDouble3> diffuse = lambert->Diffuse;
                model->diffuse.x = (float)diffuse.Get()[0];
                model->diffuse.y = (float)diffuse.Get()[1];
                model->diffuse.z = (float)diffuse.Get()[2];
            }
            //�f�B�t���[�Y�e�N�X�`�������o��
            const FbxProperty diffuseProperty = material->FindProperty(FbxSurfaceMaterial::sDiffuse);
            if(diffuseProperty.IsValid())
            {
                const FbxFileTexture* texture = diffuseProperty.GetSrcObject<FbxFileTexture>();
                if(texture)
                {
                    const char* filepath = texture->GetFileName();
                    //�t�@�C���p�X����t�@�C�������o
                    string path_str(filepath);
                    string name = ExtractFileName(path_str);
                    //�e�N�X�`���ǂݍ���
                    LoadTexture(model, baseDirectory + model->name + "/" + name);
                    textureLoaded = true;
                }
            }
        }
        //�e�N�X�`�����Ȃ��ꍇ���e�N�X�`����\��
        if(!textureLoaded)
        {
            LoadTexture(model, baseDirectory + defaultTextureFileName);
        }
    }
}

void FbxLoader::LoadTexture(FbxObjectManager *model, const std::string &fullpath)
{
    HRESULT result = S_FALSE;
    //WIC�e�N�X�`���̃��[�h
    TexMetadata& metadata = model->metadata;
    ScratchImage& scratchImg = model->scratchImg;
    //���j�R�[�h������ɕϊ�
    wchar_t wfilepath[128];
    MultiByteToWideChar(CP_ACP, 0, fullpath.c_str(), -1, wfilepath, _countof(wfilepath));
    result = LoadFromWICFile(wfilepath, WIC_FLAGS_NONE, &metadata, scratchImg);
    if(FAILED(result))
    {
        assert(0);
    }
}

void FbxLoader::ParseSkin(FbxObjectManager *model, FbxMesh *fbxMesh)
{
    //�X�L�j���O���
    FbxSkin* fbxSkin = static_cast<FbxSkin*>(fbxMesh->GetDeformer(0, FbxDeformer::eSkin));
    //�X�L�j���O��񂪂Ȃ���ΏI��
    if(fbxSkin == nullptr)
    {
        //�e���_�ɂ��Ă̏���
        for(int i = 0; i < model->vertices.size(); i++)
        {
            //�ŏ��̃{�[��(�P�ʍs��)�̉e����100%�ɂ���
            model->vertices[i].boneIndex[0] = 0;
            model->vertices[i].boneWeight[0] = 1.f;
        }

        return ;
    }

    //�{�[���z��̎Q��
    std::vector<Bone>& bones = model->bones;

    //�{�[����
    int clusterCount = fbxSkin->GetClusterCount();
    bones.reserve(clusterCount);

    //�S�Ẵ{�[���ɂ���
    for(int i = 0; i < clusterCount; i++)
    {
        //FBX�{�[�����
        FbxCluster* fbxCluster = fbxSkin->GetCluster(i);

        //�{�[�����̂̃m�[�h�̖��O���擾
        const char* boneName = fbxCluster->GetLink()->GetName();

        //�V�����{�[���ǉ����A�ǉ������{�[���̎Q�Ƃ𓾂�
        bones.emplace_back(Bone(boneName));
        Bone& bone = bones.back();
        //����{�[����FBX�̃{�[����R�Â���
        bone.fbxCluster = fbxCluster;
        
        //FBX���珉���p���s����擾
        FbxAMatrix fbxMat;
        fbxCluster->GetTransformLinkMatrix(fbxMat);

        //XMMatrix�^�ɕϊ�����
        XMMATRIX initialPose;
        ConvertMatrixFromFbx(&initialPose, fbxMat);

        //�����p���s��̋t�s��𓾂�
        bone.invInitialPose = XMMatrixInverse(nullptr, initialPose);
    }

    //�{�[���ԍ��ƃX�L���E�F�C�g�̃y�A
    struct WeightSet
    {
        UINT index;
        float weight;
    };

    //2�����z��(�W���O�z��)
    // list ���_���e�����󂯂�{�[���̑S���X�g
    // vector �����S���_��
    std::vector<std::list<WeightSet>> weightLists(model->vertices.size());

    //�S�Ẵ{�[���ɂ���
    for(int i = 0; i < clusterCount; i++)
    {
        //FBX�{�[�����
        FbxCluster* fbxCluster = fbxSkin->GetCluster(i);
        //���̃{�[���ɉe�����󂯂钸�_�̐�
        int controlPointIndicesCount = fbxCluster->GetControlPointIndicesCount();
        //���̃{�[���ɉe���������钸�_�̔z��
        int* controlPointIndices = fbxCluster->GetControlPointIndices();
        double* controlPointWeights = fbxCluster->GetControlPointWeights();

        //�e�����󂯂�S���_�ɑ΂���
        for(int j = 0; j < controlPointIndicesCount; j++)
        {
            //���_�ԍ�
            int vertIndex = controlPointIndices[j];
            //�X�L���E�F�C�g
            float weight = (float)controlPointWeights[j];
            //���̒��_�̉e�����󂯂�{�[�����X�g�ɁA�{�[���ƃE�F�C�g�̃y�A��ǉ�
            weightLists[vertIndex].emplace_back(WeightSet{(UINT)i, weight});
        }
    }

    //���_�z�񏑂������p�̎Q��
    auto& vertices = model->vertices;

    //�e���_�ɂ��ď���
    for(int i = 0; i < vertices.size(); i++)
    {
        //���_�̃E�F�C�g����ł��傫��4��I��
        auto& weightList = weightLists[i];
        //�召��r�p�̃����_�����w�肵�č~���Ƀ\�[�g
        weightList.sort([](auto const& lhs, auto const& rhs){
                //���̗v�f�̂ق����傫�����true,����łȂ����flafse��Ԃ�
                return lhs.weight > rhs.weight;
            });

        int weightArrayIndex = 0;

        //�~���\�[�g�ς݂̃E�F�C�g���X�g����
        for(auto& weightSet : weightList)
        {
            //���_�f�[�^�ɏ�������
            vertices[i].boneIndex[weightArrayIndex] = weightSet.index;
            vertices[i].boneWeight[weightArrayIndex] = weightSet.weight;
            //4�ɒB������I��
            if(++weightArrayIndex >= FbxObjectManager::MAX_BONE_INDICES)
            {
                float weight = 0.0f;
                //�Q�Ԗڈȍ~�̃E�F�C�g�����v
                for(int j = 1; j < FbxObjectManager::MAX_BONE_INDICES; j++)
                {
                    weight += vertices[i].boneWeight[j];
                }
                //���v��1.0f(100%)�ɂȂ�悤�ɒ���
                vertices[i].boneWeight[0] = 1.0f - weight;
                break;
            }
        }
    }

}

std::string FbxLoader::ExtractFileName(const std::string &path)
{
    size_t pos1;
    //��؂蕶�� "\\"���o�Ă����ԍŌ�̕���������
    pos1 = path.rfind("\\");
    if(pos1 != string::npos)
    {
        return path.substr(pos1 + 1, path.size() - pos1 - 1);
    }

    //��؂蕶�� "/" ���o�Ă����ԍŌ�̕���������
    pos1 = path.rfind("/");
    if(pos1 != string::npos)
    {
        return path.substr(pos1+ 1, path.size() - pos1 - 1);
    }

    return path;
}

void FbxLoader::Finalize()
{
    fbxImporter->Destroy();
    fbxManager->Destroy();
}
