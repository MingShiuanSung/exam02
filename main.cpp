#include "mbed.h"

#include "fsl_port.h"

#include "fsl_gpio.h"

#define UINT14_MAX        16383

// FXOS8700CQ I2C address

#define FXOS8700CQ_SLAVE_ADDR0 (0x1E<<1) // with pins SA0=0, SA1=0

#define FXOS8700CQ_SLAVE_ADDR1 (0x1D<<1) // with pins SA0=1, SA1=0

#define FXOS8700CQ_SLAVE_ADDR2 (0x1C<<1) // with pins SA0=0, SA1=1

#define FXOS8700CQ_SLAVE_ADDR3 (0x1F<<1) // with pins SA0=1, SA1=1

// FXOS8700CQ internal register addresses

#define FXOS8700Q_STATUS 0x00

#define FXOS8700Q_OUT_X_MSB 0x01

#define FXOS8700Q_OUT_Y_MSB 0x03

#define FXOS8700Q_OUT_Z_MSB 0x05

#define FXOS8700Q_M_OUT_X_MSB 0x33

#define FXOS8700Q_M_OUT_Y_MSB 0x35

#define FXOS8700Q_M_OUT_Z_MSB 0x37

#define FXOS8700Q_WHOAMI 0x0D

#define FXOS8700Q_XYZ_DATA_CFG 0x0E

#define FXOS8700Q_CTRL_REG1 0x2A

#define FXOS8700Q_M_CTRL_REG1 0x5B

#define FXOS8700Q_M_CTRL_REG2 0x5C

#define FXOS8700Q_WHOAMI_VAL 0xC7


I2C i2c( PTD9,PTD8);

Serial pc(USBTX, USBRX);

DigitalIn  Switch(SW3);

DigitalOut redLED(LED1);

int m_addr = FXOS8700CQ_SLAVE_ADDR1;


void FXOS8700CQ_readRegs(int addr, uint8_t * data, int len);

void FXOS8700CQ_writeRegs(uint8_t * data, int len);

void logger();


int main() {


   pc.baud(115200);


   uint8_t data[2];


   // creates a queue with the default size

   EventQueue queue;


   // Enable the FXOS8700Q

   FXOS8700CQ_readRegs( FXOS8700Q_CTRL_REG1, &data[1], 1);

   data[1] |= 0x01;

   data[0] = FXOS8700Q_CTRL_REG1;

   FXOS8700CQ_writeRegs(data, 2);



   while (true) {

      if( Switch == 0 ) {

         redLED = 0;

         queue.call(logger);

         queue.dispatch(10000);

      }

      else{

         redLED = 1;

      }

   }

}


void FXOS8700CQ_readRegs(int addr, uint8_t * data, int len) {

   char t = addr;

   i2c.write(m_addr, &t, 1, true);

   i2c.read(m_addr, (char *)data, len);

}


void FXOS8700CQ_writeRegs(uint8_t * data, int len) {

   i2c.write(m_addr, (char *)data, len);

}

void logger() {

   uint8_t res[6];

   int16_t acc16;

   float t[3];

   float x_acc[100];

   float y_acc[100];

   float z_acc[100];

   float dis_x = 0.0;

   float dis_y = 0.0;

   for (int i = 0; i < 100; ++i) {

      FXOS8700CQ_readRegs(FXOS8700Q_OUT_X_MSB, res, 6);


      acc16 = (res[0] << 6) | (res[1] >> 2);

      if (acc16 > UINT14_MAX/2)

         acc16 -= UINT14_MAX;

      t[0] = ((float)acc16) / 4096.0f;


      acc16 = (res[2] << 6) | (res[3] >> 2);

      if (acc16 > UINT14_MAX/2)

         acc16 -= UINT14_MAX;

      t[1] = ((float)acc16) / 4096.0f;


      acc16 = (res[4] << 6) | (res[5] >> 2);

      if (acc16 > UINT14_MAX/2)

         acc16 -= UINT14_MAX;

      t[2] = ((float)acc16) / 4096.0f;

      x_acc[i] = t[0];

      y_acc[i] = t[1];

      z_acc[i] = t[2];

      wait(0.1);

   }

   for (int i = 0; i < 100; i++) {
      pc.printf("%1.4f\r\n", x_acc[i]);
      wait(0.01);
   }

   for (int i = 0; i < 100; i++) {
      pc.printf("%1.4f\r\n", y_acc[i]);
      wait(0.01);
   }

   for (int i = 0; i < 100; i++) {
      pc.printf("%1.4f\r\n", z_acc[i]);
      wait(0.01);
   }

   for (int i = 0; i < 100; i++) {

      dis_x += (x_acc[i] * 9.8) * 0.005 * 100;

      dis_y += (y_acc[i] * 9.8) * 0.005 * 100;

      if (dis_x > 5 || dis_x < -5 || dis_y > 5 || dis_y < -5)
         pc.printf("1.0000\r\n");
      else
         pc.printf("0.0000\r\n");

      wait(0.01);
   }
}