#ifndef _ALL_USER_DATA_H_
#define _ALL_USER_DATA_H_

typedef   signed          char int8_t;
typedef   signed short     int int16_t;
typedef   signed           int int32_t;
typedef   signed       long long int64_t;

    /* exact-width unsigned integer types */
typedef unsigned          char uint8_t;
typedef unsigned short     int uint16_t;
typedef unsigned           int uint32_t;
typedef unsigned       long long uint64_t;




typedef struct{
	int16_t accX;
	int16_t accY;
	int16_t accZ;
	int16_t gyroX;
	int16_t gyroY;
	int16_t gyroZ;
}_st_Mpu;


typedef struct{
	int16_t magX;
	int16_t magY;
	int16_t magZ;
}_st_Mag;


typedef struct{
	float rate;
	float height;
}High;


typedef struct{
	float roll;
	float pitch;
	float yaw;
}_st_AngE;



extern _st_Mpu MPU6050;
extern _st_Mag AK8975; //保留，需外接磁力计
extern _st_AngE Angle;



#endif

