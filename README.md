# Project: IR Controlled Air Conditioner with Wemos D1 mini and IR Emitter (PlatformIO)

This project aims to control an air conditioner using an infrared (IR) emitter controlled by a Wemos D1 mini microcontroller. The device will read four values via MQTT (enable, disable, timer, adjust) to control the air conditioner remotely.

### Hardware Requirements:

- Wemos D1 mini (ESP8266)
- Infrared (IR) emitter
- Connection to a Wi-Fi network with internet access

### Software Requirements:

- PlatformIO development platform
- Necessary libraries for IR communication and MQTT
- MQTT broker for communication
- Node-RED for creating the control flow

### Setup:

1. **Hardware:**
   - Connect the IR emitter to the appropriate pin of the Wemos D1 mini.
   - Power the Wemos D1 mini with an appropriate power source.

2. **Software:**
   - Open the project in PlatformIO.
   - Ensure that the `platformio.ini` file is configured correctly with the compilation and device setup options.
   - Install the necessary libraries for IR communication and MQTT.
   - Configure the Wi-Fi connection parameters and MQTT server settings in the code.
   - Compile and upload the code to the Wemos D1 mini.
   - Set up Node-RED on a compatible device connected to the same network.
   - Create a Node-RED flow to handle the MQTT messages for enable, disable, timer, and adjust commands.
   - Include nodes in the flow to monitor the room temperature and send timer or adjust commands accordingly.

### Usage:

Once the device is powered and connected to the Wi-Fi network, it will listen for MQTT messages to control the air conditioner. The following four values can be sent via MQTT:
- `enable`: Enable the air conditioner control.
- `disable`: Disable the air conditioner control.
- `off`: Send the command to disconnect the air conditioner.
- `timer`: Send command to enable the timer in 1 hour to power off the air conditioner.
- `adjust`: Adjust the air conditioner temperature to a predetermined value.
- `alive`: Send the command alive

The control flow created in Node-RED will handle these MQTT messages and send the corresponding commands to the Wemos D1 mini. Additionally, it will monitor the room temperature and send timer or adjust commands accordingly.

### Additional Notes:

- Ensure that the MQTT broker supports the required topics for sending control commands to the device.
- Implement appropriate safety measures to prevent unauthorized access to the air conditioner control system.

### Contribution:

Contributions are welcome! If you'd like to improve this project, feel free to open an issue or submit a pull request.

### Credits:

This project was developed by [@cgasper79](https://github.com/cgasper79).

### License:

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.
