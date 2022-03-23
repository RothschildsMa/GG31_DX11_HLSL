

#include "common.hlsl"


Texture2D g_Texture : register(t0);
SamplerState g_SamplerState : register(s0);


void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
	float4 normal = normalize(In.Normal); //�s�N�Z���̖@���𐳋K��
	//float  light = -dot(normal.xyz, Light.Direction.xyz); //���邳���v�Z
	float  light = 0.5f - 0.5f * dot(normal.xyz, Light.Direction.xyz);//�n�[�t�����o�[�g�ɂ�閾�邳�̌v�Z

	outDiffuse = g_Texture.Sample(g_SamplerState, In.TexCoord);
	outDiffuse.rgb *= (light * In.Diffuse.rgb);
	outDiffuse.a *= In.Diffuse.a; //���ɖ��邳�͊֌W�Ȃ��̂ŕʌv�Z

	//�X�y�L�����[(�t�H�����ʔ���)
	float3 eyev = In.WorldPosition.xyz - CameraPosition.xyz;//�����x�N�g��
	eyev = normalize(eyev);
	float3 refv = reflect(Light.Direction.xyz, normal.xyz);//���̔��˃x�N�g��
	refv = normalize(refv);
	float specular = -dot(eyev, refv);	//���ʔ��˂̌v�Z
	specular = saturate(specular);		//�l���T�`�����[�g

	specular = pow(specular, 5);		//30��
	outDiffuse.rgb += specular;


	
}

