#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <stdio.h>

int file_i2c;
unsigned char buffer[6] = {0};


int openBus (){
	char *filename = (char*)"/dev/i2c-0";
	if ((file_i2c = open(filename, O_RDWR)) < 0)
	{
	    return 1;
	}
	return 0;
}


int configEncoder (int addy){
	ioctl(file_i2c, I2C_SLAVE, addy);

	// set long presh threshold. 0x32 is 500ms
	buffer[0] = 0x13;
	buffer[1] = 0x32;
	write(file_i2c, buffer, 2);

	// general conf
	buffer[0] = 0x00;
	buffer[1] = 0x00;
	write(file_i2c, buffer, 2);

	// clear counter
	buffer[0] = 0x03;
	buffer[1] = 0x00;
	buffer[2] = 0x00;
	buffer[3] = 0x00;
	buffer[4] = 0x00;
	write(file_i2c, buffer, 5);

	// set max
	buffer[0] = 0x07;
	buffer[1] = 0x7F;
	buffer[2] = 0xFF;
	buffer[3] = 0xFF;
	buffer[4] = 0xFF;
	write(file_i2c, buffer, 5);

	// set min
	buffer[0] = 0x0B;
	buffer[1] = 0x80;
	buffer[2] = 0x00;
	buffer[3] = 0x00;
	buffer[4] = 0x00;
	write(file_i2c, buffer, 5);

	// set step
	buffer[0] = 0x0F;
	buffer[1] = 0x00;
	buffer[2] = 0x00;
	buffer[3] = 0x00;
	buffer[4] = 0x01;
	write(file_i2c, buffer, 5);

	return 0;
}

int wasPushed (int addy){
	ioctl(file_i2c, I2C_SLAVE, addy);
	
	buffer[0] = 0x02;
	write(file_i2c, buffer, 1);
	read(file_i2c, buffer, 1);

	return (buffer[0] && 0x02);
}

int wasLongPushed (int addy){
	ioctl(file_i2c, I2C_SLAVE, addy);

	buffer[0] = 0x02;
	write(file_i2c, buffer, 1);
	read(file_i2c, buffer, 1);

	return (buffer[0] > 0x7);
}

int getVal (int addy){
	ioctl(file_i2c, I2C_SLAVE, addy);

	buffer[0] = 0x03;
	write(file_i2c, buffer, 1);
	read(file_i2c, buffer, 4);

	return (int) (buffer[0] << 24) | (buffer[1] << 16) | (buffer[2] << 8) | (buffer[3]);
}

//int main() {

//	if (openBus()){
//		printf("Failed to open the i2c bus");
//	}

//	int encoderAddy = 0x10;

//	configEncoder(encoderAddy);

//	while(1){
//		printf("%d\n", getVal(encoderAddy));
//	}

//}

