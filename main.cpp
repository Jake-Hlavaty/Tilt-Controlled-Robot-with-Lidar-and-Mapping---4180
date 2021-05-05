#include "mbed.h"
#include "Motor.h"
#include "XNucleo53L0A1.h"
#include <stdio.h>

//Lidar
DigitalOut shdn(p26); 
Serial blue(p13,p14);

//Motors
Motor LW(p22, p8, p11); // pwm, fwd, rev
Motor RW(p21, p15, p16); // pwm, fwd, rev

#define VL53L0_I2C_SDA   p28
#define VL53L0_I2C_SCL   p27

static XNucleo53L0A1 *board=NULL;

union f_or_char {
    float f;
    char  c[4];
};

int getLidarMeasurement(uint32_t lidarDistance) {
    // Taking measurement and returning distance
    int status = board->sensor_centre->get_distance(&lidarDistance);
    return lidarDistance;
}

double checkForObstacle(int distance) {
    double speed;
    if (distance < 150 && distance > 0) {
        speed = 0;
    } else {
        speed = 1.0;
    }
    return speed;
}

void dpadControl(int bnum, int bhit, int lDist) {
    double s;
    
    switch (bnum) 
    {
        case '5': //button 5 up arrow
            if (bhit=='1') 
            {
                s = checkForObstacle(lDist);
                RW.speed(s);
                LW.speed(-s);
            } else {
                RW.speed(0);
                LW.speed(0);
            }
            break;
        case '6': //button 6 down arrow
            if (bhit=='1') 
            {
                RW.speed(-1.0);
                LW.speed(1.0);
            } else {
                RW.speed(0);
                LW.speed(0);
            }
            break;
        case '7': //button 7 left arrow
            if (bhit=='1') 
            {
                RW.speed(0);
                LW.speed(-1.0);
            } else {
                RW.speed(0);
                LW.speed(0);
            }
            break;
        case '8': //button 8 right arrow
            if (bhit=='1') 
            {
               RW.speed(1.0);
               LW.speed(0);
            } else {
                RW.speed(0);
                LW.speed(0);
            }
            break;
        default:
            break;
    }
    wait(.1);
}

void tiltControlHelper(float x, float y, int lDist) {
    float left;
    float right;
    float th = 0.2;
    
    if (x < -th) {
        if (lDist > 0 && lDist < 150) {
            left = 0;
            right = 0;
        } else {
            left = x;
            right = -x; 
        }
    } else if (x > th) {
        left = x;
        right = -x;
    } else {
        left = 0.0;
        right = 0.0;
    }

    if (y > th) {
        if (x > 0.2) {
            right += y/2;
            left += y/2;
        } else {
            right -= y/2;
            left -= y/2;
        }     
    } else if (y < -th) {
        if (x > 0.2) {
            left += y/2;
            right += y/2;
        } else {
            left -= y/2;
            right -= y/2;
        }
    }
    
    //check that speed is within the range [-1,1]
    if (left > 1.0) {
        left = 1.0;
    } else if (left < -1.0) {
        left = -1.0;
    }
    //check that speed is within the range [-1,1]
    if (right > 1.0) {
        right = 1.0;
    } else if (right < -1.0) {
        right = -1.0;
    }
    
    //set speed
    LW.speed(left);
    RW.speed(right);
}

//bot movement using bluetooth thread
int main()
{
    char bnum=0; //button num
    char bhit=0; //when pressed
    char mode=0; //used to store 'B' or 'A' depending on mode
    union f_or_char x,y,z;
    char bchecksum;
    char temp = 0;
    uint32_t lidarDistance;
    int lidarStatus;
    
    //uLCD.cls();
    // LIDAR initialization start
    DevI2C *device_i2c = new DevI2C(VL53L0_I2C_SDA, VL53L0_I2C_SCL);
    board = XNucleo53L0A1::instance(device_i2c, A2, D8, D2);
    shdn = 0; //must reset sensor for an mbed reset to work
    wait(0.1);
    shdn = 1;
    wait(0.1);
    lidarStatus = board->init_board();
    // LIDAR initialization end

    while(1) 
    {
        int lDist = getLidarMeasurement(lidarDistance);
        
        bchecksum = 0;
        if (blue.getc()=='!') 
        {
            mode = blue.getc();
            if (mode == 'A')
            {
                for (int i=0; i<4; i++) {
                    temp = blue.getc();
                    x.c[i] = temp;
                    bchecksum = bchecksum + temp;
                }
                for (int i=0; i<4; i++) {
                    temp = blue.getc();
                    y.c[i] = temp;
                    bchecksum = bchecksum + temp;
                }
                for (int i=0; i<4; i++) {
                    temp = blue.getc();
                    z.c[i] = temp;
                    bchecksum = bchecksum + temp;
                }
                wait(0.1);
                if (blue.getc()==char(~('!' + 'A' + bchecksum))) { 
                    tiltControlHelper(x.f, y.f, lDist); 
                }
            } 
            else if (mode == 'B')
            {     
                bnum = blue.getc(); //button number
                bhit = blue.getc(); //1=hit, 0=release
                if (blue.getc()==char(~('!' + 'B' + bnum + bhit))) {
                    dpadControl(bnum, bhit, lDist);  
                }  
            }
        }
    }
} 
