//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// File: BMSState_types.h
//
// Code generated for Simulink model 'BMSState'.
//
// Model version                  : 1.2
// Simulink Coder version         : 9.8 (R2022b) 13-May-2022
// C/C++ source code generated on : Fri Feb 16 00:52:30 2024
//
// Target selection: ert.tlc
// Embedded hardware selection: ARM Compatible->ARM Cortex-M
// Emulation hardware selection:
//    Differs from embedded hardware (MATLAB Host)
// Code generation objectives: Unspecified
// Validation result: Not run
//
#ifndef RTW_HEADER_BMSState_types_h_
#define RTW_HEADER_BMSState_types_h_
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

// Forward declaration for rtModel
typedef struct tag_RTM_BMSState_T RT_MODEL_BMSState_T;

#endif                                 // RTW_HEADER_BMSState_types_h_

//
// File trailer for generated code.
//
// [EOF]
//
