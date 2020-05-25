#include "classes.hpp"



using nlohmann::json;

std::string connect()
{
	std::string result;

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
		std::getline(input_stream, result, '!');

		//socket.close();
		//acceptor.close();

	}
	catch (boost::system::system_error& e)
	{
		throw;
		//return e.code().value();//будем перебрасывать ошибку в main
	}

	return result;

}
int main()
{
		while (true)
		{
			try
			{
				std::cout << "Start" << std::endl;

				std::string path = connect();

				std::cout << path;

				if (path.empty()) break;

				json j = json::parse(path);

				System sys(j);

				sys.run();

				//std::filesystem::path path_input = "C:/Users/lbelo/save (1).txt";

				//std::fstream fin(path_input.string(), std::ios::in);

				//std::ostringstream buf;
				//buf << fin.rdbuf();
				//auto path = buf.str();
				//json j = json::parse(path);
				//fin.close();
				////std::cout << j;
				//System sys(j);

				sys.run();
			}
			catch (boost::system::system_error& e)
			{

				return e.code().value();
			}
			catch(...)
			{
				std::cout << "error";
				return 0;
			}
		}
}
	