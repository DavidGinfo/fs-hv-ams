//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// File: BMS.cpp
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
#include "BMS.h"
#include <cmath>
#include "rtwtypes.h"
#include "BMSState.h"

// Model step function
void BMS::step()
{
  real_T rtb_MinMax;
  real_T rtb_Voltage;

  // ModelReference generated from: '<Root>/Model' incorporates:
  //   Inport: '<Root>/BatteryState'

  ModelMDLOBJ1.step(&BMS_U.BatteryState, &BMS_B.NumTemp, &BMS_B.NumVol,
                    &BMS_B.Ri, &BMS_B.maxCurrent, &BMS_B.maxVoltage);

  // Outputs for Atomic SubSystem: '<Root>/SOCEstimation'
  // Sum: '<S1>/Add' incorporates:
  //   Inport: '<Root>/BMSVoltageInData'

  rtb_Voltage = BMS_U.BMSVoltageInData_c.Voltage1 +
    BMS_U.BMSVoltageInData_c.Voltage2 + BMS_U.BMSVoltageInData_c.Voltage3 +
    BMS_U.BMSVoltageInData_c.Voltage4 + BMS_U.BMSVoltageInData_c.Voltage5 +
    BMS_U.BMSVoltageInData_c.Voltage6 + BMS_U.BMSVoltageInData_c.Voltage7 +
    BMS_U.BMSVoltageInData_c.Voltage8 + BMS_U.BMSVoltageInData_c.Voltage9 +
    BMS_U.BMSVoltageInData_c.Voltage10 + BMS_U.BMSVoltageInData_c.Voltage11 +
    BMS_U.BMSVoltageInData_c.Voltage12;

  // MinMax: '<S1>/MinMax' incorporates:
  //   Inport: '<Root>/BMSCurrentInData'

  rtb_MinMax = std::fmin(BMS_U.BMSCurrentInData, BMS_B.maxCurrent);

  // Product: '<S1>/Divide' incorporates:
  //   Product: '<S1>/Product'
  //   Sum: '<S1>/Add2'

  BMS_Y.BMSActualValues.BatSOC = ((rtb_MinMax * BMS_B.Ri) + rtb_Voltage) /
    BMS_B.maxVoltage;

  // Product: '<S1>/Divide1' incorporates:
  //   Inport: '<Root>/BMSTempInData'
  //   Sum: '<S1>/Add1'

  BMS_Y.BMSActualValues.BatTemp = (BMS_U.BMSTempInData_f.Temp1 +
    BMS_U.BMSTempInData_f.Temp2 + BMS_U.BMSTempInData_f.Temp3 +
    BMS_U.BMSTempInData_f.Temp4 + BMS_U.BMSTempInData_f.Temp5 +
    BMS_U.BMSTempInData_f.Temp6 + BMS_U.BMSTempInData_f.Temp7 +
    BMS_U.BMSTempInData_f.Temp8 + BMS_U.BMSTempInData_f.Temp9 +
    BMS_U.BMSTempInData_f.Temp10 + BMS_U.BMSTempInData_f.Temp11 +
    BMS_U.BMSTempInData_f.Temp12) / BMS_B.NumTemp;

  // End of Outputs for SubSystem: '<Root>/SOCEstimation'

  // BusCreator generated from: '<Root>/BMSActualValues' incorporates:
  //   Outport: '<Root>/BMSActualValues'

  BMS_Y.BMSActualValues.Voltage = rtb_Voltage;
  BMS_Y.BMSActualValues.BatCurrent = rtb_MinMax;
}

// Model initialize function
void BMS::initialize()
{
  // Model Initialize function for ModelReference Block: '<Root>/Model'

  // Set error status pointer for ModelReference Block: '<Root>/Model'
  ModelMDLOBJ1.setErrorStatusPointer(rtmGetErrorStatusPointer((&BMS_M)));
}

// Model terminate function
void BMS::terminate()
{
  // (no terminate code required)
}

// Constructor
BMS::BMS() :
  BMS_U(),
  BMS_Y(),
  BMS_B(),
  BMS_M()
{
  // Currently there is no constructor body generated.
}

// Destructor
BMS::~BMS()
{
  // Currently there is no destructor body generated.
}

// Real-Time Model get method
RT_MODEL_BMS_T * BMS::getRTM()
{
  return (&BMS_M);
}

//
// File trailer for generated code.
//
// [EOF]
//
