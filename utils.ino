// Function for putting the sensor values in a value HTTP query string
String generateSensorQueryString() {
  String queryString = "?";
  for(int j=0; j<SENSOR_VALUE_AMOUNT; j++) {
    queryString += SENSOR_LABELS[j] + "=" + sensorValues[j]; 
    if (j != SENSOR_VALUE_AMOUNT-1) {
      queryString += "&";
    }
  }

  return queryString;
}

