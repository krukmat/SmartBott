import React from 'react';
import { View, Text } from 'react-native';
import { Svg, Circle } from 'react-native-svg';

export const GaugeChart = ({ value }) => {
  const radius = 80;
  const circumference = 2 * Math.PI * radius;
  const strokeWidth = 10;
  const dashArray = `${(value / 10) * circumference} ${circumference}`;

  return (
    <View>
      <Svg width={2 * radius} height={2 * radius}>
        <Circle
          cx={radius}
          cy={radius}
          r={radius - strokeWidth / 2}
          fill="transparent"
          stroke="#3498db"
          strokeWidth={strokeWidth}
          strokeDasharray={dashArray}
          strokeLinecap="round"
          transform={`rotate(90 ${radius} ${radius})`} // Rotate the gauge to start from the top
        />
      </Svg>
      <Text style={{ textAlign: 'center', fontSize: 18 }}>
        Bottles consumed: {value}
      </Text>
    </View>
  );
};
