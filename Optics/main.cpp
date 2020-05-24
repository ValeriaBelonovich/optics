#include "classes.hpp"



using nlohmann::json;


int main()
{
	std::cout << "Start"<<std::endl;

	std::string path;

	const std::size_t size = 30;

	auto port = 3333;

	boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::address_v4::any(), port);

	boost::asio::io_service io_service;


	try
	{
		boost::asio::ip::tcp::acceptor acceptor(io_service, endpoint.protocol());

		acceptor.bind(endpoint);

		acceptor.listen(size);

		boost::asio::ip::tcp::socket socket(io_service);

		acceptor.accept(socket);

		boost::asio::streambuf buffer;

		boost::asio::read_until(socket, buffer, '!');

		std::istream input_stream(&buffer);
		std::getline(input_stream, path, '!');



	}
	catch (boost::system::system_error& e)
	{
		std::cout << "Error occured! Error code = " << e.code() << ". Message: " << e.what() << std::endl;

		return e.code().value();
	}

	json j = json::parse(path);

	System sys(j);
	
	sys.run();
	std::cout << "Success" << std::endl;
}
	