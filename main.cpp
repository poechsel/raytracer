#include <iostream>

#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include "glm/ext.hpp"
#include "includes.h"
#include "utilities/Ray.h"
#include "geometry/mesh.h"
#include <Python.h>
#include "python/pythonfunction.h"
#include "python/python3dloader.h"
#include "utilities/manager3dloader.h"
#include "utilities/meshmanagers.h"
#include "utilities/sfmlimage.h"
#include "utilities/camera.h"
#include "python/pythonsceneloader.h"
#include "intersections/intersections.h"
#include "intersections/intersectionnaivemoller.h"
using namespace std;
Real sigmoid(Real x) {
    //return 1.0/(1+std::exp(-(x-0.5)*12.0));
    //massal
    double invgamma = 0.45;
    return std::pow(1.0 - std::exp(-0.66 * x), invgamma);
}/*
Real intersect(Scene *scene, const Ray &ray, uint *t_inter) {
    Real t = -1;
    for (uint i = 0; i < scene->triangles.size(); ++i){
        //Real t_temp = scene->triangles[i].intersect(ray);
        Real t_temp = intersectionMoller(scene, scene->triangles[i], ray);
        if (t_temp >= 0) {
            if (t < 0 || t_temp < t) {
                t = t_temp;
                *t_inter = i;
            }
        }
    }
    return t;
}*/
int main()
{

    PythonContext context;
    Scene scene;
    context.setCWD("python");
    SFMLImage image (640, 480);
    int completion_percent = 0;

    Camera cameratemp;
    PythonSceneLoader loader_scenes("scenes/suzanne.json");
    std::cout<<"loading========================\n";
    loader_scenes.load(&cameratemp, &scene);
    std::cout<<"end loading========================\n";

    IntersectionMethod *inter_method = new IntersectionNaiveMoller(&scene);
    inter_method->build();
    for (uint y = 0; y < 480; ++y) {
        for (uint x = 0; x < 640; ++x) {
            Ray ray = cameratemp.shoot(x, y);
            uint tri = 0;
            Real t = inter_method->intersect(ray, &tri);
            Color color(0.65, 1, 1);
            if (t >= 0) {
                Real gradient = std::abs(dot(ray.direction, scene.triangles[tri].normal));
                color = Color(gradient, gradient, gradient);
            }
            image.putPixelFloat(x, y, sigmoid(color.r), sigmoid(color.g), sigmoid(color.b));
        }
        if (completion_percent < (int)(((Real)y / 480.0)*100)) {
            completion_percent = ((Real)y / 480.0)*100;
            std::cout<<completion_percent<<"%\n";
        }
    }
    std::cout<<"end\n";
    image.saveTo("test.png");
    delete inter_method;

    return 0;
}
