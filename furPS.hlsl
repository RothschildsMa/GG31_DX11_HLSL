
#include "common.hlsl"

Texture2D g_Texture : register(t0);
Texture2D g_TextureFur : register(t1); //�t�@�[�e�N�X�`��
SamplerState g_SamplerState : register(s0);

void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
    //�e�N�X�`�����擾
    outDiffuse = g_Texture.Sample(g_SamplerState, In.TexCoord);
    
    //���_�̖��邳����Z���č���
    outDiffuse *= In.Diffuse;
    
    //�t�@�[�V�F�[�_�[�p�e�N�X�`���̃��l����Z���č���
    outDiffuse.a *= g_TextureFur.Sample(g_SamplerState, In.TexCoord).a;
    
}