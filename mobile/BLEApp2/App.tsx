// App.js
import React, { useState, useEffect } from 'react';
import { View, Text, Button, FlatList, TouchableOpacity } from 'react-native';
import { scanForDevices, connectToDevice, readCharacteristic, discoveredDevices } from './BleManager';
import {GaugeChart} from './GaugeChart';

const App = () => {
  const [devices, setDevices] = useState([]);
  const [selectedDevice, setSelectedDevice] = useState(null);
  const [selectedService, setSelectedService] = useState(null);
  const [integerValue, setIntegerValue] = useState(0);

  const handleScanDevices = async () => {
    try {
      const scannedDevices = await scanForDevices();
      setDevices([...discoveredDevices]);
    } catch (error) {
      console.error('Error scanning devices:', error);
    }
  };

  const handleSelectDevice = async (device) => {
    setSelectedDevice(device);
    console.log('device');
    console.log(device);
    //try {
      const services = await connectToDevice(device);
      setSelectedService(null); // Reset selected service when connecting to a new device
      console.log("services");
      console.log(services);
      const discoveredServices = services;
      console.log("discoveredServices:");
      console.log(discoveredServices);
      setSelectedService(discoveredServices); // Update devices list
    /*} catch (error) {
      console.error('Error connecting to device or discovering services:', error);
    }*/
  };

  const handleSelectService = (service) => {
    setSelectedService(service);
  };

  const handleReadValue = async () => {
    if (selectedService) {
      try {
        console.info(selectedService);
        const value = await readCharacteristic(selectedService[0]);
        setIntegerValue(value);
        handleSelectDevice(devices[0]);
      } catch (error) {
        console.error('Error reading value:', error);
      }
    } else {
      console.warn('No service selected.');
    }
  };

  useEffect(() => {
    setDevices([...discoveredDevices]); // Update devices list when discoveredDevices changes
  }, [discoveredDevices]);

  return (
    <View style={{ flex: 0, justifyContent: 'center', alignItems: 'center' }}>
      <Text>Scanner</Text>
      <Button title="Scan for SmartBotts" onPress={handleScanDevices} />
      <FlatList
        data={devices}
        keyExtractor={(item) => item.id}
        renderItem={({ item }) => (
          <TouchableOpacity onPress={() => handleSelectDevice(item)}>
            <Text>{item.localName || 'Unknown Device'}</Text>
          </TouchableOpacity>
        )}
      />
      {selectedDevice && (
        <View>
          <Text>Selected Device: {selectedDevice.localName || 'Unknown Device'}</Text>
          {selectedService ? (
            <View>
              <Button title="Get data" onPress={handleReadValue} />
              <GaugeChart value={integerValue} />
            </View>
          ) : (
            <View>
              <Text>Please wait</Text>
            </View>
          )}
          </View>
        )}
    </View>
    
  );
};

export default App;
