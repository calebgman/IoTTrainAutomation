//Nolan Gilmore
//Port of gateway code to C in order to avoid Python Shenanigans
//Now less Portable and with more complex Serial Operations!!!
//Horay!!!

#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <unistd.h>


//Make da stuff yo!
struct termios tty;

/*
//Serial Port Configuration Whatsits 
int setSerial(char sPort[], int baudRate)
{
    int serial_port = open(sPort, O_RDWR);
    if(tcgetattr(serial_port, &tty) != 0)
    {
        printf("Error %i from tcgetattr: %s\n", errno, strerror(errno));
    }

    //Configuration Stuff:
    tty.c_cflag &= ~PARENB; //Parity
    tty.c_cflag &= ~CSTOPB; //Stop Bits
    tty.c_cflag &= ~CSIZE;  //Clear Byte Size
    tty.c_cflag |= CS8;     //Set Byte Size
    tty.c_cflag |= CREAD | CLOCAL; //Stuff I don't Understand

    tty.c_lflag &= ~ICANON; //Canonical Mode
    tty.c_lflag &= ~ECHO; //Echo
    tty.c_lflag &= ~ISIG; //Signal Chars
    tty.c_iflag &= ~(IXON | IXOFF | IXANY); //Stuff
    tty.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL);

    tty.c_oflag &= ~OPOST; //Special Char Interpretation
    tty.c_oflag &= ~ONLCR; //Newline

    tty.c_cc[VTIME] = 0; 
    tty.c_cc[VMIN] = 0;

    //Baud Rates, Baby
    cfsetispeed(&tty, baudRate);
    cfsetospeed(&tty, baudRate);

    if (tcsetattr(serial_port, TCSANOW, &tty) != 0) {
        printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));
        return -1;
    }   

    return 0;
}
*/


int main(int argc, char *argv[])
{
    
    //All this just to setup serial, in future I'd like above
    //to work
    char sPort[] = "/dev/ttyUSB0";
    int baudrate = 9600;

    int serial_port = open(sPort, O_RDWR);
    if(tcgetattr(serial_port, &tty) != 0)
    {
        printf("Error %i from tcgetattr: %s\n", errno, strerror(errno));
    }

    //Configuration Stuff:
    tty.c_cflag &= ~PARENB; //Parity
    tty.c_cflag &= ~CSTOPB; //Stop Bits
    tty.c_cflag &= ~CSIZE;  //Clear Byte Size
    tty.c_cflag |= CS8;     //Set Byte Size
    tty.c_cflag |= CREAD | CLOCAL; //Stuff I don't Understand

    tty.c_lflag &= ~ICANON; //Canonical Mode
    tty.c_lflag &= ~ECHO; //Echo
    tty.c_lflag &= ~ISIG; //Signal Chars
    tty.c_iflag &= ~(IXON | IXOFF | IXANY); //Stuff
    tty.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL);

    tty.c_oflag &= ~OPOST; //Special Char Interpretation
    tty.c_oflag &= ~ONLCR; //Newline

    tty.c_cc[VTIME] = 0; 
    tty.c_cc[VMIN] = 0;

    //Baud Rates, Baby
    cfsetispeed(&tty, baudrate);
    cfsetospeed(&tty, baudrate);

    if (tcsetattr(serial_port, TCSANOW, &tty) != 0) {
        printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));
    }   

}