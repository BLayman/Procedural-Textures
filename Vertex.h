//
// Created by Brett on 3/25/2018.
//

#ifndef LAB6_VERTEX_H
#define LAB6_VERTEX_H

#include <iostream>
#include <vector>


using namespace std;

// to store information about vertices in 3D model
class Vertex{
    vector<float> xyz; // coordinates
    vector<float> normal; // normal
    vector<float> color; // color


public:
    // set default color to red
    Vertex(): color ({0.0f,0.0f,0.0f}){}

    void setColor(float r, float g, float b){
        color[0] = r;
        color[1] = g;
        color[2] = b;
    }

    // set coordinates
    void setXYZ(float x, float y, float z){
        xyz.push_back(x);
        xyz.push_back(y);
        xyz.push_back(z);
    }


    void print(vector<float> arr, int len){
        for (int i = 0; i < len; ++i) {
            cout << arr[i] << " ";
        }
        cout << endl;
    }

    // set normal directly
    void setNormal(vector<float> norm){
        normal = norm;
    }


    // retrive data in vertex array format (coordinates, color, normal)
    vector<float> getData(){
        vector<float> data = xyz;
        data.insert(data.end(), color.begin(), color.end());
        data.insert(data.end(), normal.begin(), normal.end());
        //print(data, 9);
        return data;
    }
};


#endif //LAB6_VERTEX_H
