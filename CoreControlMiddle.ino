/*********************************************************************
 *               DWIN DGUS2 <--> PROCULOS MIDDLEWARE                 *
 *                                                                   *
 *   This code aims to convert the Proculos commands into the DGUS2  *
 *  commands and also the DGUS2 responses into Proculos responses.   *
 *  It makes possible Proculos based devices/equipments to communi-  *
 *  cate with DWIN DGUS2 LCMs.                                       *
 *                                                                   *
 *            Developed/Written by: Felipe G. Almeida                * 
 *                    AGTechnologies © 2020                          *
 *                                                                   *
 *********************************************************************
 *                     Core Control functions                        *
 *********************************************************************/
 
/*** Stage control ***/
void next(bool resp){
  if(resp){                                   // Go to next stage of the response analisys
    stage_r++;
    
  } else {                                    // Go to next stage of the command analisys
    stage_c++;
  }
}

void resetCmd(String n){
  Serial.println("*** RESET CMD from "+n+" ***");
  
  stage_c = 0x00;
  
  cb_read = 0x00;
  cb_last = 0x00;
  cb_size = 0x00;

  typ_cmd = 0x00;
  vph_cmd = 0x00;
  vpl_cmd = 0x00;
  vpf_cmd = 0x00;
  len_cmd = 0x00;
}

void resetRes(String n){
  Serial.println("*** RESET RES from "+n+" ***");
  
  stage_r = 0x00;
  
  rb_read = 0x00;
  rb_last = 0x00;
  rb_size = 0x00;

  typ_res = 0x00;
  vph_res = 0x00;
  vpl_res = 0x00;
  vpf_res = 0x00;
  len_res = 0x00;
}

/*** Send data control ***/
void sendByte(unsigned char c, bool resp){
  if(resp){
    Serial1.print(c);
    Serial.print("<<< ");
    Serial.println(c, HEX);
  } else {
    Serial2.print(c);
    Serial.print(">>> ");
    Serial.println(c, HEX);
  }
}

void sendKnownBytes(bool resp){
  sendByte(0x5A, resp);                       // HEADER1
  sendByte(0xA5, resp);                       // HEADER2
  
  if(resp){
    sendByte(len_res, resp);                  // LENGTH
    sendByte(typ_res, resp);                  // COMMAND
    
  } else {
    sendByte(len_cmd, resp);                  // LENGTH
    sendByte(getDUGS2Command(typ_cmd), resp); // COMMAND
  }
}

void sendVPBytes(bool resp){
  if(resp){
    if(vpf_res == 0x02){
      sendByte(vph_res, resp);                // VPH
    }
    sendByte(vpl_res, resp);                  // VPL
  } else {
    sendByte(vph_cmd, resp);                  // VPH
    sendByte(vpl_cmd, resp);                  // VPL
  }
}

/*** Command control ***/
unsigned char getDUGS2Command(unsigned char c){
  if(c == 0x80) return 0x82;                  // Write
  if(c == 0x81) return 0x83;                  // Read
  return c;
}

unsigned char getProculosCommand(unsigned char vph, unsigned char vpl){
  if(vph == 0x00){                            // 0x00 VP Highbyte may be a register response
    
    for(int i=0; i<10; i++){
      
      if(registers[i].w_vp[1] == vpl){        // Iterates over the register map struct array
        vph_res = 0x00;
        vpl_res = registers[i].value;         // Set the value of the REG over the VP Lowbite
        len_res--;                            // VP -> REG delta size (-1)
        return 0x80;
        
      } else if(registers[i].r_vp[1] == vpl){
        vph_res = 0x00;
        vpl_res = registers[i].value;         // Set the value of the REG over the VP Lowbite
        len_res--;                            // VP -> REG delta size (-1)
        return 0x81;
      }
    }
  }
  
  vpf_res = 0x02;                             // Set the VP flag
  return typ_res;                             // Keep the same command type
}

/*** REG-VP control ***/
void loadMappedVP(unsigned char reg){
  for(int i=0; i<10; i++){                    // Iterates over the register map struct array
    
    if(registers[i].value == reg){            // Find the require register struct
      
      if(typ_cmd == 0x80){                    // Check if is a Write command
        vph_cmd = registers[i].w_vp[0];
        vpl_cmd = registers[i].w_vp[1];
        
      } else {                                // Check if is a Read command
        vph_cmd = registers[i].r_vp[0];
        vpl_cmd = registers[i].r_vp[1];
      }
      
    }
  }
  len_cmd++;                                  //REG -> VP delta size (+1)
}
