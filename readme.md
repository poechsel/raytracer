# Raytracer

Small raytracer showcasing various acceleration methods on a single core processor for ray-triangle intersections tests
It showcase :
- grid structure 
- kdtree with various build heuristics
    - Median space split
    - Median geometry split
    - SAH (build either by the normal way, or by Igo Wald O(nlogn) algorithm)

## Compile :
You must have sfml2 and python3-dev installed. You might change the link to python's header files in the makefile
To build:
```
make
```

## Use :
To use it, you must provide a json "scene file". Its structure will be explained below. If you need, a blender addon can generate it for you (the addon is python/blenderexporter.py). If the scene name is `scene.json`, you can launch the raytracer with :
```
./raytracer scene.json
```

For more complexe use, you can provide arguments after the scene file. These arguments can be :
- `-t` to choose a traversal method for kdtree exploration. `rec` means it is done recursively, otherwise it is iterative
- `-i` to determine the cost Ki of a ray / triangle intersection test
- `-p` for an approximation of the cost Kt of traversal
- `-c` can be use when the selected heuristic is `sah` and the acceleration structure is a kdtree. It can have several values : `nlogn`, `nlog2n`, `nlog2nc` or `n2`. It determines the variant of the heuristic we use to build the structure (`nlogn` refers to Igo Wald methods, `nlog2n` to the classic one, `nlog2nc` is the classic method but with a better sort in theory and `n2` the trivial method)
- `-h` for the build heuristic when using a kdtree. Either `geometry`, (splits on the middle of the geometry) `spatial` (split on the middle of the space) or `sah` (split taking into account both the geometry and the space)
- `-m` the acceleration structure used (`kdtree` for a kdtree, `grid` for a grid, `moller` for the naive method using moller's intersection test and `naive` for a very naive method with a non efficient intersection test)
- `-s` to determinate the dimension of the grid
- `-t` to put a limit on the execution time (by default 120)

Examples :
```
./raytracer scene.json -t 90 -i 30 -p 5 -c nlogn -h sah -m kdtree
```
It will render the scene using a kdtree build with sah nlogn algorithm, taking Ki=30 and Kt = 5. The time limit is set to 90

By default, `./raytracer scene.json` acts the same way has :
```
./raytracer scene.json -t rec -i 20 -p 15 -b seq -c nlogn -h sah -m kdtree -s 10 -o sfml -t 120
```


## scene file format
A scene is a json file under the form :
```json
{
    "SCENE_PRESETS": {
        "height": "height of the image",
        "width": "width of the image"
    },
    "OBJECTS": {
        "path": "path to the file containing the geometry (by default in obj)"
    },
    "CAMERA": {
        "fov": "fov of the camera",
        "ratio": "ratio of the camera",
        "matrix": "4x4 matrix representing the position/rotation/scale of the camera"
    }
}
```
