#include <IO/fuses/EFuse.h>

using namespace EFuse;

void BaseEFuse::enableFuse() {
    enablePin.write(false);
    enableSignal.setRawValue(1);
}

void BaseEFuse::disableFuse() {
    enablePin.write(true);
    enableSignal.setRawValue(0);
}

void BaseEFuse::setDefaultState() {
    if (defaultEnabled) {
        enableFuse();
    } else {
        disableFuse();
    }
}

void TPS259570::updateStatus() {
    faultSignal.setRawValue(faultPin.read());
}