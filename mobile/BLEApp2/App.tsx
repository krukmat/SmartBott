import {Picker} from '@react-native-picker/picker';
import React, { useState, useEffect } from 'react';
import { View, Text, Button, FlatList, TouchableOpacity, Modal } from 'react-native';
import { scanForDevices, connectToDevice, readCharacteristic, discoveredDevices,  sendVolume} from './BleManager';
import { GaugeChart } from './GaugeChart';


const App = () => {
  const [devices, setDevices] = useState([]);
  const [selectedDevice, setSelectedDevice] = useState(null);
  const [selectedService, setSelectedService] = useState(null);
  const [integerValue, setIntegerValue] = useState(0);

  // New state for the picklist
  const [isPicklistVisible, setIsPicklistVisible] = useState(false);
  const [selectedOption, setSelectedOption] = useState(''); // To store the selected option

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
    setSelectedService(null);
    setIsPicklistVisible(false); // Close the picklist when selecting a new device
    try {
      const services = await connectToDevice(device);
      setSelectedService(services);
    } catch (error) {
      console.error('Error connecting to device or discovering services:', error);
    }
  };

  const handleSelectService = (service) => {
    setSelectedService(service);
  };

  const handleReadValue = async () => {
    if (selectedService) {
      try {
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

  // Function to handle the selection of an option from the picklist
  const handleOptionSelect = async (option) => {
    setSelectedOption(option);
    setIsPicklistVisible(false);
    if (selectedService) {
      await sendVolume(selectedService[0], option);
    }
  };

  useEffect(() => {
    setDevices([...discoveredDevices]);
  }, [discoveredDevices]);

  return (
    <View style={{ flex: 0, justifyContent: 'center', alignItems: 'center' }}>
      <Text>Scanner</Text>
      {!selectedDevice ? (
        <Button title="Scan for SmartBotts" onPress={handleScanDevices} />
      ) : (
        <View>
          <Text></Text>
        </View>
      )}
      {!selectedDevice ? (
        <FlatList
          data={devices}
          keyExtractor={(item) => item.id}
          renderItem={({ item }) => (
            <TouchableOpacity onPress={() => handleSelectDevice(item)}>
              <View style={{ padding: 10, marginBottom: 10 }}>
                  <Text>{item.localName || 'Unknown Device'}</Text>
              </View>
            </TouchableOpacity>
          )}
        />
      ) : (
        <View>
          <Text>Selected Device: {selectedDevice.localName || 'Unknown Device'}</Text>
          {selectedService ? (
            <View>
              {/* Button to open the picklist */}
              <Button title="Select Volume" onPress={() => setIsPicklistVisible(true)} />
              {/* Picklist */}
              <Modal visible={isPicklistVisible}>
              <View>
              <Picker
                      selectedValue={selectedOption}
                      onValueChange={(itemValue, itemIndex) => handleOptionSelect(itemValue)}
                    >
                      <Picker.Item label="500 cc" value="0" />
                      <Picker.Item label="1 litre" value="1" />
                      <Picker.Item label="1.5 litres" value="2" />
                    </Picker>

                <Button title="Close" onPress={() => setIsPicklistVisible(false)} />
              </View>
            </Modal>
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
