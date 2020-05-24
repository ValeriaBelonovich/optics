#include "classes.hpp"

void Ray::send()
{
	std::string raw_ip_address = "192.168.0.112";

	auto port = 3333;
	std::string ray_send;

	for (auto i = 0;i<2; ++i)
	{
		ray_send += std::to_string(begin[i])+ " ";
	}

	for (auto i = 0; i < 2; ++i)
	{
		ray_send += std::to_string(end[i]) + " ";
	}


	try
	{
		boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::address::from_string(raw_ip_address), port);

		boost::asio::io_service io_service;

		boost::asio::ip::tcp::socket socket(io_service, endpoint.protocol());

		//socket.connect(endpoint);

		std::cout << ray_send << std::endl;

		//boost::asio::write(socket, boost::asio::buffer(ray_send));
	}
	catch (boost::system::system_error& e)
	{
		return;
	}
}

bool Ray::operator==(Ray& ray)
{
	for (size_t i = 0; i < 3; ++i)
	{
		if (ray.begin[i] != begin[i] || (ray.cos[i] != cos[i])) return false;
	}
	return true;
}