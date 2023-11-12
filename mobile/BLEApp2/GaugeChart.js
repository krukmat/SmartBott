import React from 'react';
import { View, Text } from 'react-native';
import { Svg, Circle, Text as SvgText } from 'react-native-svg';

export const GaugeChart = ({ value, isLitreMode }) => {
  const radius = 80;
  const circumference = 2 * Math.PI * radius;
  const strokeWidth = 10;
  const dashArray = isLitreMode
    ? `${(value / 6) * circumference} ${circumference}`
    : `${(value / 10) * circumference} ${circumference}`;

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
          transform={`rotate(90 ${radius} ${radius})`}
        />
        {value >= 10 && !isLitreMode && (
          <SvgText
            x={radius}
            y={radius + 10}
            textAnchor="middle"
            fontSize="18"
            fill="green" // You can adjust the color as needed
          >
            Success :)
          </SvgText>
        )}
      </Svg>
      <Text style={{ textAlign: 'center', fontSize: 18 }}>
        {isLitreMode ? `Litres remaining: ${1.5 * value}` : `Bottles consumed: ${value}`}
      </Text>
    </View>
  );
};

