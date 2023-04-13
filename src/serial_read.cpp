#include "serial_read.h"

SerialPortReader::SerialPortReader() : io(), serial(io), timer(io), work(io){
	serial.open("COM5");
	serial.set_option(boost::asio::serial_port_base::baud_rate(9600));
}

//void run_forever() {
//	while (true) {
//		std::cout << "test" << std::endl;
//	}
//}
//
//int main() {
//	run_forever();
//	std::cout << "finish" << std::endl;
//}