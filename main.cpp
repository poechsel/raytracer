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

using namespace std;
Real sigmoid(Real x) {
    //return 1.0/(1+std::exp(-(x-0.5)*12.0));
    //massal
    double invgamma = 0.45;
    return std::pow(1.0 - std::exp(-0.66 * x), invgamma);
}
Real intersect(Scene *scene, const Ray &ray, uint *t_inter) {
    Real t = -1;
    for (uint i = 0; i < scene->triangles.size(); ++i){
        Real t_temp = scene->triangles[i].intersect(ray);
        if (t_temp >= 0) {
            if (t < 0 || t_temp < t) {
                t = t_temp;
                *t_inter = i;
            }
        }
    }
    return t;
}
int main()
{

    PythonContext context;
    Scene scene;
    context.setCWD("python");

    Manager3dLoader loader3;
//std::map<std::string, Mesh*> meshes3 = loader3.load("scenes/suzanne.obj", &scene);
    //manager.append(meshes3);
    Material mat_cube(Color(1, 0, 0), 0.75, 0.5, 1.33);
    Material mat_plane(Color(0, 0, .5));
    /*manager.meshes["Plane"]->material = &mat_plane;
    manager.meshes["Cube"]->material = &mat_cube;//*/
    /*for (auto it = meshes3.begin(); it != meshes3.end(); ++it) {
        std::cout<<"Mesh: "<<it->first<<" -> "<<&(it->second)<<"\n";
        std::cout<<"    Faces: "<<it->second.triangles.size()<<" Vertices: "<<it->second.vertices.size()<<"\n";
        std::cout<<"    uvs: "<<it->second.tex_coords.size()<<" normals: "<<it->second.normals_vertex.size()<<"\n";
    }//*/


    Matrix4f camera;
    camera = perspective(640, 480, 0.1, 1000, 0.35);
    SFMLImage image (640, 480);
    Matrix4f sphere_transform;
    sphere_transform = translationMatrix<Real>(0.5, -1.5, -7.0);


    //sphere_transform = translationMatrix<Real>(2, 1, 3);
    //IntersectStruct intersection_struct;
    //KdTree<MedianSplit> intersection_struct;
    //sphere->translate(0, -50, -150);
    //sphere->rotate(45, 1, 1, 1);
    //sphere->scale(2, 1, 1);
    //std::cout<<sphere_transform<<"\n";
    Camera cameratemp;
    int completion_percent = 0;


    PythonSceneLoader loader_scenes("scenes/suzanne.json");
    std::cout<<"loading========================\n";
    loader_scenes.load(&cameratemp, &scene);
    std::cout<<"loading========================\n";

    //intersection_struct.loadGeometry(manager);
    std::cout<<"ok\n";
    /*for (auto it = manager.meshes.begin(); it != manager.meshes.end(); ++it) {
        std::cout<<"Mesh: "<<it->first<<" -> "<<&(it->second)<<"\n";
        std::cout<<"    Faces: "<<it->second->triangles.size()<<" Vertices: "<<it->second->vertices.size()<<"\n";
        std::cout<<"    uvs: "<<it->second->tex_coords.size()<<" normals: "<<it->second->normals_vertex.size()<<"\n";
    }*/
    /*for (auto it = manager.meshes.begin(); it != manager.meshes.end(); ++it) {
        it->second->setTransform(sphere_transform);
        it->second->applyTransform();
    }*/
    //intersection_struct.build();
    //raytracer.setIntersectionStructure(&intersection_struct);
    /*cameratemp.fov = 70.0/2.0*3.1416/180.0;
    cameratemp.ratio = 640.0/480.0;
    cameratemp.width = 640.0;
    cameratemp.height = 480.0;//*/
    for (uint y = 0; y < 480; ++y) {
        for (uint x = 0; x < 640; ++x) {
            Ray ray = cameratemp.shoot(x, y);
            uint tri = 0;
            Real t = intersect(&scene, ray, &tri);
            Color color(0.65, 1, 1);
            if (t >= 0) {
                //std::cout<<(tri)<<"\n";
                Real gradient = std::abs(dot(ray.direction, scene.triangles[tri].normal));
                //std::cout<<gradient<<"\n";
                color = Color(gradient, gradient, gradient);
            }

            //std::cout<<color.r<<"\n";
            image.putPixelFloat(x, y, sigmoid(color.r), sigmoid(color.g), sigmoid(color.b));
        }
        if (completion_percent < (int)(((Real)y / 480.0)*100)) {
            completion_percent = ((Real)y / 480.0)*100;
            std::cout<<completion_percent<<"%\n";
        }
    }
    std::cout<<"end\n";
    image.saveTo("test.png");

    return 0;
}
