unsigned char ECU_PID[] = {
  /* Engine Load */                 0x04,
  /* Engine coolant temp */         0x05,
  /* Fuel Pressure */               0x0A,
  /* Engine RPM */                  0x0C,
  /* Vehicle Speed (kph) */         0x0D,
  /* Timing Advance */              0x0E,
  /* Intake Air Temperature */      0x0F,
  /* Air mass flow rate */          0x10,
  /* throttle position */           0x11,
  /* runtime since engine start */  0x1F,
  /* Fuel Level */                  0x2F,
  /* Absolute Pressure */           0x33,
  /* engine oil temperature */      0x5C,
  /* Fuel injection timing */       0x5D,
  /* Fuel rate */                   0x5E,
  /* Torque percentage */           0x62,
  /* engine reference torque */     0x63,
};

unsigned char ECU_PID_msglen[] = {
  /* Engine Load */                 1,
  /* Engine coolant temp */         1,
  /* Fuel Pressure */               1,
  /* Engine RPM */                  2,
  /* Vehicle Speed (kph) */         1,
  /* Timing Advance */              1,
  /* Intake Air Temperature */      1,
  /* Air mass flow rate */          2,
  /* throttle position */           1,
  /* runtime since engine start */  2,
  /* Fuel Level */                  1,
  /* Absolute Pressure */           1,
  /* engine oil temperature */      1,
  /* Fuel injection timing */       2,
  /* Fuel rate */                   2,
  /* Torque percentage */           1,
  /* engine reference torque */     2,
};

long requestECUdata(unsigned char pid_index){
  // create data output buffer
  unsigned char data_out[9] = {0, 0, 0, 0, 0, 0, 0, 0};
  // additional data bytes
  data_out[0] = ECU_PID_msglen[pid_index];
  // mode
  data_out[1] = 0x01;
  // PID
  data_out[2] = ECU_PID[pid_index];
  // send data request
  CAN0.sendMsgBuf(0x7DF, 0, 8, data_out);
  // wait for response of ID 0x7E8
  do{
    while(digitalRead(16));
    CAN0.readMsgBuf(&len, data_in);
    rxId = CAN0.getCanId();
  }while(rxId != 0x7E8);
  // switch case for different data
  long output = 0;
  switch(pid_index){
    case ENGINE_LOAD:
      output = (100 * data_in[3]) >> 8;
      break;
    case COOLANT_TEMP:
      output = data_in[3] - 40;
      break;
    case FUEL_PRESSURE:
      output = 3*data_in[3];
      break;
    case ENGINE_RPM:
      output = (data_in[3] << 8) + data_in[4];
      output = output >> 2;
      break;
    case VEHICLE_KPH:
      output = data_in[3];
      break;
    case TIMING_ADVANCE:
      output = (data_in[3] >> 1) - 64;
      break;
    case INTAKE_AIR_TEMP:
      output = data_in[3] - 40;
      break;
    case AIR_FLOW_RATE:
      output = (data_in[3] << 8) + data_in[4];
      output = output >> 2;
      break;
    case THROTTLE_POS:
      output = (100 * data_in[3]) >> 8;
      break;
    case RUNTIME:
      output = (data_in[3] << 8) + data_in[4];
      break;
    case FUEL_LEVEL:
      output = (100 * data_in[3]) >> 8;
      break;
    case ABSOLUTE_PRESSURE:
      output = data_in[3];
      break;
    case OIL_TEMP:
      output = data_in[3] - 40;
      break;
    case FUEL_INJECTION_TIMING:
      output = (data_in[3] << 8) + data_in[4];
      output = (output >> 7) - 210;
      break;
    case FUEL_RATE:
      output = (data_in[3] << 8) + data_in[4];
      output /= 20;
      break;
    case TORQUE_PERCENT:
      output = data_in[3] - 125;
      break;
    case REFERENCE_TORQUE:
      output = (data_in[3] << 8) + data_in[4];
      break;
  }
  return output;
}
