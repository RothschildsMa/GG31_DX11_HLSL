
#include "common.hlsl"


void main(in VS_IN In, out PS_IN Out)
{

	//1���_���̃f�[�^���󂯎��A�������āA�o�͂���
	//���_�ϊ����� ���̏����͕K���K�v
    matrix wvp; //�s��ϐ����쐬
    wvp = mul(World, View); //wvp = ���[���h�s�� * �J�����s��
    wvp = mul(wvp, Projection); //wvp = wvp * �v���W�F�N�V�����s��
    Out.Position = mul(In.Position, wvp); //�ϊ����ʂ��o�͂���

	//�󂯎�������̒��_�̃f�t���[�Y�����̂܂܏o��
    Out.TexCoord = In.TexCoord;

	//���_�̃e�N�X�`���[���W���o��
    Out.Diffuse = In.Diffuse;

}

