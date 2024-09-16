#ifndef __IMU_H
#define	__IMU_H


#include "ALL_DATA.h"

#define DEG2RAD  0.017453293f //��ת���Ȧ�/180 
#define RAD2DEG  57.29578f    //����ת��180/�� 

typedef struct {
	float yaw;
	float pitch;
	float roll;
	float yaw_mag; //�����ɴ����Ƶĳ����ĽǶ�
	float Cos_Roll;
	float Sin_Roll;
	float Cos_Pitch;
	float Sin_Pitch;
	float Cos_Yaw;
	float Sin_Yaw;
}_st_IMU;



	typedef struct V{
				float x;
				float y;
				float z;
				} MPUDA;

extern float yaw_control;
extern float Yaw_Correct;
extern _st_IMU IMU;
extern float GetAccz(void);
extern void GetAngle(const _st_Mpu *pMpu,_st_AngE *pAngE, float dt);
extern	 MPUDA  Gravity, Acc, Gyro,AccGravity;

#endif
