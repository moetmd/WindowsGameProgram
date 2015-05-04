//实现虚拟摄像机

#pragma once

#include <d3d9.h>
#include <d3dx9.h>

class CameraClass
{
private:
	D3DXVECTOR3 m_vRightVector;      //右分量向量
	D3DXVECTOR3 m_vUpVector;         //上分量向量
	D3DXVECTOR3 m_vLookVector;       //观察方向向量
	D3DXVECTOR3 m_vCameraPosition;   //摄像机位置向量
	D3DXVECTOR3 m_vTargetPosition;   //目标观察位置向量
	D3DXMATRIX  m_matView;           //取景变换矩阵
	D3DXMATRIX  m_matProj;           //投影变换矩阵
	LPDIRECT3DDEVICE9 m_pd3dDevice;  //direct3d设备对象

public:
	//计算取景变换函数
	VOID CalculateViewMatrix(D3DXMATRIX *pMatrix);

	//返回当前投影矩阵
	VOID GetProjMatrix(D3DXMATRIX *pMatrix) { *pMatrix = m_matProj; }

	//返回当前摄像机位置矩阵
	VOID GetCameraPosition(D3DXMATRIX *pVector) { *pVector = m_vCameraPosition; }

	//返回当前观察矩阵
	VOID GetLookVector(D3DMATRIX *pVector) { *pVector = m_vLookVector; }

	//设置摄像机的目标观察位置向量
	VOID SetTargetPosition(D3DXVECTOR3 *pLookat = NULL);

	//设置摄像机所在位置向量
	VOID SetCameraPosition(D3DXMATRIX *pMatrix = NULL);

	//设置取景变换矩阵
	VOID SetViewMatrix(D3DMATRIX *pMatrix = NULL);

	//设置投影变换矩阵
	VOID SetProjMatrix(D3DMATRIX *pMatrix = NULL);

public:
	//沿各分量平移函数
	VOID MoveAlongRightVec(FLOAT fUnits);//沿right向量移动
	VOID MoveAlongUpVec(FLOAT fUnits);   //沿up向量移动
	VOID MoveAlongLookVec(FLOAT fUnits); //沿look向量移动

	//绕各分量旋转
	VOID RotationRightVec(FLOAT fAngle); //绕right向量旋转
	VOID RotationUpVec(FLOAT fAngle);    //绕up向量旋转
	VOID RotationLookVec(FLOAT fAngle);  //绕look向量旋转

public:
	//构造函数和析构函数
	CameraClass(IDirect3DDevice9 *pd3dDevice);
	virtual ~CameraClass(void);



};