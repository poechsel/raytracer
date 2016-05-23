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
    double invgamma = 0.45;
    return std::pow(1.0 - std::exp(-0.66 * x), invgamma);
}


int raytrace(Camera &cameratemp, Image* image, Scene &scene, IntersectionMethod *inter_method, ULARGE_INTEGER time, int offset = -1){
    int nb = 0;
    for (uint y = 0; y < cameratemp.height; ++y) {
        for (uint x = 0; x < cameratemp.width; ++x) {
            if (offset >= 0 && getTimeElapsed(time) > offset) {
                return -1;
            }
            Ray ray = cameratemp.shoot(x, y);
            uint tri = 0;
            Real t = inter_method->intersect(ray, &tri);
            Color color(0.8, 1, 0.9);
            if (t >= 0) {
                nb += 1;
                Real gradient = std::abs(dot(ray.direction, scene.triangles[tri].normal));
                color = Color(gradient, gradient, gradient);
            }//*/
            image->putPixelFloat(x, y, color.r, color.g, color.b);

        }

    }
    return nb;
}


int main(int argc, char *argv[])
{
    int flags, opt;
    int long_index = 1;
    static struct option long_options[] = {
        {"traversal",   required_argument, 0,   't' },
        {"Ki",          required_argument, 0,   'i' },
        {"Kt",          required_argument, 0,   'p' },
        {"build",       required_argument, 0,   'b' },
        {"complexity",  required_argument, 0,   'c' },
        {"heuristic",   required_argument, 0,   'h' },
        {"method",      required_argument, 0,   'm' },
        {"size",        required_argument, 0,   's' },
        {"output",      required_argument, 0,   'o' },
        {"time",       required_argument, 0,   'l' },
        {0,             0,                 0,   0 }
    };
    std::map<std::string, std::string> params;
    std::string file = (argc>1)? std::string(argv[1]) : "scenes/boudha_05.json";
    params["traversal"] = "rec";
    params["Ki"] = "20";
    params["Kt"] = "15";
    params["build"] = "dfs";
    params["complexity"] = "n2";
    params["heuristic"] = "sah";
    params["method"] = "kdtree";
    params["size"] = "1";
    params["output"] = "sfml";
    params["time"] = "-1";

    while ((opt = getopt_long(argc, argv,"t:i:p:b:c:h:m:s:o:l:", long_options, &long_index )) != -1)
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
             case 'o' : params["output"]        = lower(std::string(optarg));
                 break;
             case 'l' : params["time"]         = lower(std::string(optarg));
                 break;
             default: continue;
                 return -1;
        }
    }


    PythonContext context;
    Scene scene;
    context.setCWD("python");
    int completion_percent = 0;

    Camera cameratemp;
    PythonSceneLoader loader_scenes(file);
    //std::cout<<"loading========================\n";
    loader_scenes.load(&cameratemp, &scene);
    /*for (auto it = scene.meshes.begin(); it != scene.meshes.end(); ++it) {
            std::cout<<"manager->Mesh: "<<it->first<<" -> "<<&(it->second)<<"\n";
            std::cout<<"    Faces: "<<it->second->triangles.size()
                     <<" Vertices: "<<it->second->vertices.size()<<"\n";
    }
    std::cout<<"end loading========================\n";
    //*/

    int time_limit = stringTo<int>(params["time"]);

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


    Image *image = 0;
    if (params["output"] == "sfml")
        image = new SFMLImage(cameratemp.width, cameratemp.height);
    else
        image = new Image(cameratemp.width, cameratemp.height);


    ULARGE_INTEGER time = getTime();
    inter_method->build(time_limit);
    if (inter_method->finished) {
        std::cout<<"build: "<<getTimeElapsed(time)<<"\n";
        time = getTime();
        int nb = raytrace(cameratemp, image, scene, inter_method, time, time_limit);
        Real dt = getTimeElapsed(time);
        std::cout<<"triangles: "<<nb<<"\n";
        if (nb >= 0) {
            std::cout<<"task: "<<dt<<"\n";
        } else {
            std::cout<<"task: "<<"INFTY"<<"\n";
        }
        image->saveTo("test.png");
        std::cout<<"tri/ray: "<<COUNTS::NB_TESTS_TRI_RAY<<"\n";
        std::cout<<"box/ray: "<<COUNTS::NB_TESTS_TRI_VOXEL<<"\n";
    }
    else {
        std::cout<<"build: "<<"INFTY"<<"\n";
    }

    delete inter_method;
    delete image;

    return 0;
}
