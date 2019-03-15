#include <iostream>
#define readObj
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
//#include <Dense>
#include <cmath>

#include "bitmap_image.hpp"
#include "ObjReader.h"
#include "Shapes.h"
#include "Texture.h"
#include "FetchColor.h"

using namespace glm;
using namespace std;
const int HEIGHT = 768;
const int WIDTH = 1024;

struct Viewport {
    glm::vec2 min;
    glm::vec2 max;

    Viewport(const glm::vec2& min, const glm::vec2& max)
        : min(min), max(max) {}
};




// return if there is an intersection and assign distTo
bool intersectSphere(Ray ray, Sphere sphere, double &distTo){
    // vector from ray origin to sphere center
    vec3 originToCenter = sphere.center - ray.origin;
    // distance from ray origin to point along ray closest to center of circle (call it x)
    double oToX = glm::dot(originToCenter, ray.dir);
    // from center of circle to x, squared
    double cToXSq = glm::dot(originToCenter, originToCenter) - oToX*oToX;
    // if distance from center to x is greater than the radius, the ray misses the sphere
    if(cToXSq > sphere.radius * sphere.radius){
        return false; // so return false
    }
    // distance from intersection point to x
    double pToX = sqrt(sphere.radius * sphere.radius - cToXSq);

    // two possible distances from origin to intersection
    double t1 = oToX - pToX;
    double t2 = oToX + pToX;
    // take smallest distance and assign to distTo
    if(t1 < t2){
        distTo = t1;
    }
    else{
        distTo = t2;
    }
    return true; // does intersectSphere

}

/*
 * // Too slow
bool intersectTriangle(Ray ray, Triangle triangle, double &distTo){

    Eigen::Matrix3f A;
    Eigen::Vector3f b;

    A << triangle.points[1].x - triangle.points[0].x , triangle.points[2].x - triangle.points[0].x, -ray.dir.x,
            triangle.points[1].y - triangle.points[0].y , triangle.points[2].y - triangle.points[0].y, -ray.dir.y,
            triangle.points[1].z - triangle.points[0].z , triangle.points[2].z - triangle.points[0].z, -ray.dir.z,

    b << ray.origin.x - triangle.points[0].x, ray.origin.y - triangle.points[0].y, ray.origin.z - triangle.points[0].z;

    Eigen::Vector3f c = A.colPivHouseholderQr().solve(b);

    if(c.y() < 0 || c.y() > 1){
        return false;
    }
    else if(c.x() < 0 || c.x() > (1 - c.y())){
        return false;
    }
    else if(c.z() < 0){
        return false;
    }
    else{
        return true;
    }
}
*/

bool intersectTriangle(Ray ray, Triangle triangle, double &distTo, float& u, float& v){
    bool intersected;

    vec3 p0 = triangle.points[0];
    vec3 p1 = triangle.points[1];
    vec3 p2 = triangle.points[2];

    vec3 p0ToP1 = p1 - p0;
    vec3 p0ToP2 = p2 - p0;

    vec3 vecCross = glm::cross(ray.dir, p0ToP2);
    float determinant = dot(p0ToP1, vecCross);

    // ray and triangle are parallel if det is close to 0
    if (determinant < .0001){
        intersected = false;
    }

    float inverseDet = 1 / determinant;

    vec3 p0ToOrigin = ray.origin - p0;
    vec3 vecCross2 = cross(p0ToOrigin,p0ToP1);

    u = dot(p0ToOrigin,vecCross) * inverseDet;
    v = dot(ray.dir, vecCross2)* inverseDet;
    float t = dot(p0ToP2, vecCross2) * inverseDet;

    if(u < 0 || u > 1) intersected = false;
    else if(v < 0 || u > 1 - v) intersected = false;
    else if(t < 0) intersected = false;
    else{
        distTo = t;
        intersected = true;

    }

    return intersected;

}




// render world using ray tracing
void render(bitmap_image& image, const std::vector<Triangle>& world, bool usePerpective, Ray light) {

    // screen size
    double nx = WIDTH;
    double ny = HEIGHT;

#ifdef readObj
    double boxWidth = 2;
#else
    double boxWidth = 4;
#endif
    // 10 x 10 box with (0,0) center
    double l = -boxWidth/2;
    double r = boxWidth/2;
    double b = -boxWidth/2;
    double t = boxWidth/2;
    Texture tex;
    //tex.generate1DNoise(1231);
    tex.generate2DNoise(1231);

    // for each pixel
    for (int i = 0; i < nx; ++i) {
        for ( int j = 0; j < ny; ++j) {
            // get ray origin coordinates
            double ui = l + (r-l)*(i+.5) / nx;
            double vj = b + (t-b)*(j+.5) / ny;
            // pixel color
            rgb_t color;

            vec3 rayOrigin;
            vec3 dir;
            vec3 p;
            vec3 viewportCoord = vec3(ui,vj,-5);

            // if using perspective
            if(usePerpective){
                float d = 10;
                // ray origin
                rayOrigin = vec3(0,0,-d);

                // ray direction
                dir = normalize(viewportCoord - rayOrigin);

            }

            // orthographic
            else{
                // ray origin
                rayOrigin = viewportCoord;
                // ray direction
                dir = vec3(0,0,1);
            }

            // make ray
            Ray ray = Ray(rayOrigin,dir);

            float minDist = 1000; // distance to closest sphere
            const Triangle* toRender;
            float u;
            float v;
            float tempU;
            float tempV;



            // check for intersection with each triangle in the world
            for (auto& triangle : world) {

                // distance to sphere
                double distTo;


                if(intersectTriangle(ray, triangle, distTo, tempU, tempV)){

                    // set new minDist if closest
                    if (distTo < minDist){
                        u = tempU;
                        v = tempV;
                        minDist = distTo;
                        //Triangle renderMe = triangle;
                        toRender = &triangle;
                        p = rayOrigin + dir * minDist;


                    }

                }
            }

            // don't render anything farther than 1000 away
            if(minDist < 1000) {
                //cout << toRender->color.x << endl;
                int baseColors[3];
                int colors[3];
                //getBaseColorBary(u, v, baseColors);
                //FetchColor::get2DTextureColor(p,baseColors, tex);
                FetchColor::get2DFractalSumColor(p,baseColors, tex, 10, false, true);

                vec3 baseColorsVec(baseColors[0],baseColors[1],baseColors[2]);
                FetchColor::getPhongColor(toRender, light, colors, p, ray, baseColorsVec);
                // get color from closest shape
                //double red = toRender->color.x * 255;
                //double green = toRender->color.y * 255;
                //double blue = toRender->color.z * 255;
                //cout << "color set to: " << red << green << blue << endl;
                //cout << i << " " << j << endl;
                // set color

                if(colors[0] > 255){
                    cout << "result: " << colors[0] << " " << colors[1] << " " << colors[2] << " " << endl;
                }

                color = make_colour(colors[0], colors[1], colors[2]);

            }
            else{
                // default color
                color = make_colour(10, 100, 75);
            }
            // flip over x axis
            int invertedJ = HEIGHT - j - 1;
            // set pixel to color
            image.set_pixel(i,invertedJ,color);

        }
    }

}




int main(int argc, char** argv) {



    // create an image 640 pixels wide by 480 pixels tall
    bitmap_image image(WIDTH, HEIGHT);
#ifndef readObj
    vector<vec3> points1 = {glm::vec3(0, 0, 2),glm::vec3(2, 0, 2),glm::vec3(0, 2, 2)};
    vector<vec3> points2 = {glm::vec3(1, 1, .5),glm::vec3(0, 1, .5),glm::vec3(1, 0, 5)};
    vector<vec3> points3 = {glm::vec3(.5, .5, 2.5),glm::vec3(-.5, .5, 2.5),glm::vec3(.5, -.5, 0)};

    // build world
    std::vector<Triangle> world = {
        Triangle(points1, glm::vec3(1,1,0)),
        Triangle(points3, glm::vec3(1,0,0)),
        Triangle(points2, glm::vec3(0,0,1)),

    };
#else
    ObjReader::r = 1;
    string charFile = "../models/box.obj";
    std::vector<Triangle> world = ObjReader::readInObj(false,charFile);
# endif

    Ray light(vec3(0,0, 0),vec3(0,0,-10));

    // render orthographic view
    render(image,world, false, light);
    image.save_image("../orthographic.bmp");

    // render perspective view
    render(image, world, true, light);
    image.save_image("../perspective.bmp");

    std::cout << "Success" << std::endl;
}


