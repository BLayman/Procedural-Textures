//
// Created by Brett on 4/25/2018.
//

#ifndef PROGRAM3_FETCHCOLOR_H
#define PROGRAM3_FETCHCOLOR_H

#include "Shapes.h"
#include "Texture.h"
using namespace glm;

class FetchColor{
private:
    static float max(float float1,float float2){
        if(float1 > float2){
            return float1;
        }
        else{
            return float2;
        }
    }

public:


    static void getPhongColor(const Triangle* triangle, Ray light, int* colors, vec3 p, Ray camera, vec3 baseColors){
        float ambientStrength = 0.2;
        float diffuseStrength = 1.0;
        float specularStrength = 0.5;
        vec3 lightColor = vec3(1.0, 1.0, 1.0);
        vec3 triangleColor = vec3(triangle->color[0],triangle->color[1],triangle->color[2]);

        // calculate normal
        vec3 p0 = triangle->points[0];
        vec3 p1 = triangle->points[1];
        vec3 p2 = triangle->points[2];
        vec3 p0ToP2 = p2 - p0;
        vec3 p0ToP1 = p1 - p0;
        vec3 normal = normalize(cross(p0ToP2, p0ToP1));

        vec3 ambient = ambientStrength * lightColor;

        vec3 lightDir = normalize(light.origin - p);

        float diff = max(dot(normal, lightDir), 0.0f);

        vec3 diffuse = diffuseStrength * diff * lightColor;

        vec3 viewDir = normalize(camera.origin - p);
        vec3 reflectDir = reflect(-1.0f * lightDir, normal);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
        vec3 specular = diffuse * specularStrength * spec * lightColor;

        vec3 result = (ambient + diffuse + specular) * baseColors;


        colors[0] = glm::min(255,(int)(result.x));
        colors[1] = glm::min(255,(int)(result.y));
        colors[2] = glm::min(255,(int)(result.z));

    }



    static void getBaseColorBary(float u, float v, int *colors){
        float corners[] = {
                1,0,0,
                0,1,0,
                0,0,1,
        };
        float lambdas[] = {
                u, v, 1.0f - u - v
        };

        // set initial rgb values to 0
        for (int i = 0; i < 3; ++i) {
            colors[i] = 0;
        }


        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                colors[i] += (corners[i * 3 + j] * lambdas[i]) * 255;
            }
        }
    }

    static void get1DTextureColor(vec3 p, int *colors, Texture tex){
        float scale = 20;
        float offset = 100;
        float amplitude = 1;
        float xInput = p.x * scale + offset;
        float xVal = tex.sample1DTexture(xInput) * amplitude;

        float yInput = p.y * scale + offset;
        float yVal = tex.sample1DTexture(yInput) * amplitude;

        float zInput = p.z * scale + offset;
        float zVal = tex.sample1DTexture(zInput) * amplitude;

        colors[0] = (int)(xVal * 255);
        colors[1] = (int)(yVal * 255);
        colors[2] = (int)(zVal * 255);

    }

    /* can get same results by using 1 layer in fractal sum method
    static void get2DTextureColor(vec3 p, int *colors, Texture& tex){
        float scale = 40;
        float offset = 100;
        float amplitude = 1;
        float xInput = p.x * scale + offset;
        float yInput = p.y * scale + offset;
        float val = tex.sample2DTexture(xInput, yInput) * amplitude;

        colors[0] = (int)(val * 255);
        colors[1] = (int)(val * 255);
        colors[2] = (int)(val * 255);
    }
     */

    static void get2DFractalSumColor(vec3 p, int *colors, Texture& tex, int layers, bool turbulance, bool wood){
        float baseScale = 2;
        float offset = 100;
        float baseAmp = 2;
        float scaleMultiplier = 2;
        float ampMultiplier = .9;
        colors[0] = 0;
        colors[1] = 0;
        colors[2] = 0;
        float shiftedX = p.x + offset;
        float shiftedY = p.y + offset;

        for (int i = 0; i < layers; ++i) {
            float xInput = shiftedX * baseScale;
            float yInput = shiftedY * baseScale;
            float val;
            if (turbulance){
                val = ((fabs((tex.sample2DTexture(xInput, yInput) - .5f) * 2) * baseAmp)/layers);
            }
            else if(wood){
                float grain = (tex.sample2DTexture(xInput, yInput) * 10);
                val = ((grain - (int) grain)* baseAmp)/layers;
            }
            else{
                val = (tex.sample2DTexture(xInput, yInput) * baseAmp)/layers;
            }

            colors[0] += (int)(val * 255);
            colors[1] += (int)(val * 255);
            colors[2] += (int)(val * 255);
            baseScale *= scaleMultiplier;
            baseAmp *= ampMultiplier;
        }
    }


};

#endif //PROGRAM3_FETCHCOLOR_H
