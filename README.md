# Lab bench power supply

<img src="https://user-images.githubusercontent.com/83133831/195663146-906130e0-5699-4c6b-9b84-9f478b7fe83d.jpg" width="300" height="202"> <img src="https://user-images.githubusercontent.com/83133831/195663539-82f5cf60-b217-4e23-acb1-4ad2eb4b54f2.jpg" width="300" height="202"> <img src="https://user-images.githubusercontent.com/83133831/195664249-84418ba6-b0a5-4662-82da-99e4d841ae3b.jpg" width="269" height="202">




## Description
Linear lab bench power supply. This is a 15W constant voltage constant current power supply. It takes an 18VDC input and regulates it to 1.25VDC-15VDC and 0A-1A. The voltage is regulated by a LT1086 linear regulator. The output voltage is controlled by a voltage fed to the ADJ-pin by the MCU. The output voltage is fed back to a LM2576 switching regulator. This switching regulator ensures the voltage drop of the LT1086 never goes above ~2VDC. This way, the power loss for the LT1086 stays small, even when regulating the output to a low voltage. <br />
The current regulation is done by the MCU. The voltage over the shunt resistor gets amplified and read by the MCU. This value is then compared to the current-limit value input by the user. If the measured value >= limit value, the ADJ-pin of the LT1086 gets pulled to GND through a transistor. This reduces the voltage on the output, therefore reducing the current. When the current has dropped below the limit value, the ADJ-pin of the LT1086 is once again controlled normally. This loop goes on, therefore limiting the current.

### PCB
PCB designed in KiCad and orderer from JLCPCB. The dimensions of the PCB were chosen based on the enclosure which was ordered from Amazon. Design files are found in the project repository.

### BOM
BOM is found in the [design files](https://github.com/aMarkusa/Lab-bench-power-supply-2022/tree/main/Design%20files) directory. 

### Reflections
This was the biggest project for me so far. The most challenging parts were designing the PCB and the firmware. I got some help with the PCB design from a more experienced colleague, which enabled me to improve the design in ways I didn't think of. Even though I put a lot of effort into the PCB, there were some traces routed incorrectly, but they were corrected with some wiring. <br />
Even though the firmware was quite straightforward, I had some trouble getting the encoders to work properly. The MCU didn't react to any input from the encoders. I found out this mainly had to do with the refresh time of the OLED display. As the display is using I2C, it takes a relatively long to refresh it. While the display is refreshing, the MCU can't handle any more data, therefore leaving out any input from the encoders. I improved this by implementing some interrupts for the encoders, but the performance is still far from perfect. <br />
I also think the way I'm implementing the current limiting affects the performance. If I would use a hardware implementation, for example, a comparator, instead of software, I could free up some resources for the MCU. This way the MCU can focus on reading the encoders and displaying data without also having to limit the current. <br />
The voltage does not reach 15V on high currents. This is because I used a relatively large-valued shunt of 1ohm. The voltage drop across the shunt lowers the voltage being fed to the regulator. By using a smaller shunt, this problem could be eliminated.
Even though some things could be improved, the PSU is working and is used anytime I need power. Therefore, this project was both educational and useful in practice.
