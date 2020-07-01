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
 *                      Core Stages functions                        *
 *********************************************************************/

/*** Stage 0 - HEADER ***/
void headerStage(unsigned char* b_read, unsigned char* b_last, bool resp){
 switch(*b_read){
  
  case 0x5A: {
    *b_last = *b_read;
    break;
  }
  
  case 0xA5: {
    if(*b_last == 0x5A){
      next(resp);
    }
    break;
  }
  
  default: {                            // Non-valid command/response
    if(resp) resetRes("HEADER");
    else resetCmd("HEADER");
    return;
  }
  
}
}

 /*** Stage 1 - SIZE ***/
void sizeStage(unsigned char* b_read, unsigned char* b_size, bool resp){
  
  *b_size = *b_read;                    // Set the remaining bytes of the current command/response
  
  if(resp) len_res = *b_read;           // Set the length of the current response
  else len_cmd = *b_read;               // Set the length of the current command
  
  next(resp);
}

 /*** Stage 2 - COMMAND ***/
void commandStage(unsigned char* b_read, unsigned char* b_size, bool resp){
  *b_size -= 1;                         // Decrement the remaining bytes of the current command/response

  if(resp){                             // Response scope
    
    typ_res = *b_read;                  // Sets the current response command (0x83)

    if(typ_cmd == 0x81 || typ_cmd == 0x83){
      next(resp);
    } else {
      resetRes("COMMAND");              //Non-valid response
      return;
    }
    
  } else {                              // Command scope
    
    typ_cmd = *b_read;                  // Sets the current command command (0x80, 0x81, 0x82, 0x83)
  
    if(typ_cmd == 0x80 || typ_cmd == 0x81 ||
        typ_cmd == 0x82 || typ_cmd == 0x83){
      next(resp);
    } else {
      resetCmd("COMMAND");              // Non-valid command
      return;
    }
    
  }
}

 /*** Stage 3 - REG-VP ***/
void regVpStage(unsigned char* b_read, unsigned char* b_size, bool resp){
  *b_size -= 1;                         // Decrement the remaining bytes of the current command/response

  if(resp){                             // Response scope
    
    if(vpf_res == 0x00){
      vpf_res = 0x01;                   // Set the VP flag
      vph_res = *b_read;                // Set the VP Highbyte of the response
      
    } else {
      vpl_res = *b_read;                // Set the VP Lowbyte of the response

      typ_res = getProculosCommand(vph_res, vpl_res);
      if(typ_res != 0x80 && typ_res != 0x81 &&
        typ_res != 0x82 && typ_res != 0x83){
          resetRes("REG-VP");           // Non-valid command
      }

      if(typ_res == 0x82 || typ_res == 0x83){
        vph_res -= 0x10;                // DGUS2 VP's range began in 0x1000
      }

      sendKnownBytes(resp);             // Send Header and Response Length 
      sendVPBytes(resp);                // Send the VP or REG
      
      next(resp);
    }
  } else {                              // Command scope
    
    if(typ_cmd == 0x80 || typ_cmd == 0x81){
      
      loadMappedVP(cb_read);            // Get the DGUS2 VP from the given Proculos REG
      sendKnownBytes(resp);             // Send Header and Command Length
      sendVPBytes(resp);                // Send the VP
      
      next(resp);
      
    } else if(typ_cmd == 0x82 || typ_cmd == 0x83){
      
      if(vpf_cmd == 0x00){
        vpf_cmd = 0x01;                  // Set the VP flag
        vph_cmd = *b_read;               // Set the VP Highbyte of the command
        vph_cmd += 0x10;                 // DGUS2 VP's range began in 0x1000
        
      } else {
        vpl_cmd = *b_read;               // Set the VP Lowbyte of the command
        sendKnownBytes(resp);            // Send Header and Command Length
        sendVPBytes(resp);               // Send the VP
        
        next(resp);
      }
    } else {
      resetCmd("REG-VP");                // Non-valid command
      return;
    }
  }
}

/*** Stage 4 - Write ***/
void writeStage(unsigned char* b_read, unsigned char* b_size, bool resp){
  *b_size -= 1;                          // Decrement the remaining bytes of the current command/response
      
  sendByte(*b_read, resp);               // Fowards the remaining bytes of the command/response
  
  if(*b_size == 0) {                     // End of command/response
    if(resp) resetRes("WRITE");
    else resetCmd("WRITE");
    
    return;
  }
}
