#include "classes.hpp"


Lense::Lense(json& j)
{
	r_left = j["R1"];
	r_right = -int(j["R2"]);
	length = j["l"];
	n = j["n"];
	centre = j["centre"].get<std::valarray<double>>();

}
double Lense::aperture(dot s)//исключение
{
	return length / 2 / (sqrt(pow(s - centre, 2).sum()));
}

std::valarray<double> Lense::intersection(Ray& ray, double r)
{

	const double eps = 0.00000001;
	dot result = ray.begin;

	dot centre_curve = centre;
	centre_curve[0] += boost::math::sign(r) * sqrt(pow(r, 2) - pow(length / 2, 2));

	double a = pow(ray.cos, 2).sum();
	double b = (2 * (ray.begin - centre_curve) * ray.cos).sum();
	double c = pow(ray.begin - centre_curve,2).sum() - pow(r, 2);

	double discriminant = b * b - 4 * a * c;

	if (discriminant <= 0)
	{
		return result;
	}
	else
	{
		double t;
		if (r > 0) //если линзы выгнута влево
		{
			t = (-b - sqrt(discriminant)) / (2 * a);
			if (t > eps)
			{
				result += ray.cos * t;
				if (std::abs(result[1]) > std::abs(centre[1]) + length / 2) //(pow(result - centre, 2).sum() > length / 2)//проверка на размер
				{
					result = ray.begin;
					return result;
				}
			}
		}
		else
		{
			t = (-b + sqrt(discriminant)) / (2 * a);
			if (t > eps)
			{
				result += ray.cos * t;
				if (std::abs(result[1]) > std::abs(centre[1]) + length / 2)//(pow(result-centre,2).sum() > length/2)
				{
					result = ray.begin;
					return result;
				}
			}
		}
	}

	return result;
}

std::valarray<double> Lense::intersection(Ray& ray)
{
	if (pow(intersection(ray, r_left) - ray.begin, 2).sum() < pow(intersection(ray, r_right) - ray.begin, 2).sum())
	{
		return intersection(ray, r_left);
	}
	else
	{
		return intersection(ray, r_right);
	}
	
}

Ray Lense::refraction(Ray& ray)
{
	if (pow(intersection(ray, r_left) - ray.begin, 2).sum() < pow(intersection(ray, r_right) - ray.begin, 2).sum())
	{
		Ray result = refraction(ray, r_left);
		return refraction(result,r_right);
	}
	else
	{
		Ray result = refraction(ray, r_right);
		return refraction(result, r_left);
	}
}

Ray Lense::refraction(Ray& ray, double r) // может быть не только слева
{
	Ray result = ray;
	dot centre_curve = centre;// радиус кривизны
	centre_curve[0] += boost::math::sign(r) * sqrt(pow(r, 2) - pow(length / 2, 2));
	vector normal = (centre_curve - intersection(ray, r))*boost::math::sign(r);

	double alpha = acos((ray.cos * normal).sum() / sqrt(pow(normal, 2).sum() * pow(ray.cos, 2).sum()));
	double beta;
	try
	{
		auto s = ray.n;
		auto p = [s](){return s ? 1 : -1; };
		if (sin(alpha) * pow(n, p()) > 1.0) throw 0;
		beta = asin(sin(alpha) * pow(n, p()));
		result.n = !ray.n;
	}
	catch (...)
	{
		ray.end = intersection(ray, r);
		ray.send();
		throw;
	}
	/*if (ray.n)
	{
		if (sin(alpha) * n > 1.0)
		{

			result.begin = intersection(ray, r);
			result.cos = { 0,0,0 };
			result.end = result.begin;
			throw result;

		}
		else
		{
			beta = asin(sin(alpha) * n);
			result.n = 0;
		}
	}
	else
	{
		beta = asin(sin(alpha) / n);
		result.n = 1;
	}*/

	result.begin = intersection(ray, r);
	result.cos = slu(alpha, beta, normal, ray.cos);
	ray.end = result.begin;
	ray.send();
	return result;
}



std::valarray<double> Lense::slu(double alpha, double beta, vector& normal, vector& ray) 
{
	vector result = {0,0,0};
	std::valarray<double> system(12);


	system[std::slice(0, 3, 1)] = ray;
	system[3] = sqrt(pow(ray, 2).sum()) * cos(std::abs(alpha - beta));
	system[std::slice(4, 3, 1)] = normal;
	system[7] = sqrt(pow(normal, 2).sum()) * cos(beta);
	system[std::slice(8, 3, 1)] = ray.cshift(1) * normal.cshift(2) - ray.cshift(2) * normal.cshift(1);
	system[11] = 0;

	
	int col = 0;
	const double eps = 0.0000001;
	while (col < 3)
	{
		int max=col*4;
		double max_value = eps;
		for (int i = col; i < 3; ++i)
		{
			std::valarray<double> current = system[std::slice(i*4+col, 3 - col, 1)];
			max_value = std::max(std::abs(current[0]), max_value);
			if (max_value == current[0]) max = i*4;
			if (std::abs(current[0]) > eps)  system[std::slice(i * 4 + col, 4 - col, 1)] /= std::valarray<double>(current[0], 4 - col);//нормировка

		}


		std::valarray<double> temp_1 = system[std::slice(col*4, 4, 1)];
		std::valarray<double> temp_2 = system[std::slice(max, 4, 1)];
		system[std::slice(col*4, 4, 1)] = temp_2;
		system[std::slice(max, 4, 1)] = temp_1;

		for (int i = col+1; i < 3; ++i)
		{
			if (std::abs(system[i*4+col]) > eps) system[std::slice(i * 4, 4, 1)] -= system[std::slice(col*4, 4, 1)];
		}
		col++;
	}

	for (int str = 2; str >= 0; str--)
	{
		result[str] = system[4 * str + 3];	
		int q = std::abs(str - 2);
		for (int i = 0; i < q; ++i)
		{
			result[str] -= result[str + i+1] * system[str * 4 +str + i+1];
		}
		

	}
	return result;

}



