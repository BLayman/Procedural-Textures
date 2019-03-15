//
// Created by Brett on 4/24/2018.
//

#ifndef PROGRAM3_TRIANGLE_H
#define PROGRAM3_TRIANGLE_H

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>


using namespace glm;
using namespace std;

struct Sphere {
    int id;
    glm::vec3 center;
    glm::vec3 color;
    float radius;

    Sphere(const glm::vec3& center=glm::vec3(0,0,0),
           float radius=0,
           const glm::vec3& color=glm::vec3(0,0,0))
            : center(center), radius(radius), color(color) {
        static int id_seed = 0;
        id = ++id_seed;
    }
};

class Triangle{
public:
    vector<vec3> points;
    vec3 color;

    Triangle(vector<vec3> pts, vec3 col){
        color = col;
        points = pts;
    }

};

// hold data about direction and origin of ray
struct Ray {
    vec3 origin;
    vec3 dir;

    Ray(vec3 _origin, vec3 _dir){
        origin = _origin;
        dir = _dir;
    }
};

#endif //PROGRAM3_TRIANGLE_H
