#ifndef SERIAL_PORT_READER_H
#define SERIAL_PORT_READER_H

#include <iostream>
#include <string>
#include <thread>
#include <atomic>
#include <chrono>
#include <sstream>
#include <iomanip>
#include <cstring>
#include <boost/asio.hpp>

class SerialPortReader
{
public:
	SerialPortReader();
	boost::asio::io_service io;
	boost::asio::io_context::work work;
	boost::asio::serial_port serial;

private:	
	boost::asio::steady_timer timer;
	boost::asio::streambuf buffer;
	int latest_value = 0;
};

#endif // SERIAL_PORT_READER_H