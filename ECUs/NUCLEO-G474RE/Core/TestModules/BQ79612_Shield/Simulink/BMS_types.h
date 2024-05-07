//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// File: BMS_types.h
//
// Code generated for Simulink model 'BMS'.
//
// Model version                  : 1.4
// Simulink Coder version         : 9.8 (R2022b) 13-May-2022
// C/C++ source code generated on : Fri Feb 16 00:52:40 2024
//
// Target selection: ert.tlc
// Embedded hardware selection: ARM Compatible->ARM Cortex-M
// Emulation hardware selection:
//    Differs from embedded hardware (MATLAB Host)
// Code generation objectives: Unspecified
// Validation result: Not run
//
#ifndef RTW_HEADER_BMS_types_h_
#define RTW_HEADER_BMS_types_h_
#include "rtwtypes.h"
#ifndef DEFINED_TYPEDEF_FOR_BatteryMode_
#define DEFINED_TYPEDEF_FOR_BatteryMode_

enum class BatteryMode
  : int32_T {
  Driving = 0,                         // Default value
  Charging,
  Error
};

#endif

#ifndef DEFINED_TYPEDEF_FOR_BMSTempInData_
#define DEFINED_TYPEDEF_FOR_BMSTempInData_

struct BMSTempInData
{
  real_T Temp1;
  real_T Temp2;
  real_T Temp3;
  real_T Temp4;
  real_T Temp5;
  real_T Temp6;
  real_T Temp7;
  real_T Temp8;
  real_T Temp9;
  real_T Temp10;
  real_T Temp11;
  real_T Temp12;
};

#endif

#ifndef DEFINED_TYPEDEF_FOR_BMSVoltageInData_
#define DEFINED_TYPEDEF_FOR_BMSVoltageInData_

struct BMSVoltageInData
{
  real_T Voltage1;
  real_T Voltage2;
  real_T Voltage3;
  real_T Voltage4;
  real_T Voltage5;
  real_T Voltage6;
  real_T Voltage7;
  real_T Voltage8;
  real_T Voltage9;
  real_T Voltage10;
  real_T Voltage11;
  real_T Voltage12;
};

#endif

#ifndef DEFINED_TYPEDEF_FOR_BMSParam_
#define DEFINED_TYPEDEF_FOR_BMSParam_

struct BMSParam
{
  real_T NumTemp;
  real_T NumVol;
  real_T Ri;
  real_T maxCurrent;
  real_T maxVoltage;
};

#endif

#ifndef DEFINED_TYPEDEF_FOR_BMS_ActualValues_
#define DEFINED_TYPEDEF_FOR_BMS_ActualValues_

struct BMS_ActualValues
{
  real_T Voltage;
  real_T BatTemp;
  real_T BatCurrent;
  real_T BatSOC;
};

#endif

// Forward declaration for rtModel
typedef struct tag_RTM_BMS_T RT_MODEL_BMS_T;

#endif                                 // RTW_HEADER_BMS_types_h_

//
// File trailer for generated code.
//
// [EOF]
//
