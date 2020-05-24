#include "classes.hpp"



using nlohmann::json;


int main()
{
	/*std::cout << "Start" << std::endl;
	

		std::string path;

		const std::size_t size = 1;

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
			return e.code().value();
		}*/

		/*json j = json::parse(path);

		System sys(j);

		sys.run();*/

	std::filesystem::path path_input = "C:/Users/lbelo/save (1).txt";

	std::fstream fin(path_input.string(), std::ios::in);

	std::ostringstream buf;
	buf << fin.rdbuf();
	auto path = buf.str();
	json j = json::parse(path);
	fin.close();
	std::cout << j;
	System sys(j);

	/*Ray ray;
	ray.begin = { -50,5,0 };
	ray.cos = {10,1,0};
	
	sys.rays = { ray };*/

	sys.run();

	Lense l;

	/*try
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

		std::string end;

		input_stream >> end;


	}
	catch (boost::system::system_error& e)
	{
		return e.code().value();
	}*/
	
}
	