# PanTilt

PanTilt is an application designed for the SEI Hardware-in-the-loop (HIL) Lab. PanTilt drives motion of a small 2-axis mount and is meant to server as the system under test (SUT) for the test hardware and software in the lab.

PanTilt is a C++ application that runs on the BeagleBone Black (BBB), a small Debian Linux-based board based upon the TI ARM processor AM335x. The BBB comes with numerous on-board peripherals and be used for self-hosted development or cross-compiler development.

Considerable credit is owed to Derek Molloy, author of the book "Exploring BeagleBone", for many of the techniques - as well as some of his C++ device classes - that were utilized for this project. His explanations saved many hours of potentially lost time. 

## Development Environment

PanTilt was developed using a Debian 8 (Jessie) VM on Oracle VBox running on a Dell Optiplex 7010 Windows7 (64-bit) desktop machine. The connection to the BBB was via the satndard BBB USB cable to the host machine. The BBB drivers set up a private network using this USB connection wherein the host is 192.168.7.1 (acting as  default gateway for the BBB) and the BBB is at 192.168.7.2. 

For this work the BBB was set up with root (no password) and jaym (password jaym) accounts. Most of the development work was done utilizing only the jaym account.

The Debian VM utilized Eclipse (version Luna 4.4.2). The Eclipse project was set up to use the ARM 32-bit GNU cross-compiler tools in order to develop for the BBB. The gdb debugger was set up also for (server on the BBB) debugging the remote application on the BBB, though this capability was used fairly sparingly.

All code was version managed using git on the Debian VM command line, with the remote repo hosted on github.com for user jay-marchetti.

## Design

PanTilt uses a 2-axis mount designed to hold an iPhone. The original goal was to use a custom iPhone application sensing the phone attitude (roll, pitch, and yaw angles) and communicate this information via WiFi back to the PanTilt controller application running on the BBB. Alas, we lost our CMU student intern and so dialed back the scope a bit to just be an open loop 2-axis motion controller. 

The mount uses 2 model aircraft servo motors: HiTec HS-485HB. Each of these motors is driven from the BBB with a 50Hz PWM command signal where the angle is specified via the high-time portion of the PWM signal in the range [400, 1200] microseconds. The PWM signal is driven through a 1K ohm resistor for current limiting. The 5V and GND signals, along with both PWM outputs come directly from the BBB's P9 header (connector). Specifically, Axis 1 (inner axis, or "Tilt") is driven by P9.16 and Axis 2 (outer axis, or "Pan") is driven by P9.22. Each of these PWM outputs must be selected using the BBB pin-muxing (device tree overlay, or DTO) one time after the BBB is first powered up. This is described below under "Booting Up".

The PanTilt software design is based on the custom Axis (C++) class, of which there are 2 instances. These objects are accessed withing the code via an object array as axis[1] and axis[2]. Each Axis object contains all of the state variables and methods to opearate that particular axis. Each Axis object also contains its own 50Hz (POSIX pthread_t) thread for driving precisely timed motions using 1 of 3 possible modes, as described below under "Usage". The Axis objects each utilize the PWM object (Molloy) for interfacing with the drive signal in order to create signals of precisely specified frequency, polarity, and duty cycle. Due to this interaction with machine (DTO) resources for the PWM, the PanTilt application must be run using "superuser do" or sudo, i.e. it is invoked as:

sudo ./PanTilt

The file PanTilt.cpp contains the PanTilit application's main() function which performs all initialization and processes a simple text-based, interactive command loop. The PanTilt application is run by first SSH'ing to the BBB using:

ssh jaym@192.168.7.2		password is: jaym

This may be accomplished from a Windows SSH client (such as PuTTY) or from a terminal in the Debian VM.

## Booting Up

Preferred power-up sequence for BBB and the host development (dev) machine:

- Power on the dev machine w/ BBB USB cable plugged into it - BBB boots w/ blue LEDs
- Enable WiFi on dev host to CMU-SECURE
- Open PuTTY to BeagleBone and login as jaym with pw=jaym
- Open another PuTTY instance to BBB and login as root with no pw
- As root, run >./StartUSBNetwork. This sets the time/date via NTP on the BBB and sets the dev PC as default gateway.
- Verify the time with the command >date
- Test that can >ping 8.8.8.8 and >ping google.com - both should work from the BBB as either root or jaym
- Close the root PuTTY window
- Copy each of these 4 lines, in this order, and paste each (via right-click) into the jaym PuTTY terminal:
    sudo sh -c "echo bone_pwm_P9_22 > $SLOTS"
    sudo sh -c "echo am33xx_pwm > $SLOTS"
    sudo sh -c "echo bone_pwm_P9_16 > $SLOTS"
    sudo sh -c "echo am33xx_pwm > $SLOTS"
- Verify with >cat $SLOTS afterward
- Start up the Debian (Jessie) VM for development work
- Open a terminal on the VM and >ssh jaym@192.168.7.2  This is used for running PanTilt during development.
- Open a 2nd terminal on the VM and go to workspace/PanTilt
- Type >git status  to see the state of the the PanTilt app or view history as >git log --oneline
- Start up Eclipse and open the PanTilt project (import | exisitng | browse | finish if not visible)
- A post-build step uses secure copy (scp) to copy the executable to the BBB jaym home as PanTiltDebug

## Usage

PanTilt is a fairly simple 2-axis motion controller. It allows the user to statically set the position of either axis in units of degrees. It also allows the user to query the value of any settable variables, such as the present axis position. Beyond this static positioning, PanTilt also supports "modal" motion control to allow either sinusoidal axis motions or a stepped & swept motion range mode. Short text commands in PanTilt are typed at the ">>" prompt. Query commands always end with a "?" character. Note that the command parsing is far from robust; it expects **exactly** one space character after the command but preceeding any included value. Additional time to bullet-proof the code (especially user interface) may be applied later, but at this time it is unforgiving and may cause odd behaviors for fuzzing typist! 

The "help" command lists all available commands as shown below:

>>help
Usage:      sudo ./PanTiltDebug   Where valid commands are:
Command <space> <argument> 
help        Show (this) help information
quit        Exit the program
axis <n>    Specify axis number [1 or 2]. Retained until changed
axis?       Read present axis number
pos <f>     Statically position axis at a position in degrees [-40.0, 40.0]
pos?        Read present axis position in degrees
zero        Statically position the axis at zero degrees position
amp <f>     Specify sinusoidal motion peak amplitude in degrees [-40.0, 40.0]
amp?        Read present sinusoidal motion peak amplitude in degrees
period <f>  Specify sinusoidal motion period in seconds [1.0, 300.0]
period?     Read present sinusoidal motion period in seconds
cycles <n>  Specify sinusoidal motion number of cycles to complete [1, 100]
cycles?     Read present sinusoidal motion number of cycles to complete
mode <s>    Specify sinusoidal or range motion type [sin, cos, range]
mode?       Read present sinusoidal or range motion type
rezero      Redefine axis zero position to be the present position
duty <n>    Statically position axis at a position in duty cycle microsecs [400, 1200]
duty?       Read present axis position in duty cycle microsecs
trig <n>    Trigger axis to cease (0) or commence (1) motion per the mode [0, 1]
trigall <n> Trigger both axes to cease (0) or commence (1) motion per their mode [0, 1]
ver?        Read the version of this program
Example commands:
>>axis 1
>>pos?
>>25.342
>>pos 10.0
>>mode sin
>>trig 1
>>

### Axis Selection

The axis command is used to select an axis to work with. Once set it remains until changed. So, to set parameters (or static positions of the Pan axis (axis number 2), you issue the command:
>>axis 2

ALl subsequent axis-specific commands will then be directed to axis 2 until it is changed with another axis command.


### Modal Motions

You use the amp, period, cycles, mode, and trig / trigall commands for specifying and executing modal motions. The default mode is "sin" which uses a sine wave to drive a motion of (amp) degrees peak with a period of (period) seconds through a number of full integer (cycles). Alternately, a cosine wave can be chosen. Here the difference is really the 90 (electrical) degree phase shift. This means that is one axis is sin and the other is cos you see the conical shape traced for a vertical (say, pencil) sticking out of the 2D surface, similar to ship surface motion. These are collectively known as "scorsby" motions.

The third choice for mode is "range". This mode causes the selected axis to go through a series of stepped positions through its motion range, followed by a smooth slew back to the beginning. The timing for range mode is pre-set and is unaffected by amp and period.

Once the mode and any other parameters (amp, period, cycles) are specified for each axis (using the axis command to switch part way through the specification) the user triggers modal motion for either the selected axis using "trig" or for both axes simultaneously using "trigall". 

The value for trig and trigall either immediately commences (1) or ceases (0) modal motion. If motion is ceased, it is immediate, which may be used with precise timing where one knows where the axis "should" be after N seconds. This can then be verified using the "pos?" command.

 




  





P
 
 




  



   
