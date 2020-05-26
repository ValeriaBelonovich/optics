#include "classes.hpp"

void Service::write(std::string& s)
{

	try
	{
		boost::asio::write(socket, boost::asio::buffer(s));

	}
	catch (boost::system::system_error& e)
	{
		std::cout << e.code();
		return;
	}
	return;
}
std::string Service::read()
{
	std::string result;

	auto port = 3333;
	size_t size = 1;

	boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::address_v4::any(), port);

	try
	{
		boost::asio::ip::tcp::acceptor acceptor(io_service, endpoint.protocol());

		acceptor.bind(endpoint);

		acceptor.listen(size);

		//boost::asio::ip::tcp::socket socket(io_service);

		acceptor.accept(socket);

		boost::asio::streambuf buffer;

		boost::asio::read_until(socket, buffer, '!');


		std::istream input_stream(&buffer);
		std::getline(input_stream, result, '!');

	}
	catch (boost::system::system_error& e)
	{
		std::cout << e.code();
		return "";
	}
	return result;
}