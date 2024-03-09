import {Picker} from '@react-native-picker/picker';
import React, { useState, useEffect } from 'react';
import { View, Text, Button, FlatList, TouchableOpacity, Modal } from 'react-native';
import { scanForDevices, connectToDevice, readCharacteristic, discoveredDevices,  sendVolume, sendStartCount} from './BleManager';
import { GaugeChart } from './GaugeChart';
import AsyncStorage from '@react-native-async-storage/async-storage';
import DateTimePicker from '@react-native-community/datetimepicker';



const App = () => {
  const [devices, setDevices] = useState([]);
  const [selectedDevice, setSelectedDevice] = useState(null);
  const [selectedService, setSelectedService] = useState(null);
  const [integerValue, setIntegerValue] = useState(0);
  const [startDate, setStartDate] = useState(null);
  const [endDate, setEndDate] = useState(null);

  let bottleMode = '3';

  // New state for the picklist
  const [isPicklistVisible, setIsPicklistVisible] = useState(false);
  const [selectedOption, setSelectedOption] = useState(''); // To store the selected option
  const [isDatePickerModalVisible, setIsDatePickerModalVisible] = useState(false);
  const [activeDatePicker, setActiveDatePicker] = useState(null);
  


  
  const toggleDatePickerModal = (datePickerType) => {
    setActiveDatePicker(datePickerType);
    setIsDatePickerModalVisible(!isDatePickerModalVisible);
  };

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
      await setSelectedService(services);
      // Llama a handleReadValue después de seleccionar el servicio
      if (services.length > 0) {
        setSelectedService(services[0]);
        const value = await readCharacteristic(services[0], bottleMode);
        setIntegerValue(value);
  
      }
    } catch (error) {
      console.error('Error connecting to device or discovering services:', error);
    }
  };

// ...



const storeData = async (value) => {
  try {
    const timestamp = new Date().toISOString();
    const data = { value, timestamp };
    await AsyncStorage.setItem('integerValueData', JSON.stringify(data));
  } catch (error) {
    console.error('Error storing data:', error);
  }
};

const retrieveData = async () => {
  try {
    const storedData = await AsyncStorage.getItem('integerValueData');
    if (storedData !== null) {
      const { value, timestamp } = JSON.parse(storedData);
      console.log('Retrieved data:', { value, timestamp });
      // Puedes manejar los datos recuperados como desees.
    } else {
      console.log('No data stored yet.');
    }
  } catch (error) {
    console.error('Error retrieving data:', error);
  }
};

const retrieveDataBetweenDates = async (startDate, endDate) => {
  try {
    const allData = await AsyncStorage.getAllKeys();
    const filteredData = await AsyncStorage.multiGet(allData);
    
    const dataBetweenDates = filteredData
      .map(([key, value]) => JSON.parse(value))
      .filter(({ timestamp }) => {
        const date = new Date(timestamp);
        return date >= startDate && date <= endDate;
      });

    console.log('Data between dates:', dataBetweenDates);
  } catch (error) {
    console.error('Error retrieving data between dates:', error);
  }
};

const handleReadValue = async () => {
  if (selectedService) {
    try {
      const value = await readCharacteristic(selectedService[0], bottleMode);
      setIntegerValue(value);
      storeData(value); // Almacena el valor junto con la fecha/hora
      handleSelectDevice(devices[0]);
    } catch (error) {
      console.error('Error reading value:', error);
    }
  } else {
    console.warn('No service selected.');
  }
};

  // Function to handle the selection of an option from the picklist
  const handleOptionSelect = async (option: string) => {
    bottleMode = option;
    setSelectedOption(option);
    setIsPicklistVisible(false);
    if (selectedService) {
      await sendVolume(selectedService[0], option);
    }
  };

  useEffect(() => {
    setDevices([...discoveredDevices]);
    retrieveData();
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
            <Button title="Get latest data" onPress={handleReadValue} />
            <GaugeChart
              value={integerValue}
              isLitreMode={bottleMode === '3'}
              />
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
