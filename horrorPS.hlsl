
#include "common.hlsl"

//DirectX�̃e�N�X�`���[�̐ݒ���󂯌p��
Texture2D g_Texture : register(t0); //�e�N�X�`���[0��
//DirectX�̃T���v���[�X�e�[�g�ݒ���󂯌p��
SamplerState g_SamplerState : register(s0); //�e�N�X�`���[�T���v���[0��

float Rand(float2 coord)
{
    //frac(x)=x�̏�������Ԃ� ������0.0 <= rand < 1.0�͈̔�
    return frac(sin(dot(coord.xy, float2(12.9898f, 78.233f))) * 43758.5453f);
}

void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
	
	//���̃s�N�Z���Ɏg����e�N�X�`���[�̐F���擾
    outDiffuse = g_Texture.Sample(g_SamplerState, In.TexCoord);
    
	//���̃s�N�Z���̃f�t���[�Y�F����Z
    outDiffuse *= In.Diffuse;
    
    //�e�N�X�`���f�[�^�擾
    float4 Tex = g_Texture.Sample(g_SamplerState, In.TexCoord);
    
    //�e�N�X�`�����W��C���ꂩ��̃p�����[�^�l��Seed�ɂ��ė������쐬
    float noise = clamp(0.2f, 0.5f, Rand(In.TexCoord * Parameter.y));
    
    //�����l���P�x�l�Ƃ��ăe�N�X�`���̃f�[�^�։��Z����
    Tex.rgb += noise;

    //���_�i�s�N�Z���j�F�ƍ���
    outDiffuse.rgb = saturate(Tex.rgb) * In.Diffuse.rgb;
    outDiffuse.a = Tex.a * In.Diffuse.a; //���͕ʏ���
    
    
    //�O���[�X�P�[���� Y = 0.299 * R + 0.587 * G + 0.114 * B
    outDiffuse.rgb = (0.299 * outDiffuse.r) + (0.587 * outDiffuse.g) + (0.114 * outDiffuse.b);
}
