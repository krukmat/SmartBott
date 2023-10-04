// BleManager.js
import { BleManager, Device } from 'react-native-ble-plx';
import base64 from 'react-native-base64'

var Buffer = require('buffer/').Buffer;

const bleManager = new BleManager();
export let discoveredDevices = [];
let totalbottles = 0;
let deviceId;

export const scanForDevices = async () => {
  try {
    const devices = await bleManager.startDeviceScan(['19B10001-E8F2-537E-4F6C-D104768A1214'], null, (error, device) => {
        if (error) {
          // Handle error
          console.log('Error detected in scanning process');
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

export const sendVolume = async(characteristic, volume) => {
  let adata = []; //black magic
  if (volume === "0")
      adata = [0x02, 0x03]; // Your data as an array of hexadecimal values
  if (volume === "1")
    adata = [0x03, 0x04]; // Your data as an array of hexadecimal values
  if (volume === "2")
    adata = [0x05, 0x06]; // Your data as an array of hexadecimal values

  const dataString = adata.map(byte => byte.toString(16)).join('');
  //const dataString = volume.split("").map(c => c.charCodeAt(0).toString(16).padStart(2, "0")).join("");
  console.log(dataString);
  await characteristic.writeWithResponse(dataString);
  return 0;

}

export const readCharacteristic = async (characteristic) => {
  try {
    console.log("char: ");
    console.log(characteristic);
    const data = await characteristic.read();
    console.log(data.value);
    let integerValue = 0;
    if(data.value !== null){
      integerValue = Buffer.from(data.value, 'base64').readUIntLE(0,2);
      if ((integerValue > 0) && (integerValue<100)){
        totalbottles+=integerValue;
      }
    }
    const adata = [0x01, 0x02]; // Your data as an array of hexadecimal values
    const dataString = adata.map(byte => byte.toString(16)).join('');
    console.log('send: ', dataString);
    await characteristic.writeWithResponse(dataString);
    console.log('1. Integer value:', integerValue);
    bleManager.cancelDeviceConnection(deviceId);
    console.log('2. totalBottles:', totalbottles);
    //TODO: Desconectar el device
    return totalbottles;
  } catch (error) {
    console.error('Error reading characteristic:', error);
    throw error;
  }
};