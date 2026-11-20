# Spider-Man Peekbuster Ornament: Now He Actually Climbs!

Hallmark's 2025 **Spider-Man Peekbuster** ornament is a clever idea. Wave your hand in front of it and Spider-Man hangs upside down while the ornament plays one of its sound clips.

There was just one thing that bothered me: **Spider-Man didn't actually climb up and down.** He looked like he should move, so I decided to fix that.

I wanted to keep the original ornament's sensor and sounds, not replace everything. My goal was to add a small motor and spool so Spider-Man could climb when the ornament spoke, then lower himself back down. It also needed to fit inside a compact, 3D-printed enclosure rather than turning into a pile of exposed electronics.

## How it works

The original Hallmark ornament still senses a hand waving in front of it and plays its normal audio. A small add-on controller detects activity on the original speaker line and runs a geared DC motor that winds or unwinds the line holding Spider-Man.

1. Wave a hand in front of the ornament.
2. The original Hallmark board plays a sound clip.
3. The add-on PCB detects the speaker activity and raises Spider-Man.
4. After a pause, the motor reverses and lowers him.
5. The controller waits for the sound to stop before accepting another trigger.

The first version used a small Arduino board, an external motor driver, and a hand-built motor/spool mechanism. That proved the idea, but there was more hardware than I needed, and it took up too much room. For the next version, I used AI tools to help redesign the electronics as a **22 × 20 mm custom PCB** with an ATtiny402, a dedicated motor driver, and USB-C LiPo charging. PCBWay manufactured and assembled the prototype boards. The first board also needed a real-world fix to the motor driver's sleep circuit, described below. This is a project where a working physical prototype mattered just as much as the PCB design.

## Order the custom PCB

**[Hallmark 2025 Spider-Man Peekbuster Add-On PCB on PCBWay](https://www.pcbway.com/project/shareproject/Hallmark_2025_Spider_Man_Peekbuster_add_on_PCB_c5621c59.html)**

You can order the custom PCB through that project page. The files used for the corrected board are also in this repository:

- [PCBWay manufacturing package](hardware/PCBWay/Spidey_Compact_R8_0R_PCBWay_Manufacturing_Package.zip)
- [Gerber and drill files](hardware/PCBWay/Spidey_Compact_R8_0R_Gerbers.zip)
- [BOM](hardware/PCBWay/Spidey_Compact_R8_0R_BOM.csv) and [component placement (CPL)](hardware/PCBWay/Spidey_Compact_R8_0R_CPL.csv)
- [Assembly notes](hardware/PCBWay/Spidey_Compact_R8_0R_Assembly_Notes.txt) and [R8 engineering change](hardware/PCBWay/Spidey_Compact_R8_0R_ECO_and_Upload_Instructions.txt)
- [KiCad source archive](hardware/KiCad/Spidey_Compact_R8_0R_KiCad_Source.zip)

**Important if you're having a board assembled:** The working prototype revealed a mistake in the original motor-driver sleep pull-up. **R8 must be a fitted 0 Ω 0603 jumper, not the original 100 kΩ resistor.** The manufacturing package in this repository contains the correction. Check the assembly BOM before ordering, especially if using a previously saved PCBWay project or quote. Ordering a bare PCB does not automatically include the components.

## 3D-printed parts

The STL files for the enclosure and motor/spool assembly are in [`3d-printing/`](3d-printing/):

- [Ornament Body.stl](3d-printing/Ornament%20Body.stl)
- [Sides.stl](3d-printing/Sides.stl)
- [Lid.stl](3d-printing/Lid.stl)
- [Stack.stl](3d-printing/Stack.stl)
- [OrnamentSpool.stl](3d-printing/OrnamentSpool.stl)
- [Spool Cover.stl](3d-printing/Spool%20Cover.stl)

These are the original model files for my build. Check fit and clearance with your motor, PCB, battery, and ornament hardware before final assembly.

## Firmware

The Arduino sketch is here: **[Spidey_Ornament_Firmware.ino](firmware/Spidey_Ornament_Firmware/Spidey_Ornament_Firmware.ino)**.

It runs on the **ATtiny402** using **megaTinyCore**, programmed through the board's UPDI pads. I used an Adafruit UPDI Friend. The USB-C connector on the PCB is for **battery charging only**, not programming.

| Controller function | ATtiny402 pin |
| --- | --- |
| Speaker activity sensing | PA1 |
| Motor direction (DRV8838 PH) | PA2 |
| Motor speed/enable (DRV8838 EN) | PA3 |

The current sketch uses PWM **165/255**, with **8 seconds up**, **2 seconds paused**, **8 seconds down**, followed by a one-second silence requirement before rearming. Those timings suit my prototype. Adjust them if your motor, spool, or line length differs.

## Wiring and power notes

- Keep both original speaker wires connected to the Hallmark board. Tap **one** speaker wire for the add-on board's audio-sense input, and connect the original circuit ground to the add-on ground. Neither motor output should be tied to ground.
- Connect the motor to the two motor output pads and use a protected single-cell LiPo battery with the correct polarity.
- The external switch controls the ornament electronics. The USB-C charging circuit connects to the battery ahead of the switch, so **turn the ornament off while charging**.
- Check your UPDI programmer's voltage and target-power configuration before connecting it to a battery-powered board.
- Verify motor direction and travel with the mechanism unloaded before running it unattended.

## YouTube build video

I'm finishing a video showing the original ornament, the prototype, the custom PCB, and the completed mechanism. I'll add the link here once the video is published.

---

This is an independent hobby modification of a Hallmark ornament, not an official Hallmark product.
