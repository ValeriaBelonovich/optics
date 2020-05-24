#include "classes.hpp"



System::System(json & j) 
{
	for (auto i : j["lens"])
	{
		system.emplace_back(new Lense(i));
	}

	for (auto i : j["sour"])
	{
		sources.push_back(Source(i));
	}

	dx = j["boarders"]["dx"];
	dy = j["boarders"]["dy"];

};

Ray System::boarders(Ray& ray)
{
	Ray result = ray;
	auto t_1 = (dx - ray.begin[0]) / ray.cos[0];
	if (boost::math::sign(t_1) < 0)
	{
		t_1 = (-dx - ray.begin[0]) / ray.cos[0];
	}
	auto t_2 = (dy - ray.begin[1]) / ray.cos[1];
	if (boost::math::sign(t_2) < 0)
	{
		t_2 = (-dy - ray.begin[1]) / ray.cos[1];
	}

	t_1 = std::min(t_1, t_2);

	ray.end = ray.begin + t_1 * ray.cos;
	{
		std::scoped_lock<std::mutex> rays_lock(m_send);
		ray.send();
	}
	result.begin = { dx + 1.0,0.0,0.0 };

	return result;
}

void System::trace()
{

	Ray ray;
	{
		std::scoped_lock<std::mutex> rays_lock(m_rays);
		if (!rays.empty())
		{
			ray = rays.back();
			rays.pop_back();
		}
		else
		{
			return;
		}
	}
	
	{
			std::shared_lock<std::shared_mutex> rays_lock(m_system);
			//Lense lense;
			double n = 4*(pow(dx,2) + pow(dy,2));
			const double eps = 0.000000001;
			double min_dist = n;//задается границами n

			while (ray.begin[0] >= -dx && ray.begin[0] <= dx && ray.begin[1] >= -dy && ray.begin[1] <= dy) //добавить границу по z и границы dx, dy, dz и спросить от нуля ли идет отсчет
			{
				Ray result;
				int current = -1;
				for (size_t i = 0; i < system.size(); ++i) //проходимся по линзам
				{
					auto dist = pow(system[i]->intersection(ray) - ray.begin, 2).sum(); 
					if (min_dist > dist && dist > eps)
					{
						min_dist = dist;
						current = i;
					}
				}
				if (current>-1)
				{
					
						bool b;
						{
							std::scoped_lock<std::mutex> rays_lock(m_send);
							b = system[current]->end(ray);
						}
						if (b)
						 {
							 return;
						 }
						 {
							 std::scoped_lock<std::mutex> rays_lock(m_send);
							 try 
							 {
								 result = system[current]->refraction(ray);
							 }
							 catch (Ray& r)
							 {
								 //ray.send();
								 return;
							 }
						 }
						 /*if (result == ray)// вроде была проверка на то тот ли это объект, но странная
						 {
							 ray = result;
							 min_dist = n;
							 continue;
						 }*/
					

					ray = result;
				}
				else
				{
					ray = boarders(ray);
				}

			}
	}

		return;
}

double System::saperture()
{
	//for(auto i : system) aperture();
	return 1;
}

void System::run()
{
	size_t discr = 1;//задается
	for (size_t i = 0; i < sources.size(); ++i)
	{
		for (size_t j = 0; j < discr; ++j)
		{
			rays.push_back(sources[i].produce_ray(saperture()));
		}
	}

	struct thr
	{
		thr()
		{
			max_count = std::thread::hardware_concurrency() - 1;
		}
		~thr()
		{
			for (size_t i=0; i < threads.size(); ++i)
			{
				if(threads[i].joinable())
				{
					threads[i].join();
				}
			}
		}
		size_t max_count;
		std::vector<std::thread> threads;
		std::vector<std::future<void>> futures;
		size_t count_tasks =0;
	};

	thr t = thr();
	
	auto size = rays.size();

	while (!rays.empty())
	{
		if (t.count_tasks < size)
		{
			bool new_thr = 0;
			for (size_t i = 0; i < t.threads.size(); ++i)
			{
				auto condition = t.futures[i].wait_for(std::chrono::milliseconds(0));
				if (condition == std::future_status::ready)
				{
					std::packaged_task<void()> task(std::bind(&System::trace, this));
					t.futures[i] = task.get_future();
					t.threads[i].join();
					t.threads[i] = std::thread(std::move(task));
					new_thr = 1;
					t.count_tasks++;
					break;
				}
			}

			if ((t.threads.size() < t.max_count) && !new_thr)
			{
				std::packaged_task<void()> task(std::bind(&System::trace, this));
				t.futures.push_back(task.get_future());
				t.threads.push_back(std::thread(std::move(task)));
				new_thr = 1;
				t.count_tasks++;
			}
		}
		else break;
	}

	

}




