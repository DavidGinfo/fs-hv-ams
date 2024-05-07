import logging
import os

device_firmware_mapping = {
    "STM32F446RETx": "STM32Cube FW_F4 V1.27.1",
    "STM32G473CETx": "STM32Cube FW_G4 V1.5.1",
    "STM32G473QETx": "STM32Cube FW_G4 V1.5.1",
    "STM32G473RETx": "STM32Cube FW_G4 V1.5.1",
    "STM32G474RETx": "STM32Cube FW_G4 V1.5.1",
    "STM32H725ZGTx": "STM32Cube FW_H7 V1.11.1"
}


def main():
    # Get file path of all files ending with .ioc also in subdirectories
    ioc_files = [
        os.path.join(root, name) for root, dirs, files in os.walk("..")
        for name in files if name.endswith(".ioc")
    ]
    logging.info("Found %s ioc files" % len(ioc_files))
    found_wrong_firmware = False
    for file in ioc_files:
        fireware_version = None
        device_id = None
        project_name = None
        with open(file, "r") as f:
            for line in f:
                if "ProjectManager.FirmwarePackage" in line:
                    fireware_version = line.split("=")[1].strip().strip('"')
                if "ProjectManager.DeviceId" in line:
                    device_id = line.split("=")[1].strip().strip('"')
                if "ProjectManager.ProjectName" in line:
                    project_name = line.split("=")[1].strip().strip('"')
            if not project_name:
                logging.error(f"No project name found in {file}")
                found_wrong_firmware = True
                continue
            if fireware_version and device_id:
                if device_id in device_firmware_mapping:
                    if device_firmware_mapping[device_id] != fireware_version:
                        logging.error(
                            f"Wrong firmware version for device {device_id} in project {project_name}." +
                            f"Expected {device_firmware_mapping[device_id]} " +
                            f"but found {fireware_version}")
                        found_wrong_firmware = True
                    else:
                        logging.info(
                            f"Firmware version for device {device_id} in project {project_name} is correct"
                        )
                else:
                    logging.error(
                        f"No firmware version found for device {device_id} in project {project_name}"
                    )
                    found_wrong_firmware = True

    if found_wrong_firmware:
        exit(1)
    else:
        exit(0)


if __name__ == "__main__":
    logging.basicConfig(level=logging.INFO)
    main()
