#include"i2c-master.h"

/*
	Setup( or Initiate) I2C (For Master Mode)
*/
void I2C_Master_Init(const unsigned long I2C_BaudRate)
{
	SSPCON = 0x28;      // Slew Rate control is disabled
	SSPCON2 = 0x00;
	//SSPSTAT = 0x00;
	SSPSTAT = 0x80;    // Slew Rate control is disabled
	SSPADD = (unsigned char)( _XTAL_FREQ/(4*I2C_BaudRate) ) - 1;    // set I2C clock frequency
	
	// Make SDA and SCK pins input
	SCK_DIR = 1;	//sck - trisC3    
	SDA_DIR = 1;	//sda - trisC4
}


/*
 The I2C_Wait() function which makes your master MCU sure that 
 the previous state has reached completion successfully and the bus is on IDLE,
 so it can proceed to the desired command.
 
 Wait for MSSP module to be free (not busy).
*/
void I2C_Wait()
{
  while ((SSPSTAT & 0x04) || (SSPCON2 & 0x1F));
}


/*
    Start an I2C start condition
*/
void I2C_Start()
{
	I2C_Wait();
	SEN = 1;    // initiate start condition
}


/*
    Initiate an I2C restart condition( repeated start)
*/
void I2C_Restart()
{
	I2C_Wait();
	RSEN = 1;   // initiate repeated start condition
}


/*
    Stop Condition
*/
void I2C_Stop()
{
	I2C_Wait();
	PEN = 1;    // initiate stop condition
}

/*
    Send ACK Signal (For Master Receiver Mode Only)
    ack = 0, nack = 1,
    only master mode
 */
void I2C_ACK(void)
{
	ACKDT = 0; // 0 -> ACK
	I2C_Wait();
	ACKEN = 1; // Send ACK
}


/*
    Send NACK Signal (For Master Receiver Mode Only)
    ack = 0 --> nack = 1
    only master mode
 */
void I2C_NACK(void)
{
	ACKDT = 1; // 1 -> NACK
	I2C_Wait();
	ACKEN = 1; // Send NACK
}


/*
    Send Byte Via I2C Bus, And Return The ACK/NACK From The Slave
 */
unsigned char I2C_Write_Char(unsigned char i2c_data)
{
  I2C_Wait();
  SSPBUF = i2c_data;    // update buffer
  
  I2C_Wait();
  return ACKSTAT;
}


/*
    Receive And Return A Byte From The I2C Bus
 */
unsigned char I2C_Read_Char(char ACK)
{
    uint8_t _data;
    
    I2C_Wait();
    RCEN = 1;       // Enable & Start Reception
    I2C_Wait();
    _data = SSPBUF; // Return The Received Byte( or  read data from buffer))
    I2C_Wait();
    
    // send acknowledge pulse ? (depends on ack, if 0 send, otherwise don't send)
    if(ACK==0)
       I2C_ACK();
    else
       I2C_NACK();
    
    return _data;   // return data read
}



/*
cach 2: thu vien i2c
KHAC NHAU: funstion Wait


#define write_cmd 0xA0
#define read_cmd 0xA1

void i2c_init();
void i2c_start();
void i2c_stop();
void i2c_restart();
void i2c_ack();
void i2c_nak();
void waitmssp();
void i2c_send(unsigned char dat);
void i2c_send_byte(unsigned char addr,unsigned char *count);
unsigned char i2c_read();
unsigned char i2c_read_byte(unsigned char addr);


// Function Purpose: Produce approximate delay in given uSecs.
void I2C_Init(void)
{
    TRISCbits.TRISC4 = 1;    // Make SDA and
    TRISCbits.TRISC4 = 1;    // SCK pins input
    SSPADD  = ((_XTAL_FREQ/4000)/I2C_SPEED) - 1;
    SSPSTAT = 0x80;    // Slew Rate control is disabled
    SSPCON  = 0x28;    // Select and enable I2C in master mode
}

void I2C_Start()
{
    SEN=1;// Send start bit
    Wait_Mssp();
}

void I2C_Restart(void)
{
    RSEN=1;// Send Restart bit
    Wait_Mssp();
}


void I2C_Stop(void)
{
    PEN=1;// Send stop bit
    Wait_Mssp();
}


void I2C_Send_ACK(void)
{
    ACKDT=0;	// 0 means ACK
    ACKEN=1;	// Send ACKDT value
    Wait_Mssp();
}

void I2C_Send_NACK(void)
{
    ACKDT=1;	// 1 means NACK
    ACKEN=1;	// Send ACKDT value
    Wait_Mssp();
}

void Wait_Mssp()
{
    while(!SSPIF);// Wait for it to complete
    SSPIF=0;// Clear the flag bit
}

//I2C_Write_Byte transfers one byte
bit I2C_Write_Char(unsigned char Byte)
{
    SSPBUF = Byte;    // Send Byte value
    Wait_Mssp();
    return ACKSTAT;    // Return ACK/NACK from slave
}

void i2c_send(unsigned char dat)
{
L1: SSPBUF=dat;
    waitmssp();
    while(ACKSTAT){i2c_restart;goto L1;}
}





//I2C_Read_Byte reads one byte
unsigned char I2C_Read_Char(void)
{
    RCEN=1;	// Enable reception of 8 bits
    Wait_Mssp();
    return SSPBUF;	// Return received byte
}  
*/






/////////////////////////CHUA KIEM TRA////////////////////////////////////

/*
void i2c_send_byte(unsigned char addr,unsigned char *count)
{
    i2c_start();
    i2c_send(write_cmd);
    i2c_send(addr>>8);
    i2c_send(addr);
    while(*count) {
        i2c_send(*count++);
    }
    i2c_stop();
}


unsigned char i2c_read_byte(unsigned char addr)
{
    unsigned char rec;
L:  i2c_restart();
    SSPBUF=write_cmd;
    waitmssp();
    while(ACKSTAT){goto L;}
    i2c_send(addr>>8);
    i2c_send(addr);
    i2c_restart();
    i2c_send(read_cmd);
    rec=i2c_read();
    i2c_nak();
    i2c_stop();
    return rec;
}

*/


/*
//////////////////////////CHUA XEM SET KY LUONG///////////////////////////////////


void I2C_Write_String(unsigned char addr,unsigned char *count)
{
    I2C_Master_Start();
    I2C_Master_Write(write_cmd);
    I2C_Master_Write(addr>>8);
    I2C_Master_Write(addr);
    while(*count) 
    {
        I2C_Master_Write(*count++);
    }
    I2C_Master_Stop();
}


unsigned char I2C_Read_String(unsigned char addr)
{
    unsigned char rec;
L:  I2C_master_RepeatedStart();
    SSPBUF=write_cmd;
    I2C_Master_Wait();
    while(ACKSTAT)
    {
        goto L;
    }
    I2C_Master_Write(addr>>8);
    I2C_Master_Write(addr);
    I2C_Master_RepeatedStart();
    I2C_Master_Write(read_cmd);
    rec = I2C_Master_Read();
    I2C_NACK();
    I2C_Master_Stop();
    return rec;
}
*/
