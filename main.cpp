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
#include "utilities/meshmanagers.h"
#include "utilities/sfmlimage.h"
#include "utilities/camera.h"
#include "python/pythonsceneloader.h"
#include "intersections/intersections.h"
#include "intersections/intersectionnaivemoller.h"
#include "intersections/intersectiongrid.h"
#include "intersections/intersectionkdtreespacemedian.h"
#include "intersections/intersectionkdtreegeometrymedian.h"
#include "intersections/intersectionkdtreesah.h"
#include "intersections/intersectionkdtreesahnlogn.h"
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
    int completion_percent = 0;

    Camera cameratemp;
    PythonSceneLoader loader_scenes("scenes/boudha.json");
    std::cout<<"loading========================\n";
    loader_scenes.load(&cameratemp, &scene);
    for (auto it = scene.meshes.begin(); it != scene.meshes.end(); ++it) {
            std::cout<<"manager->Mesh: "<<it->first<<" -> "<<&(it->second)<<"\n";
            std::cout<<"    Faces: "<<it->second->triangles.size()
                     <<" Vertices: "<<it->second->vertices.size()<<"\n";
    }
    std::cout<<"end loading========================\n";

    //IntersectionMethod *inter_method = new IntersectionNaiveMoller(&scene);
    //IntersectionMethod *inter_method = new IntersectionGrid(&scene, 0.5);
    //IntersectionMethod *inter_method = new IntersectionKdTreeGeometryMedian(&scene, true);
    //IntersectionMethod *inter_method = new IntersectionKdTreeSAH(&scene, 15, 20, NLOG2N);
    IntersectionMethod *inter_method = new IntersectionKdTreeSAHnlogn(&scene, 15, 20);
    //IntersectionMethod *inter_method2 = new IntersectionKdTreeSAH(&scene, 15, 20, NLOG2N);
    ULARGE_INTEGER time = getTime();
    //inter_method2->build();
    std::cout<<"change method\n";
    inter_method->build();
    std::cout<<"build done in "<<getTimeElapsed(time)<<"\n";
    time = getTime();
    int nb = 0;
    SFMLImage image (cameratemp.width, cameratemp.height);
    for (uint y = 0; y < cameratemp.height; ++y) {
        for (uint x = 0; x < cameratemp.width; ++x) {
            Ray ray = cameratemp.shoot(x, y);
            uint tri = 0;
            Real t = inter_method->intersect(ray, &tri);
            Color color(0.65, 1, 1);
            if (t >= 0) {
                    nb += 1;
                Real gradient = std::abs(dot(ray.direction, scene.triangles[tri].normal));
                color = Color(gradient, gradient, gradient);
            }//*/
            image.putPixelFloat(x, y, sigmoid(color.r), sigmoid(color.g), sigmoid(color.b));
        }
        if (completion_percent < (int)(((Real)y / 480.0)*100)) {
            completion_percent = ((Real)y / 480.0)*100;
            //std::cout<<completion_percent<<"%\n";
        }
    }
    std::cout<<"detected "<<nb<<" triangles\n";
    std::cout<<"task done in "<<getTimeElapsed(time)<<"\n";
    std::cout<<"end\n";
    image.saveTo("test.png");
    delete inter_method;

    return 0;
}
