#include "Geometry.hlsli"

//�O�p�`�̓��͂���A�_��1�o�̓T���v��
[maxvertexcount(6)]
void main(
	triangle VSOutput input[3] : SV_POSITION,
	//�_�X�g���[��
	inout TriangleStream< GSOutput > output
)
{
	for (uint i = 0; i < 3; i++)
	{
		GSOutput element;	//�o�͒��_�f�[�^
		element.svpos = input[i].svpos;	//���_���W���R�s�[
		element.normal = input[i].normal;	//�@�����R�s�[
		element.uv = input[i].uv;	//�t�u���R�s�[
		//���_��1�o��(�o�̓��X�g�ɒǉ�)
		output.Append(element);
	}
	//���݂̃X�g���b�v���I��
	output.RestartStrip();

	// 2�ڂ̎O�p�`
    for (uint i = 0; i < 3; i++)    {
        GSOutput element;
        // X������20���炷
        element.svpos = input[i].svpos + float4(20.0f, 0.0f, 0.0f, 0.0f);
        element.normal = input[i].normal;
        // UV��5�{��
        element.uv = input[i].uv * 5.0f;
        output.Append(element);
    }

}