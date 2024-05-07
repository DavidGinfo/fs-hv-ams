#include "Init.h"
#include "main.h"
#include <IO/ADC/ADS8698.h>
#include <IO/CAN/generated/CANMessages.h>
#include <parallel/Task.hpp>
#include <IO/EEPROM/EEPROM.h>

using namespace ADS8698;
using namespace Parallel;
using namespace eeprom;

extern SPI_HandleTypeDef hspi1;
extern I2C_HandleTypeDef hi2c3;

// Forward declaration for Task
[[noreturn]] void runADC();
Parallel::NormalTask adcTask(&runADC, "ADC");

Parallel::BinaryChannel channel(adcTask);
SPI::Port &port = SPI::Port::create<&hspi1>();

auto device = SPI::Device(port, GPIO::Pin(ADC_CS_GPIO_Port, ADC_CS_Pin));
auto adc    = ADS8698::ADC(device, channel);

EEPROM eeprom_fcu(0x50, 64, GPIO::Pin(EEPROM_Write_Control_GPIO_Port, EEPROM_Write_Control_Pin), &hi2c3);

Channel *channels[8];

// Fallback values for sensors if nothing saved on EEPROM

float readEEPROMValueOrSetDefault(uint16_t page, uint16_t offset, float defaultValue) {
    float value;
    // Check if there is already a value set at eeprom that is not 0 or 0xFFFFFFFF
    if (!eeprom_fcu.read(page, offset, (uint8_t *) &value, 4)) {
        return defaultValue;
    }
    if (value == 0 || value == 0xFFFFFFFF) {
        // Write default value to EEPROM
        value = defaultValue;
        if (!eeprom_fcu.write(page, offset, (uint8_t *) &value, 4)) {
            return defaultValue;
        }
    } else {
        // Use the stored value in EEPROM
        return value;
    }

}

class CalibratedSensor {
  protected:
    float defaultMin = 0.1;
    float defaultMax = 4.9;
    uint16_t pageEEPROM = 0;

    uint8_t eepromMinValueAdress = 0;
    uint8_t eepromMaxValueAdress = 4;
  public:
    float min = defaultMin;
    float max = defaultMax;

    CAN::Signal valueSignal;

    CAN::Signal minCalibrationSignal;
    CAN::Signal maxCalibrationSignal;

    CAN::Signal actualMinSignal;
    CAN::Signal actualMaxSignal;

    CalibratedSensor(CAN::Signal valueSignal, CAN::Signal minCalibrationSignal, CAN::Signal maxCalibrationSignal, CAN::Signal actualMinSignal, CAN::Signal actualMaxSignal) :
          valueSignal(valueSignal), minCalibrationSignal(minCalibrationSignal), maxCalibrationSignal(maxCalibrationSignal),
            actualMinSignal(actualMinSignal), actualMaxSignal(actualMaxSignal) {}

    virtual void setMinMax() {
        min = readEEPROMValueOrSetDefault(pageEEPROM, eepromMinValueAdress, defaultMin);
        max = readEEPROMValueOrSetDefault(pageEEPROM, eepromMaxValueAdress, defaultMax);
    }

    virtual void calibrate() {
        min = minCalibrationSignal.getPhysicalValue();
        max = maxCalibrationSignal.getPhysicalValue();
        eeprom_fcu.write(pageEEPROM, eepromMinValueAdress, (uint8_t *) &min, 4);
        eeprom_fcu.write(pageEEPROM, eepromMaxValueAdress, (uint8_t *) &max, 4);
    }

    virtual void setActualValues() {
        actualMinSignal.setPhysicalValue(min);
        actualMaxSignal.setPhysicalValue(max);
    }

    uint16_t getPage() {
        return pageEEPROM;
    }

    void setValue(float value) {
        valueSignal.setPhysicalValue(value);
    }

};

class APPS : public CalibratedSensor {
  protected:
    bool apps_A = false;
  public:
    APPS(CAN::Signal valueSignal, CAN::Signal minCalibrationSignal, CAN::Signal maxCalibrationSignal, CAN::Signal actualMinSignal, CAN::Signal actualMaxSignal, bool apps_A = false) :
            CalibratedSensor(valueSignal, minCalibrationSignal, maxCalibrationSignal, actualMinSignal, actualMaxSignal),
            apps_A(apps_A) {
        pageEEPROM = apps_A ? 0 : 1;
    }

    float calculate(float raw) {
        return (raw - min) / (max - min) * 100;
    }
};

class CalibratedSensorWithVoltageAndOtherValues : public CalibratedSensor {
    protected:
       // The other value is for example the minimum spring deflection in mm, or the minimum steering angle in degrees
       CAN::Signal minCalibrationSignalOther;
       CAN::Signal maxCalibrationSignalOther;

       CAN::Signal actualMinSignalOther;
       CAN::Signal actualMaxSignalOther;
       uint8_t eepromMinValueAdressOther = 8;
       uint8_t eepromMaxValueAdressOther = 12;

    public:
      float minOther;
      float maxOther;

        CalibratedSensorWithVoltageAndOtherValues(CAN::Signal valueSignal, CAN::Signal minCalibrationSignal, CAN::Signal maxCalibrationSignal, CAN::Signal actualMinSignal, CAN::Signal actualMaxSignal, CAN::Signal minCalibrationSignalOther, CAN::Signal maxCalibrationSignalOther, CAN::Signal actualMinSignalOther, CAN::Signal actualMaxSignalOther) :
                CalibratedSensor(valueSignal, minCalibrationSignal, maxCalibrationSignal, actualMinSignal, actualMaxSignal),
                minCalibrationSignalOther(minCalibrationSignalOther), maxCalibrationSignalOther(maxCalibrationSignalOther),
                actualMinSignalOther(actualMinSignalOther), actualMaxSignalOther(actualMaxSignalOther) {}

        override void setMinMax() {
            min = readEEPROMValueOrSetDefault(pageEEPROM, eepromMinValueAdress, defaultMin);
            max = readEEPROMValueOrSetDefault(pageEEPROM, eepromMaxValueAdress, defaultMax);
            minOther = readEEPROMValueOrSetDefault(pageEEPROM, eepromMinValueAdressOther, defaultMin);
            maxOther = readEEPROMValueOrSetDefault(pageEEPROM, eepromMaxValueAdressOther, defaultMax);
        }

        override void calibrate() {
            min = minCalibrationSignal.getPhysicalValue();
            max = maxCalibrationSignal.getPhysicalValue();
            eeprom_fcu.write(pageEEPROM, eepromMinValueAdress, (uint8_t *) &min, 4);
            eeprom_fcu.write(pageEEPROM, eepromMaxValueAdress, (uint8_t *) &max, 4);

            minOther = minCalibrationSignalOther.getPhysicalValue();
            maxOther = maxCalibrationSignalOther.getPhysicalValue();
            eeprom_fcu.write(pageEEPROM, eepromMinValueAdressOther, (uint8_t *) &minOther, 4);
            eeprom_fcu.write(pageEEPROM, eepromMaxValueAdressOther, (uint8_t *) &maxOther, 4);

        }

        override void setActualValues() {
            actualMinSignal.setPhysicalValue(min);
            actualMaxSignal.setPhysicalValue(max);
            actualMinSignalOther.setPhysicalValue(minOther);
            actualMaxSignalOther.setPhysicalValue(maxOther);
        }
};

class SpringDeflection : public CalibratedSensorWithVoltageAndOtherValues {
  protected:
    bool left = false;
  public:
    SpringDeflection(CAN::Signal valueSignal, CAN::Signal minCalibrationSignal, CAN::Signal maxCalibrationSignal, CAN::Signal actualMinSignal, CAN::Signal actualMaxSignal, CAN::Signal minCalibrationSignalOther, CAN::Signal maxCalibrationSignalOther, CAN::Signal actualMinSignalOther, CAN::Signal actualMaxSignalOther, bool left) :
            CalibratedSensorWithVoltageAndOtherValues(valueSignal, minCalibrationSignal, maxCalibrationSignal, actualMinSignal, actualMaxSignal, minCalibrationSignalOther, maxCalibrationSignalOther, actualMinSignalOther, actualMaxSignalOther), left(left) {
            pageEEPROM = left ? 2 : 3;
    }

    float calculate(float raw) {
        return (raw - min) / (max - min) * (maxOther - minOther) + minOther;
    }
};

class SteeringAngle : public CalibratedSensorWithVoltageAndOtherValues {
  public:
    SteeringAngle(CAN::Signal valueSignal, CAN::Signal minCalibrationSignal, CAN::Signal maxCalibrationSignal, CAN::Signal actualMinSignal, CAN::Signal actualMaxSignal, CAN::Signal minCalibrationSignalOther, CAN::Signal maxCalibrationSignalOther, CAN::Signal actualMinSignalOther, CAN::Signal actualMaxSignalOther) :
            CalibratedSensorWithVoltageAndOtherValues(valueSignal, minCalibrationSignal, maxCalibrationSignal, actualMinSignal, actualMaxSignal, minCalibrationSignalOther, maxCalibrationSignalOther, actualMinSignalOther, actualMaxSignalOther) {
        pageEEPROM = 4;
    }
    float calculate(float raw) {
        return (raw - min) / (max - min) * (maxOther - minOther) + minOther;
    }
};

float calculate_BP(float raw) {
    // Voltage in between 0 and 5V, 0V = 0 bar, 5V = 200 bar, linear
    return raw / 5 * 200;
}

APPS appsA(FCU_APPS::FCU_APPS_A,
        FCU_APPS_Calibration::FCU_APPS_A_Calibration_Min,
           FCU_APPS_Calibration::FCU_APPS_A_Calibration_Max,
           FCU_APPS_Current_Min_Max::FCU_APPS_A_Actual_Min,
           FCU_APPS_Current_Min_Max::FCU_APPS_A_Actual_Max,
           true);

APPS appsB(FCU_APPS::FCU_APPS_B,
        FCU_APPS_Calibration::FCU_APPS_B_Calibration_Min,
           FCU_APPS_Calibration::FCU_APPS_B_Calibration_Max,
           FCU_APPS_Current_Min_Max::FCU_APPS_B_Actual_Min,
           FCU_APPS_Current_Min_Max::FCU_APPS_B_Actual_Max,
           false);

SpringDeflection sdLeft(FCU_SA_SD::FCU_SpringDeflection_L,
                      FCU_SD_L_Calibration::FCU_SpringDeflection_L_Cal_Min_V,
                      FCU_SD_L_Calibration::FCU_SpringDeflection_L_Cal_Max_V,
                      FCU_SD_L_Current_Min_Max::FCU_SpringDeflection_L_Act_Min_V,
                      FCU_SD_L_Current_Min_Max::FCU_SpringDeflection_L_Act_Max_V,
                      FCU_SD_L_Calibration::FCU_SpringDeflection_L_Cal_Min_W,
                      FCU_SD_L_Calibration::FCU_SpringDeflection_L_Cal_Max_W,
                      FCU_SD_L_Current_Min_Max::FCU_SpringDeflection_L_Act_Min_W,
                      FCU_SD_L_Current_Min_Max::FCU_SpringDeflection_L_Act_Max_W,
                      true);

SpringDeflection sdRight(FCU_SA_SD::FCU_SpringDeflection_R,
                       FCU_SD_R_Calibration::FCU_SpringDeflection_R_Cal_Min_V,
                       FCU_SD_R_Calibration::FCU_SpringDeflection_R_Cal_Max_V,
                       FCU_SD_R_Current_Min_Max::FCU_SpringDeflection_R_Act_Min_V,
                       FCU_SD_R_Current_Min_Max::FCU_SpringDeflection_R_Act_Max_V,
                       FCU_SD_R_Calibration::FCU_SpringDeflection_R_Cal_Min_W,
                       FCU_SD_R_Calibration::FCU_SpringDeflection_R_Cal_Max_W,
                       FCU_SD_R_Current_Min_Max::FCU_SpringDeflection_R_Act_Min_W,
                       FCU_SD_R_Current_Min_Max::FCU_SpringDeflection_R_Act_Max_W,
                       false);

SteeringAngle sa(FCU_SA_SD::FCU_SteeringAngle,
               FCU_SA_Calibration::FCU_SteeringAngle_Cal_Min_V,
               FCU_SA_Calibration::FCU_SteeringAngle_Cal_Max_V,
               FCU_SA_Current_Min_Max::FCU_SteeringAngle_Act_Min_V,
               FCU_SA_Current_Min_Max::FCU_SteeringAngle_Act_Max_V,
               FCU_SA_Calibration::FCU_SteeringAngle_Cal_Min_Deg,
                FCU_SA_Calibration::FCU_SteeringAngle_Cal_Max_Deg,
                FCU_SA_Current_Min_Max::FCU_SteeringAngle_Act_Min_Deg,
                FCU_SA_Current_Min_Max::FCU_SteeringAngle_Act_Max_Deg);

void calibrateAPPSOnCalibrationMessage() {
    appsA.calibrate();
    appsB.calibrate();
}

void runADC() {
    // Init ADC and Channels
    adc.init();
    for (int i = 0; i < 8; ++i) {
        if (!channels[i]->read()) continue;
        channels[i] = adc.getChannel(i);
        channels[i]->setRange(&RANGE_25_25);
    }

    float voltages[8];
    uint32_t raws[8];

    // Register handler for calibration message for APPS, SD and SA
    // Message is sent by Diagnose / MCU
    FCU_APPS_Calibration::message.setHandler(&calibrateAPPSOnCalibrationMessage);
    FCU_SD_L_Calibration::message.setHandler(&sdLeft.calibrate);
    FCU_SD_R_Calibration::message.setHandler(&sdRight.calibrate);
    FCU_SA_Calibration::message.setHandler(&sa.calibrate);

    // Set Min/Max values based on eeprom or default values
    appsA.setMinMax();
    appsB.setMinMax();
    sdLeft.setMinMax();
    sdRight.setMinMax();
    sa.setMinMax();

    /* Main Loop */
    while (true) {

        for (int i = 0; i < 8; ++i) {
            if (!channels[i]->read()) continue;
            voltages[i] = channels[i]->getVoltage();
            raws[i]     = channels[i]->getRaw();
        }

        // Set Data
        FCU_ADC_RAW_2::FCU_ADC_RAW_SD_R.setPhysicalValue(voltages[0]);
        FCU_ADC_RAW_2::FCU_ADC_RAW_SD_L.setPhysicalValue(voltages[1]);
        FCU_ADC_RAW_1::FCU_ADC_RAW_APPS_B.setPhysicalValue(voltages[2]);
        FCU_ADC_RAW_1::FCU_ADC_RAW_APPS_A.setPhysicalValue(voltages[3]);
        FCU_ADC_RAW_1::FCU_ADC_RAW_BP_R.setPhysicalValue(voltages[4]);
        FCU_ADC_RAW_1::FCU_ADC_RAW_BP_F.setPhysicalValue(voltages[5]);
        FCU_ADC_RAW_2::FCU_ADC_RAW_SA.setPhysicalValue(voltages[6]);
        FCU_ADC_RAW_2::FCU_ADC_RAW_RH.setPhysicalValue(voltages[7]);

        // Set current min and max values on CAN for diagnose porpuse
        appsA.setActualValues();
        appsB.setActualValues();
        sdLeft.setActualValues();
        sdRight.setActualValues();
        sa.setActualValues();

        // Set values
        appsA.setValue(appsA.calculate(voltages[3]));
        appsB.setValue(appsB.calculate(voltages[2]));
        sdLeft.setValue(sdLeft.calculate(voltages[1]));
        sdRight.setValue(sdRight.calculate(voltages[0]));
        sa.setValue(sa.calculate(voltages[6]));


        FCU_BP::FCU_BP_F.setPhysicalValue(calculate_BP(voltages[5]));
        FCU_BP::FCU_BP_R.setPhysicalValue(calculate_BP(voltages[4]));

        delay(5);

    }
}

void initADC() {
    adcTask.init();
}

