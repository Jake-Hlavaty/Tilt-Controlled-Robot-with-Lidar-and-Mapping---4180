# Tilt-Controlled-Robot-with-Lidar-and-Mapping-4180
The purpose of this project is to create a Bluetooth controlled robot that can be controlled through various methods and will collect orthometric data based on distance travelled, to be displayed on a uLCD screen. Further, the robot is designed to be intelligent enough to avoid collision using lidar readings to not only prevent forward movement, but also utilize this data to help map the environment it is in on the uLCD screen. All features are controlled using the arm mbed LPC1768 processor and accompanying peripherals located in the components portion of this guide.  

### Movement Mode 1: Dpad Controlled

Using the Adafruit Bluefruit mobile application, a user can connect to the bluetooth controller to control the robot via the arrow keys found under the controls tab. When a user presses any of the directional buttons (up, down, left, and right buttons) the DC motors will turn and the robot will move in that direction at max speed until the button is released. Upon pressing the left or right arrows, the robot will turn while stationary until the button is released.Using the lidar sensor we also implemented a feature so that the robot will refuse to move forward if it senses an object in close proximity. The ability to turn or move backwards is still in place so that robot may be adjusted and continue its journey again. 

### Movement Mode 2: Tilt Controlled

Additionally, the Adafruit Bluefruit mobile application can be used to control the robot via tilt controls. Upon pressing the button labeled 1, the robot will switch into tilt control mode. This mode allows for much more freedom in movement and functions similarly to a steering wheel on a car. Unlike a steering wheel, this mode also allows you to utilize tilt to adjust the speed of the robot as well. Upon passing a threshold, the robot will calculate its forward or backward velocity based on the tilt of the phone forward or backwards. Then, using trigonometry, the speed of an individual wheel is scaled based on the tilt of the phone left or right allowing the car to take wider or sharper turns as needed. As mentioned before, the car will not allow you to move forward if the lidar senses an object is present. As such, this movement mode provides the operator with more precise turning and speed control allowing for smoother movement and more continuous movement.

### Components List:
1. Mbed Microcontroller (LPC1768) - https://www.sparkfun.com/products/9564 
2. Two DC motors for controlling Left and Right wheels - https://www.sparkfun.com/products/13302 
3. H-Bridge: TB6612FNG Breakout - https://os.mbed.com/users/4180_1/notebook/relays1/ 
4. Adafruit Bluefruit LE UART Friend - Bluetooth - https://learn.adafruit.com/introducing-the-adafruit-bluefruit-le-uart-friend 
5. uLCD: uLCD-144-G2 - https://os.mbed.com/users/4180_1/notebook/ulcd-144-g2-128-by-128-color-lcd/ 
6. Lidar (Adafruit VL53L0X Time of Flight Distance Sensor) - https://www.adafruit.com/product/3317 
7. Speaker - PCB Mount - https://www.sparkfun.com/products/11089 
8. TI TPA2005D1 class D audio amp chip - https://www.sparkfun.com/products/11044 
9. Sparkfun Black Robot Kit - https://os.mbed.com/users/4180_1/notebook/shadow-robot-kit/ 

### Pin Layout

##### Bluetooth

| Bluetooth  | mbed1 | Battery |
| ---------- | ----- | ------- |
|    GND     |  GND  |         |
|    Vin     |       |   5V    |
|    CTS     |  GND  |         |
|    TXO     |  p14  |         |
|    RXI     |  p13  |         |
|    RTS     |  nc   |         |

##### H-Bridge and Motors

| H-Bridge  | mbed1 | Right Motor | Left Motor | Battery |
| --------- | ----- | ----------- | ---------- | ------- |
|    VM     |       |             |            |    +    |
|    VCC    | VOUT  |             |            |         |
|    GND    | GND   |             |            |         |
|    STBY   | VOUT  |             |            |         |
|    PWMA   | p22   |             |            |         |
|    AIN1   | p8    |             |            |         |
|    AIN2   | p11   |             |            |         |
|    AO1    |       |     +       |            |         |
|    AO2    |       |     -       |            |         |
|    PWMB   | p21   |             |            |         |
|    BIN1   | p15   |             |            |         |
|    BIN2   | p16   |             |            |         |
|    BO1    |       |             |     +      |         |
|    BO2    |       |             |     -      |         |

##### Lidar

| Lidar | mbed1 |
| ----- | ----- |
|  GND  |  GND  |
|  SDA  |  p28  |
|  SCL  |  p27  |
|  SHDN |  p25  |
|  VIN  | Vout  |
