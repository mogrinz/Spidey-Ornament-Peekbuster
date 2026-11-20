# Spidey Ornament Peekbuster

A speaker-triggered motor controller that raises and lowers a Spider-Man Christmas ornament figure while retaining the original ornament's motion-triggered sound.

## Project status

A prototype assembled by PCBWay is working with an **ATtiny402**, **DRV8838** motor driver, and the original ornament's speaker output as the trigger. The compact board is 22 x 20 mm and uses a single-cell LiPo with USB-C charging.

**Important R8 hardware correction:** The first assembled board used a 100 kΩ pull-up on the DRV8838 `nSLEEP` input and the motor driver remained asleep. Bridging R8 to SYS restored operation. All updated manufacturing files in this repository specify **R8 as a fitted 0 Ω 0603 jumper** (Yageo RC0603JR-070RL). Do not use the old 100 kΩ value.

## Files

- [Arduino firmware](firmware/Spidey_Ornament_Firmware/Spidey_Ornament_Firmware.ino) - speaker-triggered up/pause/down sequence for the ATtiny402
- [PCBWay manufacturing package](hardware/PCBWay/Spidey_Compact_R8_0R_PCBWay_Manufacturing_Package.zip) - complete archived handoff
- [Gerbers and drills](hardware/PCBWay/Spidey_Compact_R8_0R_Gerbers.zip) - upload as PCB fabrication ZIP
- [Assembly BOM](hardware/PCBWay/Spidey_Compact_R8_0R_BOM.csv) and [pick-and-place CPL](hardware/PCBWay/Spidey_Compact_R8_0R_CPL.csv) - upload as separate assembly files
- [Assembly notes](hardware/PCBWay/Spidey_Compact_R8_0R_Assembly_Notes.txt) - R8 correction and manufacturing instructions
- [KiCad source archive](hardware/KiCad/Spidey_Compact_R8_0R_KiCad_Source.zip) - complete editable KiCad project, local footprints, and design reports

## Firmware

The sketch targets **ATtiny402** with **megaTinyCore** and is uploaded through **UPDI** (e.g. Adafruit UPDI Friend). USB-C on the ornament PCB is **charging only**, not firmware programming.

| Function | MCU pin |
| --- | --- |
| Speaker sense (ADC) | PA1 |
| DRV8838 PH (direction) | PA2 |
| DRV8838 EN (PWM) | PA3 |

Default motor settings: PWM 165/255, 8 seconds up, 2-second pause, 8 seconds down. The firmware requires approximately one second of speaker silence after the cycle before rearming. These values worked on the author's prototype; adjust for your spool and motor before extended operation.

## Wiring and charging

- Power is from a protected **1S LiPo** via an external on/off switch.
- Keep both speaker wires attached to the original ornament's board; the new controller senses activity from a tap on **one speaker wire**. Share circuit ground.
- Motor wires connect to the two motor output pads; neither motor output is ground.
- Program using UPDI, target SYS reference/power, and ground. Check the programmer's power mode before connecting.
- Charge through USB-C with the ornament's power switch **OFF**; the charger is connected to the battery before the system power switch.

## Manufacturing cautions

All fitted components are on the top side. The PCBWay assembly team may need to panelize this 22 x 20 mm board. Avoid unapproved substitutions, and confirm **R8 = 0 Ω (FIT)**. The DRV8838 WSON exposed pad and USB-C mechanical anchors require correct assembly treatment.

These files describe a working prototype and are provided for educational/hobby use. Verify battery polarity, charger behavior, component sourcing, and motor travel on your own assembly.
