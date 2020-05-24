#include "classes.hpp"



Source::Source(json &j)
{
    centre = j["centre"].get<std::valarray<double>>();
}

Ray Source::produce_ray(double ap)
{
    std::random_device seed;
    std::default_random_engine dre(seed());
    std::uniform_real_distribution<double> yzdis(-ap,ap);
    Ray ray;
    ray.begin = centre;
    ray.cos.resize(3);

    ray.cos[0] = 1;
    for (int i = 1; i<3 ;++i)
    {
        ray.cos[i] = yzdis(dre)*ray.cos[0]; 
    }
    ray.cos[2] = 0.0; //убрать потом

    return ray;
    
}