//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// File: BMSState.cpp
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
#include "BMSState.h"
#include "rtwtypes.h"
#include "BMSState_types.h"
#include "BMSState_private.h"

// Output and update for referenced model: 'BMSState'
void BMSState::step(const BatteryMode *rtu_BatteryMode, real_T
                    *rty_mBMSParam_NumTemp, real_T *rty_mBMSParam_NumVol, real_T
                    *rty_mBMSParam_Ri, real_T *rty_mBMSParam_maxCurrent, real_T *
                    rty_mBMSParam_maxVoltage)
{
  // SwitchCase: '<Root>/Switch Case1'
  switch (*rtu_BatteryMode) {
   case BatteryMode::Driving:
    // Outputs for IfAction SubSystem: '<Root>/Switch Case Action Subsystem1' incorporates:
    //   ActionPort: '<S1>/Action Port'

    // SignalConversion generated from: '<S1>/Constant' incorporates:
    //   Constant: '<S1>/Constant'

    *rty_mBMSParam_NumTemp = 12.0;

    // SignalConversion generated from: '<S1>/Constant1' incorporates:
    //   Constant: '<S1>/Constant1'

    *rty_mBMSParam_NumVol = 12.0;

    // SignalConversion generated from: '<S1>/Constant2' incorporates:
    //   Constant: '<S1>/Constant2'

    *rty_mBMSParam_Ri = 0.18932;

    // SignalConversion generated from: '<S1>/Constant3' incorporates:
    //   Constant: '<S1>/Constant3'

    *rty_mBMSParam_maxCurrent = 100.0;

    // SignalConversion generated from: '<S1>/Constant4' incorporates:
    //   Constant: '<S1>/Constant4'

    *rty_mBMSParam_maxVoltage = 600.0;

    // End of Outputs for SubSystem: '<Root>/Switch Case Action Subsystem1'
    break;

   case BatteryMode::Charging:
    // Outputs for IfAction SubSystem: '<Root>/Switch Case Action Subsystem2' incorporates:
    //   ActionPort: '<S2>/Action Port'

    // SignalConversion generated from: '<S2>/Constant' incorporates:
    //   Constant: '<S2>/Constant'

    *rty_mBMSParam_NumTemp = 12.0;

    // SignalConversion generated from: '<S2>/Constant1' incorporates:
    //   Constant: '<S2>/Constant1'

    *rty_mBMSParam_NumVol = 12.0;

    // SignalConversion generated from: '<S2>/Constant2' incorporates:
    //   Constant: '<S2>/Constant2'

    *rty_mBMSParam_Ri = 0.18932;

    // SignalConversion generated from: '<S2>/Constant3' incorporates:
    //   Constant: '<S2>/Constant3'

    *rty_mBMSParam_maxCurrent = -30.0;

    // SignalConversion generated from: '<S2>/Constant4' incorporates:
    //   Constant: '<S2>/Constant4'

    *rty_mBMSParam_maxVoltage = 600.0;

    // End of Outputs for SubSystem: '<Root>/Switch Case Action Subsystem2'
    break;

   default:
    // Outputs for IfAction SubSystem: '<Root>/Switch Case Action Subsystem3' incorporates:
    //   ActionPort: '<S3>/Action Port'

    // SignalConversion generated from: '<S3>/Constant' incorporates:
    //   Constant: '<S3>/Constant'

    *rty_mBMSParam_NumTemp = 12.0;

    // SignalConversion generated from: '<S3>/Constant1' incorporates:
    //   Constant: '<S3>/Constant1'

    *rty_mBMSParam_NumVol = 12.0;

    // SignalConversion generated from: '<S3>/Constant2' incorporates:
    //   Constant: '<S3>/Constant2'

    *rty_mBMSParam_Ri = 0.18932;

    // SignalConversion generated from: '<S3>/Constant3' incorporates:
    //   Constant: '<S3>/Constant3'

    *rty_mBMSParam_maxCurrent = 0.0;

    // SignalConversion generated from: '<S3>/Constant4' incorporates:
    //   Constant: '<S3>/Constant4'

    *rty_mBMSParam_maxVoltage = 600.0;

    // End of Outputs for SubSystem: '<Root>/Switch Case Action Subsystem3'
    break;
  }

  // End of SwitchCase: '<Root>/Switch Case1'
}

// Constructor
BMSState::BMSState() :
  BMSState_M()
{
  // Currently there is no constructor body generated.
}

// Destructor
BMSState::~BMSState()
{
  // Currently there is no destructor body generated.
}

// Real-Time Model get method
RT_MODEL_BMSState_T * BMSState::getRTM()
{
  return (&BMSState_M);
}

// member function to setup error status pointer
void BMSState::setErrorStatusPointer(const char_T **rt_errorStatus)
{
  rtmSetErrorStatusPointer((&BMSState_M), rt_errorStatus);
}

//
// File trailer for generated code.
//
// [EOF]
//
