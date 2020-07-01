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
 *                      Variable declaration                         *
 *********************************************************************/

/*** Command variables ***/
unsigned char cb_read = 0x00;  // Command byte read
unsigned char cb_last = 0x00;  // Last command byte read (Header Only)
unsigned char cb_size = 0x00;  // Proculus command byte size

unsigned char typ_cmd = 0x00;  // Command Type (0x80 || 0x81 || 0x82 || 0x83)
unsigned char vph_cmd = 0x00;  // Mapped VP highbyte
unsigned char vpl_cmd = 0x00;  // Mapped VP lowbyte
unsigned char vpf_cmd = 0x00;  // Flag for VP
unsigned char len_cmd = 0x00;  // DWIN2 command size (length)

unsigned char stage_c = 0x00;  // Translate Stage ( Header || Size || Command || VP || Write )

/*** Response variables ***/
unsigned char rb_read = 0x00;  // Response byte read
unsigned char rb_last = 0x00;  // Last response byte read (Header Only)
unsigned char rb_size = 0x00;  // Proculus response byte size

unsigned char typ_res = 0x00;  // Command Type (0x80 || 0x81 || 0x82 || 0x83)
unsigned char vph_res = 0x00;  // Mapped VP highbyte
unsigned char vpl_res = 0x00;  // Mapped VP lowbyte
unsigned char vpf_res = 0x00;  // Flag for VP
unsigned char len_res = 0x00;  // DWIN2 command size (length)

unsigned char stage_r = 0x00;  // Translate Stage ( Header || Size || Command || VP || Write )

/*** Register Map Array ***/
struct Register {
  unsigned char w_vp[2];
  unsigned char r_vp[2];
  unsigned char value;
};

Register registers[10] = {     // Registers <--> VP Mappings
  // Version //
  {
    { 0x00, 0x0F },
    { 0x00, 0x00 },
    0x00
  },
  // Backlight //
  {
    { 0x00, 0x82 },
    { 0x00, 0x31 },
    0x01
  },
  // Buzzer //
  {
    { 0x00, 0x9B },
    { 0x00, 0x00 },
    0x02
  },
  // Page //
  {
    { 0x00, 0x84 },
    { 0x00, 0x14 },
    0x03
  },
  // Touch Press Flag //
  {
    { 0x00, 0x16 },
    { 0x00, 0x00 },
    0x05
  },
  // Touch Press Status //
  {
    { 0x00, 0x00 },
    { 0x00, 0x16 },
    0x06
  },
  // Touch Press Coordinates //
  {
    { 0xaa, 0xbb },
    { 0x00, 0x16 },
    0x07
  },
  // Backlight Readyonly //
  {
    { 0x00, 0x00 },
    { 0x00, 0x31 },
    0x1E
  },
  // RTC value //
  {
    { 0x00, 0x10 },
    { 0x00, 0x10 },
    0x20
  },
  // Reset //
  {
    { 0x00, 0x04 },
    { 0x00, 0x00 },
    0xEE
  }
};
