
#include <stdio.h>
#include "boards.h"
#include "app_util_platform.h"
#include "app_error.h"
#include "nrf_drv_twi.h"
#include "nrf_gpio.h"
#include "mpu6050.h"

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"

#define led1 13
#define led2 14
#define led3 15
#define led4 16

bool led_control(int16_t *x_data, int16_t *y_data)
{
  //NRF_LOG_INFO("LEDs Control:  y = %d  x = %d", *x_data, *y_data);

  if(*x_data > 5000){
    nrf_gpio_pin_clear(led1);
  }else if(*x_data < -5000){
    nrf_gpio_pin_clear(led2);
  }else if(*y_data > 5000){
    nrf_gpio_pin_clear(led3);
  }else if(*y_data < -5000){
    nrf_gpio_pin_clear(led4);
  }else{
    nrf_gpio_pin_set(led1);
    nrf_gpio_pin_set(led2);
    nrf_gpio_pin_set(led3);
    nrf_gpio_pin_set(led4);
  }

}

int main(void)
{
    
  // initialize the logger
  APP_ERROR_CHECK(NRF_LOG_INIT(NULL));
  NRF_LOG_DEFAULT_BACKENDS_INIT();

  nrf_gpio_cfg_output(led1);
  nrf_gpio_cfg_output(led2);
  nrf_gpio_cfg_output(led3);
  nrf_gpio_cfg_output(led4);
  
  // create arrays which will hold x,y & z co-ordinates values of acc and gyro
  static int16_t AccValue[3], GyroValue[3];

  //bsp_board_init(BSP_INIT_LEDS | BSP_INIT_BUTTONS); // initialize the leds and buttons

  twi_master_init(); // initialize the twi 
  nrf_delay_ms(1000); // give some delay

  while(mpu6050_init() == false) // wait until MPU6050 sensor is successfully initialized
  {
    NRF_LOG_INFO("While init."); 
    NRF_LOG_INFO("MPU_6050 initialization failed!!!"); // if it failed to initialize then print a message
    nrf_delay_ms(1000);
  }

  NRF_LOG_INFO("MPU6050 Init Successfully!!!"); 

  NRF_LOG_INFO("Reading Values from ACC & GYRO"); // display a message to let the user know that the device is starting to read the values
  nrf_delay_ms(2000);

  while (true)
  {
    if(MPU6050_ReadAcc(&AccValue[0], &AccValue[1], &AccValue[2]) == true) // Read acc value from mpu6050 internal registers and save them in the array
    {
      NRF_LOG_INFO("ACC Values:  y = %d  x = %d", AccValue[0], AccValue[1]); // display the read values
    }
    else
    {
      NRF_LOG_INFO("Reading ACC values Failed!!!"); // if reading was unsuccessful then let the user know about it
    }

    led_control(&AccValue[0], &AccValue[1]);

    //NRF_LOG_INFO("\n");
    nrf_delay_ms(100); // give some delay 
  }
}

/** @} */
