#include "OBJLoader.h"



/*
OBJLoader::OBJLoader(std::string path): _offset(1), _current_mesh(0), _path(path)
{
    std::string line;
    std::ifstream file(path);
    while (std::getline(file, line)){
        std::vector<std::string> temp = split(line, '\r');
        for (uint i = 0; i < temp.size(); ++i) {
            _content.push_back(trimLeft(trimRight(temp[i])));
            std::replace(_content[_content.size()-1].begin(), _content[_content.size()-1].begin(), '\t', ' ');
        }
    }

}

std::map<std::string, Mesh> OBJLoader::load() {
    _output.clear();
    for (uint i = 0; i < _content.size(); i++) {
        //processline
        std::string current = _content[i];
        std::string command = firstWord(current);
        if (command == "o") {
            processObject(trimRight(current.substr(command.size())));
        } else if (command == "v") {
            processVertex(trimRight(current.substr(command.size())));
        } else if (command == "f") {
            processFace(trimRight(current.substr(command.size())));
        }
    }
    return _output;
}

void OBJLoader::processFace(std::string line) {
    std::vector<std::string> vertices = split (line, ' ');
    if (vertices.size() < 3) {
        std::cout<<"[ERREUR] Lors du chargement de "<<_path<<":\n\tUne face doit au moins avoir 3 vertex\n";
        std::exit(0);
    }
    std::vector<int> vertices_index;
    for (uint i = 0; i < vertices.size(); ++i) {
        std::vector<std::string> temp = split (vertices[i], '/');
        vertices_index.push_back(stringTo<int>(temp[0]));
    }
        //std::cout<<"-> "<<line<<"\n";
    for (uint i = 1; i < vertices_index.size() - 1; ++i) {
        int v1 = (vertices_index[0] > 0)? vertices_index[0] - _offset : _current_mesh->vertices.size() + vertices_index[0];
        int v2 = (vertices_index[i] > 0)? vertices_index[i] - _offset : _current_mesh->vertices.size() + vertices_index[i];
        int v3 = (vertices_index[i+1] > 0)? vertices_index[i+1] - _offset : _current_mesh->vertices.size() + vertices_index[i+1];
        if (!_current_mesh)  {
            this->processObject("");
        }
       // std::cout<<v1<<" "<<v2<<" "<<v3<<"\n";
        _current_mesh->addFace(v1, v2, v3);
    }
}
void OBJLoader::processVertex(std::string line) {
    std::vector<std::string> numbers = split(line, ' ');
    if (numbers.size() != 3) {
        std::cout<<"[ERREUR] Lors du chargement de "<<_path<<":\n\tUn vertex n'avait pas 3 coordonnées\n";
        std::exit(0);
    }
    if (!_current_mesh)  {
        this->processObject("");
    }
    //std::cout<<stringTo<Real>(numbers[0])<<" "<<stringTo<Real>(numbers[1])<<" "<<stringTo<Real>(numbers[2])<<"\n";
    _current_mesh->addVertex(stringTo<Real>(numbers[0]), stringTo<Real>(numbers[1]), stringTo<Real>(numbers[2]));
}
void OBJLoader::processObject(std::string line) {
    _output [line] = Mesh();
    if (_current_mesh)
        _offset += _current_mesh->vertices.size();
    _current_mesh = &(_output[line]);
}

OBJLoader::~OBJLoader()
{
    //dtor
}
//*/
