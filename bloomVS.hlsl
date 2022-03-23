
#include "common.hlsl"

void main(in VS_IN In, out PS_IN Out)
{
    matrix wvp; //���[���h�r���[�v���W�F�N�V�����s��쐬
    wvp = mul(World, View);
    wvp = mul(wvp, Projection);
    Out.Position = mul(In.Position, wvp); //���_��WVP�s��ŕϊ����ďo��
    Out.TexCoord = In.TexCoord; //�e�N�X�`�����W���o��
    Out.Diffuse = In.Diffuse; //���_�J���[���o��
    
}
