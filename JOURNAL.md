
Total time before submission: 61.5 hours


# December 1 - Planning Idea
**Total time spent: 5.5 hours**

Today i started drafting my idea on paper and researched what components i would use.

Features
- Possibly midi in?
- Maybe built in mic?
- Velocity support for keys
- Arpeggio module for effects
- 2-3 octave keyboard
- Portable, similar dimensions to a 75% keyboard

Components
- External psu for 4 stepper motors
- CD74HC4067 Analog mux i found from ti with 16:1 channels
- TMC2209 motor controller 
- DRV5053 analog hall sensors for hall effect key switches
- Esp32-s3 module

A major problem was finding a way to connect all 36 hall effect sensors to an esp32. Multiplexing isnt really my strong suit and i have some ptsd from doing it from a previous project.

![b](/user-attachments/blobs/proxy/eyJfcmFpbHMiOnsiZGF0YSI6MTcxMjcsInB1ciI6ImJsb2JfaWQifX0=--a948e05a87e2ae5387c1e9b4fbf7705b212b75a7/b.jpg)




# December 3 - Starting the Schematics
**Total time spent: 6 hours**

## MCU Setup

The microcontroller setup was pretty straightforward. 5v power was from usb-c and going through a LDO for 3v3 power for logic for the esp32.

![image](/user-attachments/blobs/proxy/eyJfcmFpbHMiOnsiZGF0YSI6MTgyNjYsInB1ciI6ImJsb2JfaWQifX0=--1e1b6f37bcca3c4c15896f685a32cbad8e38f318/image.png)

## TMC2209 Implementation


This was super hard as I had to design my own implementation of the IC for my specific use case. I had to reference many different resources including already made modules and the datasheet.

**References**
- [official datasheet](https://www.analog.com/media/en/technical-documentation/data-sheets/TMC2209_datasheet_rev1.09.pdf)
- [adafruit tmc2209 breakout board eagle files](https://github.com/adafruit/Adafruit-TMC2209-Breakout-PCB)
- [random schematic png](https://microcontrollerslab.com/wp-content/uploads/2021/03/TMC2209-Stepper-Motor-Driver-module-internal-circuit-diagram-768x377.jpg)
- [random schematic png 2](https://image.easyeda.com/components/e375a9ddb5db4d86a2d0d3d4b046859c.png)

![image](/user-attachments/blobs/proxy/eyJfcmFpbHMiOnsiZGF0YSI6MTgyNjcsInB1ciI6ImJsb2JfaWQifX0=--ae89b15afb878370eba336992995bcc9f20b1e4c/image.png)

I'm not entirely sure if my implementation is right or great... What got me really confused was how this IC could function without a MCU with all of its configuration pinouts, but I could really just do this all with the UART.




# December 4 - Creating Schematic for Multiplexer and Sensors
**Total time spent: 5 hours**

I think my implementation is alright. The sheer amount of sensors I am using definitely requires me to use a multiplexer as there isn't enough GPIO on the esp32-s3.

At first, I thought it would be really complicated to use the mux ic, but it was really simple and just acts like a switch between all the connected IO. The S0,1,2,3 accept binary to switch between all the connected devices and sends them to COM. I was told that my use case with tons of analog devices required an op-amp for stable voltage when switching.

I also used a bunch of resources below!

- [mux datasheet](https://www.ti.com/lit/ds/symlink/cd74hc4067.pdf?ts=1764837448384&ref_url=https%253A%252F%252Fwww.mouser.cn%252F)
- [op-amp datasheet](https://ww1.microchip.com/downloads/aemDocuments/documents/MSLD/ProductDocuments/DataSheets/MCP6001-1R-1U-2-4-1-MHz-Low-Power-Op-Amp-DS20001733L.pdf)
- [forum post on the mux](https://forum.arduino.cc/t/cd74hc4067-arduino/1116756)
- [sparkfun's mux breakout board schematic](https://cdn.sparkfun.com/datasheets/BreakoutBoards/Analog-Digital-Mux-Breakout-v11.pdf)

In the end, i think it'll function pretty solid
![image](/user-attachments/blobs/proxy/eyJfcmFpbHMiOnsiZGF0YSI6MTg4OTUsInB1ciI6ImJsb2JfaWQifX0=--d0c1b9787dc18bfd21e16a52ddb196ba81ba3676/image.png)
![image](/user-attachments/blobs/proxy/eyJfcmFpbHMiOnsiZGF0YSI6MTg4OTYsInB1ciI6ImJsb2JfaWQifX0=--ccf79970799c2b0239fb014a32adec21d59e70d2/image.png)




# December 6 - Finishing up schematic, defining footprints for symbols
**Total time spent: 7 hours**

## Connecting Components to MCU

I decided to skip connecting all my components to my mcu until the end as i knew it would be time consuming to check which pins were capable of what... and i was right.

A bunch of checking if GPIO or ADC was supported for specific pins. There were so many limitations that I sadly had to remove one rotary encoder in my design as well as two buttons that are linked to two encoders. 

I double, even triple checked if the io i used was safe and there was so much conflicting info due to there being multiple revisions of the esp32-s3. In the end, IO6-12 are actually safe to use and kicad's symbol is correct as the SPI flash io is between IO21-30 which is why they're omitted. I THINK.

![image](/user-attachments/blobs/proxy/eyJfcmFpbHMiOnsiZGF0YSI6MjAzMTksInB1ciI6ImJsb2JfaWQifX0=--327924ec7975840479412be45670e0de2dbafc25/image.png)

Anyways, In the end i was able to use up all of my GPIO lol.

![image](/user-attachments/blobs/proxy/eyJfcmFpbHMiOnsiZGF0YSI6MjAzMjUsInB1ciI6ImJsb2JfaWQifX0=--a5f3a990efcc51e24bb0fbc7258f54655ed104bc/image.png)

IO47 + 48 are also not on 3.3v logic high for some reason...

## Footprints

I decided to use mostly all 0805 size components for resistors and capacitors as they should be common and standard component for PCBA. For my external PSU, i am using a 5mm pitch screw terminal. I also will use a hot-swappable key switch thing for my two octave change keys

![image](/user-attachments/blobs/proxy/eyJfcmFpbHMiOnsiZGF0YSI6MjAzMjYsInB1ciI6ImJsb2JfaWQifX0=--948d15911e5a5e272ccf3d23bf6a696412f83405/image.png)

![image](/user-attachments/blobs/proxy/eyJfcmFpbHMiOnsiZGF0YSI6MjAzMjcsInB1ciI6ImJsb2JfaWQifX0=--4353a658c3b5a6f9929319df79009ad14cb4eb57/image.png)






# December 7 - Component Positioning
**Total time spent: 7 hours**

I positioned everything on my board. It took quite a while as I had to search back through some datasheets as they recommend a distance between some components, like pulldowns and capacitors for the hall effect sensors.

I generated a outline file for my keyboard switches and based the board size off of it. This would let me know every position of the keys n stuff.

One thing that I had to also account for was the space underneath the pcb. The power supply i was using was pretty large, and it limited places of where I could put components that stuck out (screw terminals, heatsinks for tmc2209 drivers, pinheaders, etc)

![image](/user-attachments/blobs/proxy/eyJfcmFpbHMiOnsiZGF0YSI6MjEwMTUsInB1ciI6ImJsb2JfaWQifX0=--01db2d0d93ea8dccf7b35c8ba0a040a0b3968a35/image.png)





# December 8 - DC-DC Buck Converter + Auto Power Switch
**Total time spent: 3 hours**

Since my design has two power sources, one from a power supply and another from USB power delivery, i would have to manage which power I am going to use.

Ideally I should support all these conditions below

- USB plugged in, PSU not plugged in (esp32/logic is powered, motors cant drive, ideal for developing firmware)
- USB unplugged, PSU plugged in (both esp32 and stepper motor ics are powered and can drive, for production)
- Both USB and PSU plugged in (same as above but can also push code and run motors at the same time)

First, I need to turn 24v to 5v. I can do this with a simple DC-DC Buck Converter module i found.

Now, I can then use a TPS2115A to switch between two 5v sources and then output to the LDO instead of USB PD doing it directly.

I used a bunch of sources including the ones below to help wire the IC!

- [ti forum post 1](https://e2e.ti.com/support/power-management-group/power-management/f/power-management-forum/962334/tps2115-backdriving)
- [ti forum post 2](https://e2e.ti.com/support/power-management-group/power-management/f/power-management-forum/969018/tps2115a-using-in1-as-input-for-d1)
- [official datasheet](https://www.ti.com/product/TPS2115A)

![image](/user-attachments/blobs/proxy/eyJfcmFpbHMiOnsiZGF0YSI6MjE0NzMsInB1ciI6ImJsb2JfaWQifX0=--d73b70624e355f291bfa28beb469c6a55a8b6b70/image.png)


# December 10 - Cadded a Bunch of Stuff
**Total time spent: 18 hours**

This entry is a combination of work spread around 4ish days or more. I chose to do this on onshape as i am very annoyed at how slow fusion 360 loads. 

## Measuring Parts On Hand

I needed to measure parts i had on hand and model some references. This included my ac plug, stepper motors, and power supply.

![image](/user-attachments/blobs/proxy/eyJfcmFpbHMiOnsiZGF0YSI6MjIzNTcsInB1ciI6ImJsb2JfaWQifX0=--defea23f87e7516bc44bb14591e88044fdf3de64/image.png)

## Importing and Designing around the PCB Model

Kicad has this feature where i can export a 3d model (step file) and import into onshape to take solid dimensions of. At first i had issues with lag but i remembered that all the caps/resistors were also included and fixed that.

![image](/user-attachments/blobs/proxy/eyJfcmFpbHMiOnsiZGF0YSI6MjIzNjAsInB1ciI6ImJsb2JfaWQifX0=--1d0736a85632933168ed229987cef1ec8428f623/image.png)

With this, I started designing the plate that fits the keycaps, neopixels (shining through a thin layer of plastic), and rotary encoders. I could then build a case around it fastened with M3 screws i have laying around.

![chrome_lmEsddRs0H](/user-attachments/blobs/proxy/eyJfcmFpbHMiOnsiZGF0YSI6MjIzNjEsInB1ciI6ImJsb2JfaWQifX0=--9dda4cf184178c12ce93703a825f0688c97fa719/chrome_lmEsddRs0H.png)

![image](/user-attachments/blobs/proxy/eyJfcmFpbHMiOnsiZGF0YSI6MjIzNjIsInB1ciI6ImJsb2JfaWQifX0=--1a43d04dd48b271ba36a7e4aa79a865da1bb686a/image.png)

The PCB would be held in through the keycaps in the plate, but is also supported by a inner lip inside the case

A press fit mount was made for the power supply as well

![image](/user-attachments/blobs/proxy/eyJfcmFpbHMiOnsiZGF0YSI6MjIzNjMsInB1ciI6ImJsb2JfaWQifX0=--a8e83f4515f96a4032c43b57d9fffaa49c498e9a/image.png)

The power supply is also held with this bar that hugs the power supply and stops the plate from sagging

![image](/user-attachments/blobs/proxy/eyJfcmFpbHMiOnsiZGF0YSI6MjIzNjQsInB1ciI6ImJsb2JfaWQifX0=--96895b50ef0b8108b7d25f692f6847b20a5dbd7f/image.png)

Lastly, a motor mount was made to stylishly display the stepper motors at a 45 degree angle to the user. It is also press fit lol

![image](/user-attachments/blobs/proxy/eyJfcmFpbHMiOnsiZGF0YSI6MjIzNjUsInB1ciI6ImJsb2JfaWQifX0=--4dcdef82ba3b043c4926ea8b0dc342ad1cf73c0b/image.png)


## End result

Overall, i think its a really solid cad design but kinda bland, I'll probably add some holes to allow the user to see inside of it later.

![chrome_8n2NrPDpo4](/user-attachments/blobs/proxy/eyJfcmFpbHMiOnsiZGF0YSI6MjIzNjYsInB1ciI6ImJsb2JfaWQifX0=--391f396dc09cc2aca7b530a1c4142d165950db3f/chrome_8n2NrPDpo4.png)

![image](/user-attachments/blobs/proxy/eyJfcmFpbHMiOnsiZGF0YSI6MjIzNjcsInB1ciI6ImJsb2JfaWQifX0=--dcbddec47c4fd41152110558d03009d6ff4b3650/image.png)







# December 13 - Routed PCB
**Total time spent: 6 hours**

I started routing my PCB but saw a few mistakes on my schematic while doing so. The pinout for TMC2209 to my specific stepper motor was a little different with the coil pairs being together instead of separated. Some previous placement of footprints also didnt make sense and i definitely could of chose specific pins based on their physical location. (analog muxes) 


I tried ignoring this problem until now, but I've never routed any power lines above 5v before. Looking at TMC2209 modules sold online, they of course have thick traces coming from OA1/OA2/OB1/OB2. I wasn't sure how thick mine were going to be, but definitely not the 0.3mm default. 

I first did 1.0mm thickness which took tons of space, but after some chatting and doing some research, 0.6mm seemed like it would work.

![image](/user-attachments/blobs/proxy/eyJfcmFpbHMiOnsiZGF0YSI6MjM5OTEsInB1ciI6ImJsb2JfaWQifX0=--22ac618ba8fc3433b51d69bb846681624d24ea35/image.png)

My workflow was something like this:

- Route the hall effect sensors and mux first
- Give power traces priority, and also traces farthest from the esp32 pin
- Also give analog traces more priority over digital ones
- Try not to use too many vias to jump across traces
- its literally a game of chess, but kicad (iykyk)

Overall, my routing was good enough

For final touches, I tried something new called via stitching

![image](/user-attachments/blobs/proxy/eyJfcmFpbHMiOnsiZGF0YSI6MjM5ODksInB1ciI6ImJsb2JfaWQifX0=--b5f41055694911b916054e3253d92cbdfc842fec/image.png)







# December 14 - Final Touches
**Total time spent: 4 hours**

I pretty have everything done! But i need to fix some small issues and also have some flare


## Cad

The cad was improved a little. I added some cool extruded geometry on the side seen by users, and filleted corners in the x/y axis for 3d print optimization.

![image](/user-attachments/blobs/proxy/eyJfcmFpbHMiOnsiZGF0YSI6MjQ0NzEsInB1ciI6ImJsb2JfaWQifX0=--bafd0d36cee3c946ee94bce58a5ad69d25884596/image.png)


![image](/user-attachments/blobs/proxy/eyJfcmFpbHMiOnsiZGF0YSI6MjQ0NzAsInB1ciI6ImJsb2JfaWQifX0=--b80f1cc8f4a4d551aa9064270949ec3486ed677f/image.png)

## PCB

I made sure to use as many basic parts as i could and only am doing PCBA on one side. I will have to solder on some components, including the MCU, neopixels, bulk caps, and more. 

I also created some cool silkscreens for the front and back of my pcb since there was empty space

![image](/user-attachments/blobs/proxy/eyJfcmFpbHMiOnsiZGF0YSI6MjQ0NjksInB1ciI6ImJsb2JfaWQifX0=--9e6434aad66594089ae207a3d255d9046f71c2c6/image.png)

## BOM

I started writing my bom

![image](/user-attachments/blobs/proxy/eyJfcmFpbHMiOnsiZGF0YSI6MjQ0NjgsInB1ciI6ImJsb2JfaWQifX0=--ebbd39522198ee2e26bf69bff5dbbe18558b5919/image.png)
