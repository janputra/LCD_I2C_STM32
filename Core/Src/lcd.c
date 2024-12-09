#include "lcd.h"
#include "i2c.h"
#include "gpio.h"

#define LCD_ADDR   0x27

//    Piggyboard Configuration
//    P7 P6 P5 P4 P3 P2 P1 P0
//    D7 D6 D5 D4 BT E RW RS 

#define PIN_RS 0b00000001 
#define PIN_EN 0b00000100  
#define PIN_BT 0b00001000

void LCDInit(void);
void LCDSendCommand(uint8_t cmd);
void LCDSendData(uint8_t data);


void LCDInit(void)
{
    HAL_Delay(50);

    LCDSendCommand(0x30);
    HAL_Delay(5);
    LCDSendCommand(0x30);
    HAL_Delay(5);
    LCDSendCommand(0x30);
    HAL_Delay(5);
    LCDSendCommand(0x2);
    HAL_Delay(5);
    // 4-bit mode, 2 lines, 5x8 format
	LCDSendCommand(0x28);
    HAL_Delay(5);
	// display & cursor home (keep this!)
	LCDSendCommand(0x02);
    HAL_Delay(5);
    //  mode set --> I/D = 1 (increment cursor) & S = 0 (no shift)
    LCDSendCommand(0x06);
     HAL_Delay(5);
	// display on, cursor off, cursor blink off
	LCDSendCommand(0x0C);
     HAL_Delay(5);
	// clear display (optional here)
	LCDSendCommand(0x01);
     HAL_Delay(5);
    

}

void LCDSendCommand(uint8_t cmd)
{
    uint8_t buffer[4] = {0};
    uint8_t msb = cmd & 0xF0;
    uint8_t lsb = (cmd << 4 ) & 0xF0;

    buffer[0] = msb|PIN_BT|PIN_EN;
    buffer[1] = msb|PIN_BT;
    buffer[2] = lsb|PIN_BT|PIN_EN;
    buffer[3] = lsb|PIN_BT;

    for (int i = 0 ; i < 4;i++)
    {
        if(HAL_I2C_Master_Transmit(&hi2c1, LCD_ADDR<<1,&buffer[i],1,100) == HAL_ERROR)
        {
            HAL_GPIO_WritePin(GPIOG, GPIO_PIN_14, GPIO_PIN_SET);
        }else{
            HAL_GPIO_WritePin(GPIOG, GPIO_PIN_14, GPIO_PIN_RESET);
        }
        HAL_Delay(1);
    }

}

void LCDSendData(uint8_t data)
{
    uint8_t buffer[4] = {0};
    uint8_t msb = data & 0xF0;
    uint8_t lsb = (data << 4 ) & 0xF0;

    buffer[0] = msb|PIN_RS|PIN_BT|PIN_EN;
    buffer[1] = msb|PIN_RS|PIN_BT;
    buffer[2] = lsb|PIN_RS|PIN_BT|PIN_EN;
    buffer[3] = lsb|PIN_RS|PIN_BT;
    for (int i = 0 ; i < 4;i++)
    {
        if(HAL_I2C_Master_Transmit(&hi2c1, LCD_ADDR<<1,&buffer[i],1,100) == HAL_ERROR)
        {
             HAL_GPIO_WritePin(GPIOG, GPIO_PIN_14, GPIO_PIN_SET);
        }else{
            HAL_GPIO_WritePin(GPIOG, GPIO_PIN_14, GPIO_PIN_RESET);
        }
        HAL_Delay(1);
    }
    

   
    
}

void LCDSendString(char *str,uint8_t row, uint8_t col) 
{
    uint8_t pos = col;

    switch (row)
    {
        case 0:
            pos |= 0x80;
            break;
        case 1:
            pos |= 0xC0;
            break;
        case 2:
            pos |= 0x94;
            break;
        case 3:
            pos |= 0xD4;
            break;
        
        default:
            break;
    }

    LCDSendCommand(pos);

    while(*str)
    {
        LCDSendData((uint8_t)*str++);
    }

}