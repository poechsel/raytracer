#ifndef PYTHONTESTSLOADER_H
#define PYTHONTESTSLOADER_H

#include "../includes.h"
#include "pythonfunction.h"
#include "../utilities/camera.h"
#include "../utilities/meshmanagers.h"
#include "../intersections/intersections.h"
#include "../intersections/intersectionnaivemoller.h"
#include "../intersections/intersectiongrid.h"
#include "../intersections/intersectionkdtreespacemedian.h"
#include "../intersections/intersectionkdtreegeometrymedian.h"
#include "../intersections/intersectionkdtreesah.h"
#include "../intersections/intersectionkdtreesahnlogn.h"
#include "pythonsceneloader.h"
#include <map>
#include <string>
#include <iostream>
std::string toString(PyObject* object);

struct Test {
    public:
        Test(){intersection = 0;}
        ~Test(){}
        std::string getJson();
        IntersectionMethod *intersection;
        std::map<std::string, std::string>  parameters;
        std::map<std::string, Real>         render_results;
        std::map<std::string, Real>         build_results;
        std::string                         type;

};

class PythonTestsLoader
{
    public:
        PythonTestsLoader(std::string path);
        std::map<std::string, std::vector<Test> >& load(Scene &scene);
        std::vector<Test>& loadTestForAFile(PyObject *object, Scene &scene);
        Test& loadKdTreeTest(PyObject* object, Scene &scene);
        Test& loadGridTest(PyObject* object, Scene &scene);
        Test& loadNaiveTest(PyObject* object, Scene &scene);
        virtual ~PythonTestsLoader();
    protected:
        std::string     _path;
        PythonModule    _module;
        PythonFunction  _function;
    private:
};

#endif // PYTHONTESTSLOADER_H
