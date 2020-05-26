#include "classes.hpp"



using nlohmann::json;


int main()
{
		/*while (true)
		{
			try
			{*/

				/*std::string path = connect();

				std::cout << path<<"\n";

				if (path.empty()) break;

				json j = json::parse(path);

				System sys(j);

				sys.run();*/

				//std::filesystem::path path_input = "C:/Users/lbelo/save (1).txt";

				//std::fstream fin(path_input.string(), std::ios::in);

				//std::ostringstream buf;
				//buf << fin.rdbuf();
				//auto path = buf.str();
				//json j = json::parse(path);
				//fin.close();
				////std::cout << j;
				//System sys(j);

			/*	sys.run();
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
		}*/

	Service io_service = Service();
	std::string s = io_service.read();
	std::cout << s;
	s = "return back";
	io_service.write(s);
	s = "!";
	io_service.write(s);
}
	