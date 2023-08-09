// BleManager.js
import { BleManager, Device } from 'react-native-ble-plx';
import base64 from 'react-native-base64'
var Buffer = require('buffer/').Buffer;

const bleManager = new BleManager();
export let discoveredDevices = [];
let deviceId;

export const scanForDevices = async () => {
  try {
    const devices = await bleManager.startDeviceScan(['19B10001-E8F2-537E-4F6C-D104768A1214'], null, (error, device) => {
        if (error) {
          // Handle error
          console.log(error);
        }
        if (device) {
          // Device was discovered
          console.log(device.localName);
          console.log(device.id);
          if (discoveredDevices.length<= 0 && device.localName == 'Hydro')
            discoveredDevices.push(device);
            deviceId = device.id;
            return device;
        }

      });
      console.info(devices);
    return devices;
  } catch (error) {
    console.error('Error scanning for devices:', error);
    throw error;
  }
};

export const connectToDevice = async (device) => {
  try {
    await device.connect();
    const services = await device.discoverAllServicesAndCharacteristics();
    const data = await services.characteristicsForService('19B10001-E8F2-537E-4F6C-D104768A1214');
    console.log('connectToDevice: ');
    console.log(services);
    console.log('data');
    console.log(data);
    return data;
  } catch (error) {
    console.error('Error connecting to device:', error);
    throw error;
  }
};

export const readCharacteristic = async (characteristic) => {
  try {
    console.log("char: ");
    console.log(characteristic);
    const data = await characteristic.read();
    console.log(data);
    const integerValue = Buffer.from(data.value, 'base64').readUIntLE(0,2);
    await characteristic.writeWithResponse([1]);
    console.log('Integer value:', integerValue);
    bleManager.cancelDeviceConnection(deviceId);
    //TODO: Desconectar el device
    return integerValue;
  } catch (error) {
    console.error('Error reading characteristic:', error);
    throw error;
  }
};