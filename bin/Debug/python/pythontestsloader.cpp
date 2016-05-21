#include "pythontestsloader.h"

std::string Test::getJson() {
    std::string json = "\t\t" + this->type + " { \n";
    //parameters:
    std::string json_param = "\t\t\t parameters: {\n";
    for (auto &it : this->parameters) {
        json_param += "\t\t\t\t " + it.first + ": "+ it.second+ ",\n";
    }
    json_param += "\t\t\t},";
    //results build
    std::string json_build = "\t\t\t build: {\n";
    for (auto &it : this->build_results) {
        //json_build += "\t\t\t\t " + it.first + ": "+ std::to_string(it.second)+ ",\n";
    }
    json_build += "\t\t\t},";
    std::string json_intersect = "\t\t\t intersect: {\n";
    for (auto &it : this->render_results) {
        //json_intersect += "\t\t\t\t " + it.first + ": "+ std::to_string(it.second)+ ",\n";
    }
    json_intersect += "\t\t\t},";
    json += json_param + "\n" + json_build + "\n" + json_intersect + "\n\t\t\t},";
    return json_param;
}



PythonTestsLoader::PythonTestsLoader(std::string path):
    _function(&_module), _path(path)
{
    if(!_module.loadModule("loaderOfScene")){
        std::cout<<" _______________________________________________________\n";
        std::cout<<"|  Il n'existe pas de plugins pour charger la scene \n";
        std::cout<<"|_______________________________________________________\n";
    } else if(!_function.loadFunction("load")) {
        std::cout<<" _______________________________________________________\n";
        std::cout<<"|  Le loader doit comprendre une fonction nommée load \n";
        std::cout<<"|_______________________________________________________\n";
    }
}

std::string toString(PyObject* object){
    PyObject*   pyStr = PyUnicode_AsEncodedString(object, "utf-8", "Error ~");
    std::string s = std::string(PyBytes_AS_STRING(pyStr));
    Py_DECREF(pyStr);
    return s;
}


std::vector<Test>& PythonTestsLoader::loadTestForAFile(PyObject* value, Scene &scene){
    std::vector<Test> tests;
    if (PyDict_Check(value)) {
        PyObject* keys = PyDict_Keys(value);
        for (uint i = 0; i < PyList_Size(keys); ++i) {
            PyObject*   key = PyList_GetItem(keys, i);
            PyObject*   content = PyDict_GetItem(value, key);
            std::string type = toString(key);
            if (upper(type) == "KDTREE") {
                tests.push_back(this->loadKdTreeTest(content, scene));
            } else if (upper(type) == "GRID") {
                tests.push_back(loadGridTest(content, scene));
            } else {
                tests.push_back(loadNaiveTest(content, scene));
            }
        }
        Py_DECREF(keys);
    }
    return tests;
}
Test& PythonTestsLoader::loadKdTreeTest(PyObject* value, Scene &scene){
    PyObject *key_complexity = PyUnicode_FromString("complexity");
    std::string complexity = "nlogn";
    if (PyDict_Contains(value, key_complexity)){
        complexity = toString(PyDict_GetItem(value, key_complexity));
    }
    Py_XDECREF(key_complexity);
    PyObject *key_traversal = PyUnicode_FromString("traversal");
    std::string traversal = "rec";
    if (PyDict_Contains(value, key_traversal)){
        traversal = toString(PyDict_GetItem(value, key_traversal));
    }
    Py_XDECREF(key_traversal);
    PyObject *key_heuristic = PyUnicode_FromString("heuristic");
    std::string heuristic = "sah";
    if (PyDict_Contains(value, key_heuristic)){
        heuristic = toString(PyDict_GetItem(value, key_heuristic));
    }
    Py_XDECREF(key_heuristic);
    PyObject *key_ki = PyUnicode_FromString("ki");
    std::string ki = "20";
    if (PyDict_Contains(value, key_ki)){
        ki = toString(PyDict_GetItem(value, key_ki));
    }
    Py_XDECREF(key_ki);
    PyObject *key_kt = PyUnicode_FromString("kt");
    std::string kt = "15";
    if (PyDict_Contains(value, key_kt)){
        kt = toString(PyDict_GetItem(value, key_kt));
    }
    Py_XDECREF(key_ki);


    Test test;
    test.type = "KdTree";
    test.parameters["traversal"] = traversal;
    test.parameters["complexity"] = complexity;
    test.parameters["heuristic"] = heuristic;
    if (upper(heuristic) == "SAH") {
        if (upper(complexity) == "NLOGN") {
            test.intersection = new IntersectionKdTreeSAHnlogn(&scene,
                                                               stringTo<Real>(kt),
                                                               stringTo<Real>(ki));
        }
        else if (upper(complexity) == "NLOG2N") {
            test.intersection = new IntersectionKdTreeSAH(&scene,
                                                            stringTo<Real>(kt),
                                                            stringTo<Real>(ki),
                                                            NLOG2N);
        }
        else {
            test.intersection = new IntersectionKdTreeSAH(&scene,
                                                            stringTo<Real>(kt),
                                                            stringTo<Real>(ki),
                                                            N2);
        }
    } else if (upper(heuristic) == "GEOMETRY") {
        test.intersection = new IntersectionKdTreeGeometryMedian(&scene,
                                                (upper(traversal) == "REC"));
    } else {
        test.intersection = new IntersectionKdTreeSpaceMedian(&scene,
                                                (upper(traversal) == "REC"));
    }
    return test;
}
Test& PythonTestsLoader::loadGridTest(PyObject* value, Scene &scene){
    PyObject *key_size = PyUnicode_FromString("size");
    std::string size = "automatic";
    if (PyDict_Contains(value, key_size)){
        size = toString(PyDict_GetItem(value, key_size));
    }
    Py_XDECREF(key_size);
    PyObject *key_build = PyUnicode_FromString("build");
    std::string build = "dfs";
    if (PyDict_Contains(value, key_build)){
        build = toString(PyDict_GetItem(value, key_build));
    }
    Py_XDECREF(key_build);

    Test test;
    test.type = "Grid";
    test.parameters["build"] = build;
    test.parameters["size"] = size;
    if (upper(size) == "AUTOMATIC") {
        //do nothing as of now
    } else {
        test.intersection = new IntersectionGrid(&scene, stringTo<Real>(size),
                                                 (upper(build) == "DFS"));
    }
    return test;
}
Test& PythonTestsLoader::loadNaiveTest(PyObject* value, Scene &scene){
    PyObject *key_method = PyUnicode_FromString("method");
    std::string method = "naive";
    if (PyDict_Contains(value, key_method)){
        method = toString(PyDict_GetItem(value, key_method));
    }
    Py_XDECREF(key_method);
    Test test;
    test.type = "KdTree";
    test.parameters["method"] = method;
    if (upper(method) == "MOLLER") {
        test.intersection = new IntersectionNaiveMoller(&scene);
    } else {
        //test.intersection = new IntersectionNaiveNaive(&scene);
    }
    return test;
}

std::map<std::string, std::vector<Test> >& PythonTestsLoader::load(Scene &scene){
    PyObject*   ppath = convertToPy(_path);
    PyObject* args = PyTuple_New(1);
    PyTuple_SetItem(args, 0, ppath);
    PyObject* value = _function.call(args);
    Py_DECREF(args);
    bool state = true;
    std::map<std::string, std::vector<Test> > collection;
    if (PyDict_Check(value)) {
        PyObject* keys = PyDict_Keys(value);
        for (uint i = 0; i < PyList_Size(keys); ++i) {
            PyObject*   key = PyList_GetItem(keys, i);
            PyObject*   content = PyDict_GetItem(value, key);
            PyObject*   pyStr = PyUnicode_AsEncodedString(key, "utf-8", "Error ~");
            std::string file = std::string(PyBytes_AS_STRING(pyStr));
            Scene scene;
            collection[file] = this->loadTestForAFile(content, scene);
            Py_DECREF(pyStr);
        }
        Py_DECREF(keys);
    }
    Py_DECREF(value);
    return collection;
}

PythonTestsLoader::~PythonTestsLoader()
{
    //dtor
}
