//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// File: BMS.h
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
#ifndef RTW_HEADER_BMS_h_
#define RTW_HEADER_BMS_h_
#include "rtwtypes.h"
#include "BMS_types.h"
#include "BMSState.h"

// Macros for accessing real-time model data structure
#ifndef rtmGetErrorStatus
#define rtmGetErrorStatus(rtm)         ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
#define rtmSetErrorStatus(rtm, val)    ((rtm)->errorStatus = (val))
#endif

#ifndef rtmGetErrorStatusPointer
#define rtmGetErrorStatusPointer(rtm)  ((const char_T **)(&((rtm)->errorStatus)))
#endif

// Block signals (default storage)
struct B_BMS_T {
  real_T NumTemp;                      // '<Root>/Model'
  real_T NumVol;                       // '<Root>/Model'
  real_T Ri;                           // '<Root>/Model'
  real_T maxCurrent;                   // '<Root>/Model'
  real_T maxVoltage;                   // '<Root>/Model'
};

// External inputs (root inport signals with default storage)
struct ExtU_BMS_T {
  BatteryMode BatteryState;            // '<Root>/BatteryState'
  BMSTempInData BMSTempInData_f;       // '<Root>/BMSTempInData'
  BMSVoltageInData BMSVoltageInData_c; // '<Root>/BMSVoltageInData'
  real_T BMSCurrentInData;             // '<Root>/BMSCurrentInData'
};

// External outputs (root outports fed by signals with default storage)
struct ExtY_BMS_T {
  BMS_ActualValues BMSActualValues;    // '<Root>/BMSActualValues'
};

// Real-time Model Data Structure
struct tag_RTM_BMS_T {
  const char_T *errorStatus;
};

// Class declaration for model BMS
class BMS final
{
  // public data and function members
 public:
  // Copy Constructor
  BMS(BMS const&) = delete;

  // Assignment Operator
  BMS& operator= (BMS const&) & = delete;

  // Move Constructor
  BMS(BMS &&) = delete;

  // Move Assignment Operator
  BMS& operator= (BMS &&) = delete;

  // Real-Time Model get method
  RT_MODEL_BMS_T * getRTM();

  // Root inports set method
  void setExternalInputs(const ExtU_BMS_T *pExtU_BMS_T)
  {
    BMS_U = *pExtU_BMS_T;
  }

  // Root outports get method
  const ExtY_BMS_T &getExternalOutputs() const
  {
    return BMS_Y;
  }

  // model initialize function
  void initialize();

  // model step function
  void step();

  // model terminate function
  static void terminate();

  // Constructor
  BMS();

  // Destructor
  ~BMS();

  // private data and function members
 private:
  // External inputs
  ExtU_BMS_T BMS_U;

  // External outputs
  ExtY_BMS_T BMS_Y;

  // Block signals
  B_BMS_T BMS_B;

  // model instance variable for '<Root>/Model'
  BMSState ModelMDLOBJ1;

  // Real-Time Model
  RT_MODEL_BMS_T BMS_M;
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
//  '<Root>' : 'BMS'
//  '<S1>'   : 'BMS/SOCEstimation'

#endif                                 // RTW_HEADER_BMS_h_

//
// File trailer for generated code.
//
// [EOF]
//
