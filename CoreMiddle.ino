/*********************************************************************
 *               DWIN DGUS2 <--> PROCULOS MIDDLEWARE                 *
 *                                                                   *
 *   This code aims to convert the Proculos commands into the DGUS2  *
 *  commands and also the DGUS2 responses into Proculos responses.   *
 *  It makes possible Proculos based devices/equipments to communi-  *
 *  cate with DWIN DGUS2 LCMs.                                       *
 *                                                                   *
 *            Developed/Written by: Felipe G. Almeida                * 
 *                        FGApps © 2020                              *
 *                                                                   *
 *********************************************************************
 *                        Conversion core                            *
 *********************************************************************/

void setup() {
  Serial.begin(115200);                       // Output for DEBUG console
  
  Serial1.begin(115200);                      // Input from Proculos
  Serial2.begin(115200);                      // Output to DWIN DGUS2
}

void loop() {
  analyseCommands(false, &stage_c);
  analyseCommands(true, &stage_r);
}

void analyseCommands(bool resp, unsigned char* stage){
  
  if(Serial1.available() && !resp){           // Command from Proculos
    cb_read = Serial1.read();
    //cb_read = convertSerialInput(cb_read);  // DEBUG/Test only (change input to Serial.read())
  } else if(Serial2.available() && resp){     // Response from DGUS2
    rb_read = Serial2.read(); 
    //rb_read = convertSerialInput(rb_read);  // DEGUB/Test only (change input to Serial.read())
  } else {                                    //No data available
    return;
  }

  switch(*stage){
    
    // Header stage //
    case 0: {
      if(resp) headerStage(&rb_read, &rb_last, resp);
      else headerStage(&cb_read, &cb_last, resp);
      break;
    }
    
    // Size stage //
    case 1: {
      if(resp) sizeStage(&rb_read, &rb_size, resp);
      else sizeStage(&cb_read, &cb_size, resp);
      break;
    }

    // Command stage //
    case 2: {
      if(resp) commandStage(&rb_read, &rb_size, resp);
      else commandStage(&cb_read, &cb_size, resp);
      break;
    }

    // REG-VP stage //
    case 3: {
      if(resp) regVpStage(&rb_read, &rb_size, resp);
      else regVpStage(&cb_read, &cb_size, resp);
      break;
    }

    // Write stage //
    case 4: {
      if(resp) writeStage(&rb_read, &rb_size, resp);
      else writeStage(&cb_read, &cb_size, resp);
      break;
    }
    
  }

}
