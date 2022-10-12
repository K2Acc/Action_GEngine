#include "FBX.hlsli"

VSOutput main( VSInput input )
{
	float4 wnormal = normalize(mul(world, float4(input.normal, 0)));
	//�s�N�Z���V�F�[�_�[�ɓn���l
	VSOutput output;
	//�s��ɂ����W�ϊ�
	output.svpos = mul(shadow, input.pos);
	output.svpos = mul(mul(viewproj, world), output.svpos);
	//���[���h�@�������̃X�e�[�W�ɓn��
	output.normal = wnormal.xyz;
	//���͒l�����̂܂܎��̃X�e�[�W��n��
	output.uv = input.uv;

	return output;
}
