/* 
This cpp file contains the implementations required for the vehicle's mapping interface, that draws out the vehicle's path on the uLCD.
*/

#include "mbed.h"
#include "LSM9DS1.h"
#include "uLCD_4DGL.h"
#define PI 3.14159
#define DECLINATION -4.94 // Declination (degrees) in Atlanta,GA.

Serial pc(USBTX, USBRX);
uLCD_4DGL uLCD(p28,p27,p11); // serial tx, serial rx, reset pin;

int main()
{
    LSM9DS1 IMU(p9, p10, 0xD6, 0x3C);
    IMU.begin();
    if (!IMU.begin()) {
        pc.printf("Failed to communicate with LSM9DS1.\n");
    }
    IMU.calibrate(1);
    IMU.calibrateMag(0);
    
    int radius = 1;
    int center = 64;
    float xpos = 0;
    float ypos = 0;
    float xvel = 0;
    float yvel = 0;
    
    float heading;
    float i_heading = -1;
    float h_diff;
    float ax, ay, mx, my;
    float xtemp, ytemp;
    
    float cosH;
    float sinH;
    
    float times = 0.01;
    double factor = 9.8 * 3.28 * 6 / 1000; // 9.8 m/s^2 * 3.28 ft/m * 6 px/ft * milliseconds
    double t_r = 0;
    
    Timer t;
    
    while(1) {
        t.start();
        
        
        while(!IMU.tempAvailable());
        IMU.readTemp();
        while(!IMU.magAvailable(X_AXIS));
        IMU.readMag();
        while(!IMU.accelAvailable());
        IMU.readAccel();
        while(!IMU.gyroAvailable());
        IMU.readGyro();
        
        mx = -1 * IMU.mx;
        my = IMU.my; 
        if (my == 0.0)
            heading = (mx < 0.0) ? 180.0 : 0.0;
        else
            heading = atan2(mx, my)*360.0/(2.0*PI);
        heading -= DECLINATION; //correct for geo location
        if(heading>180.0) heading = heading - 360.0;
        else if(heading<-180.0) heading = 360.0 + heading;
        else if(heading<0.0) heading = 360.0  + heading;
        
        if (i_heading < 0) i_heading = heading;
        
        h_diff = heading - i_heading; // measure difference in angle to measure rotation
        
        i_heading = heading;
        
        cosH = cos(h_diff * PI / 180.0);
        sinH = sin(h_diff * PI / 180.0);
        
        
        // pc.printf("gyro:  %9f %9f %9f in deg/s\n\r", IMU.calcGyro(IMU.gx), IMU.calcGyro(IMU.gy), IMU.calcGyro(IMU.gz));
        //uLCD.printf("xpos: %4f | ypos: %4f | t_r: %4f \n\r", xpos, ypos, t_r);
        //uLCD.printf("Magnetic Heading: %f degress\n\r",heading);
        // uLCD.cls();
        
        uLCD.filled_circle(center - ypos, center + xpos, radius, 0xFFFF00);
        
        ax = IMU.calcAccel(IMU.ax) * factor;
        ay = IMU.calcAccel(IMU.ay) * factor;
        
        t.stop();
        
        t_r = t.read(); // time
        
        xtemp = (xvel * t_r) + (.5 * ax * t_r * t_r); // d = d0 + v*t + .5*a*t^2
        ytemp = (yvel * t_r) + (.5 * ay * t_r * t_r); // d = d0 + v*t + .5*a*t^2
        
        xpos += ytemp*sinH + xtemp*cosH;
        ypos += ytemp*cosH - xtemp*sinH;
        
        
        xtemp = ax * t_r; // vf = vi + at
        ytemp = ay * t_r; // vf = vi + at
        
        xvel += ytemp*sinH + xtemp*cosH;
        yvel += ytemp*cosH - xtemp*sinH;
        
                      
        wait(times);
        
    }
}
