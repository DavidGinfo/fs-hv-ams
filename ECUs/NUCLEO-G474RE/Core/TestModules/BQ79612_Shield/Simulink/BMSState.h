//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// File: BMSState.h
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
#ifndef RTW_HEADER_BMSState_h_
#define RTW_HEADER_BMSState_h_
#include "rtwtypes.h"
#include "BMSState_types.h"
#include <cstring>

// Real-time Model Data Structure
struct tag_RTM_BMSState_T {
  const char_T **errorStatus;
};

// Class declaration for model BMSState
class BMSState final
{
  // public data and function members
 public:
  // Copy Constructor
  BMSState(BMSState const&) = delete;

  // Assignment Operator
  BMSState& operator= (BMSState const&) & = delete;

  // Move Constructor
  BMSState(BMSState &&) = delete;

  // Move Assignment Operator
  BMSState& operator= (BMSState &&) = delete;

  // Real-Time Model get method
  RT_MODEL_BMSState_T * getRTM();

  //member function to setup error status pointer
  void setErrorStatusPointer(const char_T **rt_errorStatus);

  // model step function
  void step(const BatteryMode *rtu_BatteryMode, real_T *rty_mBMSParam_NumTemp,
            real_T *rty_mBMSParam_NumVol, real_T *rty_mBMSParam_Ri, real_T
            *rty_mBMSParam_maxCurrent, real_T *rty_mBMSParam_maxVoltage);

  // Constructor
  BMSState();

  // Destructor
  ~BMSState();

  // private data and function members
 private:
  // Real-Time Model
  RT_MODEL_BMSState_T BMSState_M;
};

//-
//  The generated code includes comments that allow you to trace directly
//  back to the appropriate location in the model.  The basic format
//  is <system>/block_name, where system is the system number (uniquely
//  assigned by Simulink) and block_name is the name of the block.
//
//  Use the MATLAB hilite_system command to trace the generated code back
//  to the model.  For example,
//
//  hilite_system('<S3>')    - opens system 3
//  hilite_system('<S3>/Kp') - opens and selects block Kp which resides in S3
//
//  Here is the system hierarchy for this model
//
//  '<Root>' : 'BMSState'
//  '<S1>'   : 'BMSState/Switch Case Action Subsystem1'
//  '<S2>'   : 'BMSState/Switch Case Action Subsystem2'
//  '<S3>'   : 'BMSState/Switch Case Action Subsystem3'

#endif                                 // RTW_HEADER_BMSState_h_

//
// File trailer for generated code.
//
// [EOF]
//
