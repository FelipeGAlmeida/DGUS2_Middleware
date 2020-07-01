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
 *                            DEBUG/Test                             *
 *********************************************************************/

unsigned char convertSerialInput(unsigned char b){
  // COMMAND STRUCTURE:
  // [5A][A5][SIZE][TYPE]  [REG]   [VALUE(S)||LENGTH]
  // [5A][A5][SIZE][TYPE][VPH][VPL][VALUE(S)||LENGTH]
  
  // [5A][A5] - Enter the header:
  if(b == 'a') return 0x5A; // H1 *
  if(b == 's') return 0xA5; // H2 *

  // [SIZE] - Choose the command size:
  if(b == 'd') return 0x03; // Size 1 - 3 bytes
  if(b == 'c') return 0x04; // Size 2 - 4 bytes
  if(b == 'x') return 0x05; // Size 3 - 5 bytes
  if(b == 'z') return 0x06; // Size 4 - 6 bytes
  if(b == 'q') return 0x0C; // Size 5 - 12 bytes

  // [TYPE] - Choose the command type:
  if(b == 'f') return 0x80; // Cmd 1 - Write REG
  if(b == 'v') return 0x82; // Cmd 2 - Write VP
  if(b == 'b') return 0x81; // Cmd 3 - Read REG
  if(b == 'n') return 0x83; // Cmd 4 - Read VP

  // [REG]||[VPH][VPL] - Choose the REG or VP:
  if(b == 'g') return 0x01; // Register -> W 0x82 || R 0x31

  // [VALUE(S)||LENGTH] - Choose the Value(s) or Length: 
  if(b == 'h') return 0x02; // Value 1
  if(b == 'e') return 0x64; // Value 2
  if(b == 'r') return 0x78; // Value 3
  if(b == 't') return 0x45; // Value 4
  if(b == 'y') return 0x99; // Value 5
  if(b == 'u') return 0x31; // Value 6
  return 0x00; // Any other key (I, o, p; J, k, l, ç; M ..)
}
