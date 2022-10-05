# Lab bench power supply

## Description
Linear lab bench power supply. This is a 15W constant voltage constant current power supply. It takes a 18VDC input and regulates it to 1.25VDC-15VDC and 0A-1A. The voltage is regulated by a LT1086 linear regulator. The ouput voltage is controlled by a voltage fed to the ADJ-pin by the MCU. The output voltage is fed back to a LM2576 switching regulator. This switching regulator ensures the voltage on the input pin of the LT1086 never goes above ~2VDC. This way, the power loss for the LT1086 stays small, even when regulating the the output to a low voltage.  <br />
The current regulation is done by the MCU. The voltage over the shunt-resistor gets amplified and read by the MCU. This value is then compared to the current-limit value input by the user. If the measured value >= limit value, the ADJ-pin of the LT1086 gets pulled to GND through a transistor. This reduces the voltage on the output, therefore reducing the current. When the current has dropped below the limit value, the ADJ-pin of the LT1086 is once again controlled normally. This loop goes on, therefore limiting the current.

### PCB
PCB designed in KiCad and orderer from JLCPCB. The dimensions of the PCB were chosen based on the enclosure which was ordered from Amazon.

###BOM
BOM found in design file repository.
