//ʵ�����������

#pragma once

#include <d3d9.h>
#include <d3dx9.h>

class CameraClass
{
private:
	D3DXVECTOR3 m_vRightVector;      //�ҷ�������
	D3DXVECTOR3 m_vUpVector;         //�Ϸ�������
	D3DXVECTOR3 m_vLookVector;       //�۲췽������
	D3DXVECTOR3 m_vCameraPosition;   //�����λ������
	D3DXVECTOR3 m_vTargetPosition;   //Ŀ��۲�λ������
	D3DXMATRIX  m_matView;           //ȡ���任����
	D3DXMATRIX  m_matProj;           //ͶӰ�任����
	LPDIRECT3DDEVICE9 m_pd3dDevice;  //direct3d�豸����

public:
	//����ȡ���任����
	VOID CalculateViewMatrix(D3DXMATRIX *pMatrix);

	//���ص�ǰͶӰ����
	VOID GetProjMatrix(D3DXMATRIX *pMatrix) { *pMatrix = m_matProj; }

	//���ص�ǰ�����λ�þ���
	VOID GetCameraPosition(D3DXMATRIX *pVector) { *pVector = m_vCameraPosition; }

	//���ص�ǰ�۲����
	VOID GetLookVector(D3DMATRIX *pVector) { *pVector = m_vLookVector; }

	//�����������Ŀ��۲�λ������
	VOID SetTargetPosition(D3DXVECTOR3 *pLookat = NULL);

	//�������������λ������
	VOID SetCameraPosition(D3DXMATRIX *pMatrix = NULL);

	//����ȡ���任����
	VOID SetViewMatrix(D3DMATRIX *pMatrix = NULL);

	//����ͶӰ�任����
	VOID SetProjMatrix(D3DMATRIX *pMatrix = NULL);

public:
	//�ظ�����ƽ�ƺ���
	VOID MoveAlongRightVec(FLOAT fUnits);//��right�����ƶ�
	VOID MoveAlongUpVec(FLOAT fUnits);   //��up�����ƶ�
	VOID MoveAlongLookVec(FLOAT fUnits); //��look�����ƶ�

	//�Ƹ�������ת
	VOID RotationRightVec(FLOAT fAngle); //��right������ת
	VOID RotationUpVec(FLOAT fAngle);    //��up������ת
	VOID RotationLookVec(FLOAT fAngle);  //��look������ת

public:
	//���캯������������
	CameraClass(IDirect3DDevice9 *pd3dDevice);
	virtual ~CameraClass(void);



};