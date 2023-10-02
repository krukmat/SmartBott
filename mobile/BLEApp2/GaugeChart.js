import { Svg, Circle } from 'react-native-svg';

export const GaugeChart = ({ value }) => {
    const radius = 80; // Adjust the radius as needed
    const circumference = 2 * Math.PI * radius;
    const strokeWidth = 10; // Adjust the stroke width as needed
  
    // Calculate the stroke-dasharray to represent the value as a percentage of the circumference
    const dashArray = `${(value / 100) * circumference} ${circumference}`;
  
    return (
      <Svg width={2 * radius} height={2 * radius}>
        <Circle
          cx={radius}
          cy={radius}
          r={radius - strokeWidth / 2}
          fill="transparent"
          stroke="#3498db" // Adjust the color as needed
          strokeWidth={strokeWidth}
          strokeDasharray={dashArray}
          strokeLinecap="round"
        />
      </Svg>
    );
  };