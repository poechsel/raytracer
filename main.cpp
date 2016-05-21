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
#include "intersections/intersectionnaivenaive.h"
#include "intersections/intersectiongrid.h"
#include "intersections/intersectionkdtreespacemedian.h"
#include "intersections/intersectionkdtreegeometrymedian.h"
#include "intersections/intersectionkdtreesah.h"
#include "intersections/intersectionkdtreesahnlogn.h"
#include <getopt.h>
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
int main(int argc, char *argv[])
{
    int flags, opt;
    int long_index = 1;
    static struct option long_options[] = {
        {"traversal",   required_argument, 0,   't' },
        {"Ki",          required_argument, 0,   'i' },
        {"Kt",          required_argument, 0,   'p' },
        {"build",       required_argument, 0,   'b' },
        {"complexity",  required_argument, 0,   'c'},
        {"heuristic",   required_argument, 0,   'h' },
        {"method",      required_argument, 0,   'm' },
        {"size",        required_argument, 0,   's' },
        {0,             0,                 0,   0 }
    };
    std::map<std::string, std::string> params;
    std::string file = (argc>1)? std::string(argv[1]) : "scenes/suzanne.json";
    params["traversal"] = "rec";
    params["Ki"] = "20";
    params["Kt"] = "15";
    params["build"] = "dfs";
    params["complexity"] = "nlogn";
    params["heuristic"] = "sah";
    params["method"] = "kdtree";
    params["size"] = "1";
    while ((opt = getopt_long(argc, argv,"t:i:p:b:c:h:m:s:", long_options, &long_index )) != -1)
    {
        switch (opt) {
             case 't' : params["traversal"]     = lower(std::string(optarg));
                 break;
             case 'i' : params["Ki"]            = lower(std::string(optarg));
                 break;
             case 'p' : params["Kt"]            = lower(std::string(optarg));
                 break;
             case 'b' : params["build"]         = lower(std::string(optarg));
                 break;
             case 'c' : params["complexity"]    = lower(std::string(optarg));
                 break;
             case 'h' : params["heuristic"]     = lower(std::string(optarg));
                 break;
             case 'm' : params["method"]        = lower(std::string(optarg));
                 break;
             case 's' : params["size"]          = lower(std::string(optarg));
                 break;
             default: std::cout<<"erreur\n";
                 return -1;
        }
    }

    for (auto &e : params) {
        std::cout<<e.first<<" "<<e.second<<"\n";
    }

    std::cout<<"===> "<<file<<"\n";


    PythonContext context;
    Scene scene;
    context.setCWD("python");
    int completion_percent = 0;

    Camera cameratemp;
    PythonSceneLoader loader_scenes(file);
    std::cout<<"loading========================\n";
    loader_scenes.load(&cameratemp, &scene);
    for (auto it = scene.meshes.begin(); it != scene.meshes.end(); ++it) {
            std::cout<<"manager->Mesh: "<<it->first<<" -> "<<&(it->second)<<"\n";
            std::cout<<"    Faces: "<<it->second->triangles.size()
                     <<" Vertices: "<<it->second->vertices.size()<<"\n";
    }
    std::cout<<"end loading========================\n";

    IntersectionMethod *inter_method = 0;
    if (params["method"] == "kdtree") {
        if (params["heuristic"] == "sah") {
            if (params["complexity"] == "nlogn") {
                inter_method = new IntersectionKdTreeSAHnlogn(&scene,
                                                         stringTo<Real>(params["Ki"]),
                                                         stringTo<Real>(params["Kt"]),
                                                         params["traversal"] == "rec");
            } else if (params["complexity"] == "nlog2n") {
                inter_method = new IntersectionKdTreeSAH(&scene,
                                                         stringTo<Real>(params["Ki"]),
                                                         stringTo<Real>(params["Kt"]),
                                                         params["traversal"] == "rec",
                                                         NLOG2N);
            } else {
                inter_method = new IntersectionKdTreeSAH(&scene,
                                                         stringTo<Real>(params["Ki"]),
                                                         stringTo<Real>(params["Kt"]),
                                                         params["traversal"] == "rec",
                                                         N2);
            }
        } else if (params["heuristic"] == "geometry") {
            inter_method = new IntersectionKdTreeGeometryMedian(&scene,
                                                                params["traversal"] == "rec");
        } else {
            inter_method = new IntersectionKdTreeSpaceMedian(&scene,
                                                            params["traversal"] == "rec");
        }

    } else if (params["method"] == "naive") {
        inter_method = new IntersectionNaiveNaive(&scene);
    } else if (params["method"] == "grid") {
        inter_method = new IntersectionGrid(&scene,
                                            stringTo<Real>(params["size"]),
                                            params["build"] == "dfs");
    } else {
        inter_method = new IntersectionNaiveMoller(&scene);
    }

    //IntersectionMethod *inter_method = new IntersectionNaiveMoller(&scene);
    //IntersectionMethod *inter_method = new IntersectionGrid(&scene, 0.5);
    //IntersectionMethod *inter_method = new IntersectionKdTreeGeometryMedian(&scene, true);
    //IntersectionMethod *inter_method = new IntersectionKdTreeSAH(&scene, 15, 20, NLOG2N);
    //IntersectionMethod *inter_method = new IntersectionKdTreeSAHnlogn(&scene, 15, 20);
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
