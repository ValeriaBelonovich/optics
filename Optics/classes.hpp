#pragma once

#include<algorithm>
#include <boost/math/special_functions.hpp>
#include <boost/asio.hpp>
#include <chrono>
#include <cmath>
#include <filesystem>
#include <fstream>
#include<future>
#include<iostream>
#include<mutex>
#include "nlohmann/json.hpp"
#include <random>
#include <shared_mutex>
#include <thread>
#include<valarray>
#include<vector>



using nlohmann::json;

class Ray
{
public:
	using dot = std::valarray<double>;
	using vector = std::valarray<double>;
	Ray()
		: begin({ 0.0,0.0,0.0 }),
		end({ 0.0,0.0,0.0 }),
		cos({ 0.0,0.0,0.0 }),
		n(0)
	{};
	~Ray() = default;
	void send();// послать на отрисовку

	bool operator==(Ray& ray);

	dot begin;
	dot end;
	vector cos;
	bool n;
};

class Object
{
public:
	Object()=default;
	virtual ~Object() {};
	virtual std::valarray<double> intersection(Ray& ray) =0;
	virtual Ray refraction(Ray& ray) { return ray; };
	virtual bool end(Ray& ray) { return 0; };
//private:
	std::valarray<double> centre;
};

class Screen: public Object
{
	using dot = std::valarray<double>; 
public:
	Screen() = default;

	dot intersection(Ray& ray) override;
	bool end(Ray& ray) override;
	double length;
};


class Source
{
public:

	Source()= default;
	Source(json& j);
	~Source() = default;
	Ray produce_ray( double ap); //генерирует луч

private:
	std::valarray<double> centre;
};

class Lense : public Object 
{
	using dot = std::valarray<double>;
	using vector = std::valarray<double>;

public:
	Lense() = default;
	Lense(json& j);

	~Lense() = default;

	Ray refraction(Ray& ray) override; // преломление (выдает луч после выхода из линзы)
	double aperture(dot s);
	dot intersection(Ray& ray) override;

private:
	dot intersection(Ray& ray, double r);
	Ray refraction(Ray& ray,double r); // луч 
	vector slu(double alpha, double beta, vector& normal, vector& ray); //решение слу для определения координат преломленного луча

private:
	//dot centre; //координаты центра
	double length; //длина всей! линзы
	double n; // показатель преломления
	double r_left; //левый радиус кривизны больше нуля если линза выгнута влево
	double r_right; // правый радиус кривизны меньше нуля если выгнута вправо
};

class System 
{
public:
	//System() {};
	System(json & j);
	~System() = default;
	void run();
private:
	void trace();
	double saperture();
	Ray boarders(Ray& ray);
private:
	std::vector<Source> sources;
	std::vector<std::unique_ptr<Object>> system;
	std::vector<Ray> rays;
	std::mutex m_rays;
	std::shared_mutex m_system;
	std::mutex m_send;
	double dx;
	double dy;
};