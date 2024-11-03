# AIOC minus cable
This is the Ham Radio *All-in-one-Cable* without the cable!. **Its a fork of a project currently still being tested! So infer what you want as to how much this has been tested** Please read this README as it'll be the main place to see the differences between the amazing [AIOC](https://github.com/skuep/AIOC) which you should still go support and purchase.


## What does it do?
The AIOC is a small adapter with a USB-C connector that enumerates itself as a sound-card (e.g. for APRS purposes), a virtual tty ("COM Port") for programming and asserting the PTT (Push-To-Talk) as well as a CM108 compatible HID endpoint for CM108-style PTT (new in firmware version 1.2.0).

But! Minus the cable and uses BT instead

## New features not in AIOC ##
- Well, obviously BT support
- ?

**And then you still get all the wonderful ones from AIOC!**
## Features ##
- Cheap & Hackable Digital mode USB interface (similar to digirig, mobilinkd, etc...)
- Programming Cable Function via virtual Serial Port
- Compact form-factor (DIY overmolded enclosure is currently TBD)
- Based on easy-to-hack **STM32F302** with internal ADC/DAC (Programmable via USB bootloader using [DFU](#how-to-program))
- Can support Dual-PTT HTs
- Supports all popular OSes (Linux, Windows and MacOS with limitations [[*]](https://github.com/skuep/AIOC/issues/13))


## Compatibility
### Software
  - TBD

### Tested Radios (so far)
  - TBD

![Top side of PCB](doc/images/k1-aioc-photo.jpg?raw=true "Top side of PCB")

## How To Fab - NOT YET UPDATED FOR AIOC-one, so your just making an aioc at that point, which is good, just FYI ##
- Go to JLCPCB.com and upload the GERBER-k1-aioc.zip package (under ``kicad/k1-aioc/jlcpcb``)
  - Select PCB Thickness 1.2mm (that is what I recommend with the TRS connectors I used)
  - You may want to select LeadFree HASL
  - Select Silkscreen/Soldermask color to your liking
- Check "PCB Assembly"
  - PCBA Type "Economic"
  - Assembly Side "Top Side"
  - Tooling Holes "Added by Customer"
  - Press Confirm
  - Click "Add BOM File" and upload ``BOM-k1-aioc.csv``
  - Click "Add CPL File" and upload ``CPL-k1-aioc.csv``
  - Press Next
  - Look Through components, see if something is missing or problematic and press Next
  - Check everything looks roughly good (rotations are already baked-in and should be correct). Save to Cart

This gives you 5 (or more) SMD assembled AIOC. The only thing left to do is soldering on the TRS connectors (see [here](#how-to-assemble)).
The total bill should be around 60$ US for 5 pieces plus tax and shipping from China.

__Note__ that the following message from JLCPCB is okay and can be ignored.
````
The below parts won't be assembled due to data missing.
H1,H2 designators don't exist in the BOM file.
J2,D3,D4,R17 designators don't exist in the CPL file.
````

__Note__ for people doing their own PCB production: I suggest using the LCSC part numbers in the BOM file as a guide on what to buy (especially regarding the MCU).

__Note__ the current hardware version is **1.0**, but there is a pre-release **1.2** in the making [here](https://github.com/skuep/AIOC/pull/93) that features
- support for an external hardware input (e.g. for COS)
- a split pad for the PTT/UART contact, so in case you would like to use the AIOC hardware for another purpose than a K1-style connector, you can separate these two signals that are otherwise connected to the same TRS contact.
- Adjustable input and output audio levels (line-level, mic-level)
  
## How To Assemble
This is the process I use for building. See photographs in ``images`` folder.
- You need to use **Monacor** ``PG-204P`` and ``PG-203P`` or compatible TRS connectors (2 solder lugs and a big tab for the sleeve connection). **Adafruit** products ``1800`` and ``1798`` are confirmed to work as well.
- Cut the 2.5mm and 3.5mm TRS sleeve tab where the hole is located
- Put both TRS connectors into the 3d-printed solder guide (or a cheap HT that you don't mind potentially damaging). Make sure, that they are seated all the way in. If the holes in the solder guide are too small, you can ream them using a 2.5mm and 3.5mm drill bit.
- Insert the AIOC PCB into the solder guide
- Solder sleeve tab on the back side for both TRS connectors first
- Turn around PCB and solder remaining solder lugs
- Optionally you can 3D print a case for your AIOC. [This model](https://www.thingiverse.com/thing:6144997) has been designed by a third party but is confirmed to work with the AIOC.

__Note__ It is unfortunately quite common that the TRS connectors have intermittent contact after soldering the ring or tip tabs. In this case, it helps to re-melt the solder joint, then using e.g. tweezer slightly assert some sideways force onto the contact and let the solder joint solidify **while applying the pressure on the contact**. This will keep some tension on the area where the tab internally connects to the barrel and thus give a solid (spring loaded) connection.

## How To Build
For building the firmware, clone the repository and initialize the submodules. Create an empty workspace with the STM32CubeIDE and import the project.
  - ``git clone <repositry url>``
  - ``git submodule update --init``
  - Start STM32CubeIDE and create a new workspace under ``<project-root>/stm32``
  - Choose File->Import and import the ``aioc-fw`` project in the same folder without copying
  - Select Project->Build All and the project should build. Use the Release build unless you specifically want to debug an issue

## How To Program
### Initial programming
The following steps are required for initial programming of the AIOC:
- Short outermost pins on the programming header. This will set the device into bootloader mode in the next step.
![Shorting pins for bootloader mode](doc/images/k1-aioc-dfu.jpg?raw=true "Shorting pins for bootloader mode")
- Connect USB-C cable to the AIOC PCB
- Use a tool like ``dfu-util`` to program the firmware binary from the GitHub Releases page like this:
  ````
  dfu-util -a 0 -s 0x08000000 -D aioc-fw-x-y-z.bin
  ````
  __Note__ that a ``libusb`` driver is required for this. On Windows there are additional steps required as shown [here](https://yeswolf.github.io/dfu) (*DFuSe Utility and dfu-util*). On other operating systems (e.g. Linux, MacOS), this just works ™ (provided libusb is installed on your system).
  On Linux (and MacOS), your user either needs to have the permission to use libusb (``plugdev`` group) or you might need to use ``sudo``.
- Remove short from first step, unplug and replug the device, it should now enumerate as the AIOC device

### Firmware updating
Once the AIOC has firmware loaded onto it, it can be re-programmed without the above BOOT sequence by following these steps.

__Note__ This requires firmware version >= 1.2.0. For older firmwares, the initial programming sequence above is required for updating the firmware.
- Run ``dfu-util`` like this
  ````
  dfu-util -d 1209:7388 -a 0 -s 0x08000000:leave -D aioc-fw-x-y-z.bin
  ````

This will reboot the AIOC into the bootloader automatically and perform the programming. 
After that, it automatically reboots the AIOC into the newly programmed firmware.

__Note__ Should you find yourself with a bricked AIOC, use the initial programming sequence above

## How To Use

# Known issues

# Future work
