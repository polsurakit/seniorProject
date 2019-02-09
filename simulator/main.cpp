//
//  main.cpp
//  printer
//
//  Created by Pol Surakitkoson on 12/27/18.
//  Copyright © 2018 Pol Surakitkoson. All rights reserved.
//

#include "opencv2/opencv.hpp"
using namespace cv;
#include <iostream>
using namespace std;

class myRandom{
    cv::RNG rng;
    public:
    myRandom(){
        cv::RNG rng(cv::getCPUTickCount());
    }

    double normal(double sigma){
        return rng.gaussian(sigma);
    }
};

struct color{
    int r,g,b;
};

//var
myRandom randomGenerator;
Mat field;

class myCamera{
    //var

    //func
    //constructor
public:
    myCamera() {};
    
    Mat getImage(double x, double y, double theta, Mat field){
        Mat result;
        return result;
    }
};

class myPrinter{
    //var
    myCamera camera;
    double x;
    double y;
    double theta;
    double cameraX; //camera position compare to center of printer position
    double cameraY;
    double cameraZ;
    
    const double movePositionError = 5; //5 cm.
    const double moveRotationError = 5; //5 degree
    
    //func
    //constructor
    myPrinter(double cameraX, double cameraY, double cameraZ) :
    cameraX(cameraX), cameraY(cameraY), cameraZ(cameraZ) {};
    
    void move(double newX, double newY){
        x = newX+randomGenerator.normal(movePositionError);
        y = newY+randomGenerator.normal(movePositionError);
        theta = randomGenerator.normal(moveRotationError);
    }
    
    //xlocal, ylocal is compare to center of printer
    void print(double xlocal, double ylocal, color c, Mat field){
        
    }
    //print(xlocal,ylocal,color,field)
    
};

void showResult(){
    
}

void initialize(){
    
}

void algorithm(){
    
    showResult();
}

int main(int argc, char** argv)
{
    initialize();
    algorithm();
    return 0;
}
