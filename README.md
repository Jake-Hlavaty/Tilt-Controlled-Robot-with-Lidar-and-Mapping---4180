# Tilt-Controlled Robot with Lidar and Mapping 4180

The purpose of this project is to create a Bluetooth controlled robot that can be controlled through various methods and will collect orthometric data based on distance travelled, to be displayed on a uLCD screen. Further, the robot is designed to be intelligent enough to avoid collision using lidar readings to not only prevent forward movement, but also utilize this data to help map the environment it is in on the uLCD screen. All features are controlled using the arm mbed LPC1768 processor and accompanying peripherals located in the components portion of this guide.  

### Movement Mode 1: Dpad Controlled

Using the Adafruit Bluefruit mobile application, a user can connect to the bluetooth controller to control the robot via the arrow keys found under the controls tab. When a user presses any of the directional buttons (up, down, left, and right buttons) the DC motors will turn and the robot will move in that direction at max speed until the button is released. Upon pressing the left or right arrows, the robot will turn while stationary until the button is released.Using the lidar sensor we also implemented a feature so that the robot will refuse to move forward if it senses an object in close proximity. The ability to turn or move backwards is still in place so that robot may be adjusted and continue its journey again. 

### Movement Mode 2: Tilt Controlled

Additionally, the Adafruit Bluefruit mobile application can be used to control the robot via tilt controls. This mode allows for much more freedom in movement and functions similarly to a steering wheel on a car. Unlike a steering wheel, this mode also allows you to utilize tilt to adjust the speed of the robot as well. Upon passing a threshold, the robot will calculate its forward or backward velocity based on the tilt of the phone forward or backwards. Then, using trigonometry, the speed of an individual wheel is scaled based on the tilt of the phone left or right allowing the car to take wider or sharper turns as needed. As mentioned before, the car will not allow you to move forward if the lidar senses an object is present. As such, this movement mode provides the operator with more precise turning and speed control allowing for smoother movement and more continuous movement.

### Mapping Functionality: IMU and uLCD

On a separate MBED device, we have added a function to help map the cars surroundings and track its movement. We use the IMU’s accelerometer to calculate acceleration in the x and y direction. We then use a Timer to measure how long this acceleration took place. Using kinematics formulas, we are then able to measure the change in velocity and in distance using vf = vi + a * t, and d = vi * t + .5 * a * t^2. Although the initial measurements are in g’s, we convert to meters, then feet, then set a pixel to feet ratio. To account for the changes in rotation, we use the magnetometer to measure the magnetic force and convert these values into the heading in radians. We use the initial heading and the current heading to calculate the change in rotation, and we then rotate the velocity vector and the distance vector accordingly at each iteration. Although our methodology on paper seems like it should work, the IMU is not always accurate, and every slight calculation error causes a domino effect which can cause errors in the graph. Additionally, if the IMU is tilted, the Earth’s gravity gets measured as acceleration and can cause errors in the graph.


### Components List:
1. Two mbed Microcontroller (LPC1768) - https://www.sparkfun.com/products/9564 
2. Two DC motors for controlling Left and Right wheels - https://www.sparkfun.com/products/13302 
3. H-Bridge: TB6612FNG Breakout - https://os.mbed.com/users/4180_1/notebook/relays1/ 
4. Adafruit Bluefruit LE UART Friend - Bluetooth - https://learn.adafruit.com/introducing-the-adafruit-bluefruit-le-uart-friend 
5. uLCD: uLCD-144-G2 - https://os.mbed.com/users/4180_1/notebook/ulcd-144-g2-128-by-128-color-lcd/ 
6. Lidar (Adafruit VL53L0X Time of Flight Distance Sensor) - https://www.adafruit.com/product/3317 
7. Speaker - PCB Mount - https://www.sparkfun.com/products/11089 
8. TI TPA2005D1 class D audio amp chip - https://www.sparkfun.com/products/11044 
9. Sparkfun Black Robot Kit - https://os.mbed.com/users/4180_1/notebook/shadow-robot-kit/ 

### Pin Layouts

#### *Note that the uLCD and IMU run off of mbed2 for additional serial ports*

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

##### uLCD

| mbed2 | uLCD | Battery |
| ----- | ---- | ------- |
|  5V   |      |    +    |
|  GND  |  GND |         |
|  p9   |  TX  |         |
|  p10  |  RX  |         |
|  p11  | Reset|         |

##### IMU

| mbed2 | IMU |
| ----- | --- |
| 3.3V  | VDD |
|  GND  | GND |
|  p28  | SDA |
|  p27  | SCL |

### Block Diagram

![image](https://user-images.githubusercontent.com/83670606/117174145-6a4e8900-ad9b-11eb-8931-d6c8902cd33b.png)


### Final Notes
Given the relatively short timeline of the project, we were overal very happy with the resulting finl project. Using tilt controls as an additional feature to control the DC motors of the robot felt very robust and innovative. Given more time, we would like to further hone in on this aspect of the design for improved accuracy and more lifeline driving capability. We would like to explore aspects such as energy efficency inside multiple threads to maximize the runtime efficency of our robot and mimimize possible delay errors. 

Furthermore, after finishing all the baseline requirements we set out to accomplish, we wanted to try to add mapping capability utiilizing the second mbed and IMU unit. If we had more time, we would work towards improving this feature of the project to become much more accurate and more reliable. That being said, we implemented a really interesting control unit that can switch between two modes to suit the user and set out to surpass our expectations for the given time restrictions.
