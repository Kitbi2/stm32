#ifndef __MPU6050_H
#define __MPU6050_H





extern int16_t MpuOffset[6];

extern int8_t MpuInit(void);
extern void MpuGetData(void);
extern void MpuGetOffset(void);


#endif // __MPU6050_H__








