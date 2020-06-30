# DGUS2 Middleware
This is an easy-to-read firwmare for ESP32/Arduino that aims to convert the Proculos commands into the DGUS2 commands and also the DGUS2 responses into Proculos responses. It makes possible Proculos based devices/equipments to communicate with DWIN DGUS2 LCMs.

It's STILL IN DEVELOPMENT.

## How to use
- Download the source files and move them to a folder named 'MainMiddle' (Arduino projects require the project folder having the same name of the main file);
- Open the 'MainMiddle.ino' file with the Arduino IDE;
- Load the code into an ESP32/Arduino board (It may work on other boards - NOT TESTED);
- Connect the TX/RX from a Proculos/DGUS equipment on the TX1/RX1 pins of the board;
- Also, connect the TX2/RX2 pins in the TX/RX of the DGUS2 LCM;
- Turn the system on. It should works.

## Project organization
- _MainMiddle.ino_

    Variables declarations.
- _CoreControl.ino_

    Core auxiliare functions for conversions, adjustments and state machine control.
- _CoreMiddle.ino_


    Core conversion itself.
- _CoreStage.ino_

    Core functions of each stage of the conversion.
- _TestMiddle.ino_

    Test function for DEBUG porposes.
    
 ### AGTechnologies and FGApps © 2020