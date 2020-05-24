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
	Ray() {};
	~Ray() = default;
	void send(); // послать на отрисовку

	dot begin;
	dot end;
	vector cos;
	bool n = 0;
};

class Source
{
public:

	Source() {};
	Source(json& j);
	~Source() = default;
	Ray produce_ray( double ap); //генерирует луч

private:
	std::valarray<double> centre;

};

class Lense
{
	using dot = std::valarray<double>;
	using vector = std::valarray<double>;

public:
	Lense()
	{}
	Lense(json& j);

	~Lense() = default;

	Ray refraction(Ray& ray); // преломление (выдает луч после выхода из линзы)
	double aperture();
	dot intersection(Ray& ray);

private:
	dot intersection(Ray& ray, double r);
	Ray refraction(Ray& ray,double r); // луч 
	vector slu(double alpha, double beta, vector& normal, vector& ray); //решение слу для определения координат преломленного луча

private:
	dot centre; //координаты центра
	double length = 0; //длина всей! линзы
	double n = 0; // показатель преломления
	double r_left = 0; //левый радиус кривизны больше нуля если линза выгнута влево
	double r_right = 0; // правый радиус кривизны меньше нуля если выгнута вправо
};

class System 
{
public:
	System() {};
	System(json & j);
	~System() = default;
	void run();
private:
	void trace();
	double saperture();
private:
	std::vector<Source> sources;
	std::vector<Lense> system;
	std::vector<Ray> rays;
	std::mutex m_rays;
	std::shared_mutex m_system;
	std::mutex m_send;
	double dx =0;
	double dy =0;
};