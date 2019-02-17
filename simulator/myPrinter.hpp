//
//  myPrinter.hpp
//  simulator
//
//  Created by Pol Surakitkoson on 2/18/19.
//  Copyright © 2019 Pol Surakitkoson. All rights reserved.
//

#ifndef myPrinter_hpp
#define myPrinter_hpp

#include <stdio.h>
#include "opencv2/opencv.hpp"
#include "myCamera.hpp"
#include "myRandom.hpp"
using namespace cv;

class myPrinter{
public:
    
    //var
    myCamera camera;
    double x;
    double y;
    double theta;
    double cameraX; //camera position compare to center of printer position
    double cameraY;
    double cameraZ;
    Mat cameraImage;
    Mat field;
    const double movePositionError = 5; //5 cm.
    const double moveRotationError = 5; //5 degree
    myRandom randomGenerator;
    
    //func
    //constructor
    myPrinter(double cameraX, double cameraY, double cameraZ, Mat field) :
    cameraX(cameraX), cameraY(cameraY), cameraZ(cameraZ), field(field) {
        x = 5000;
        y = 5000;
        theta = 0;
    };
    
    void move(double newX, double newY){
        x = newX+randomGenerator.normal(movePositionError);
        y = newY+randomGenerator.normal(movePositionError);
        theta = randomGenerator.normal(moveRotationError)/180;
        //cout << "move to " << x << " " << y << " " << theta << endl;
    }
    
    //xlocal, ylocal is compare to center of printer
    void print(int xlocal, int ylocal, Scalar c, Mat field){
        field.at<Scalar>(ylocal+y,xlocal+x) = c;
    }
    
    void getCameraImage(){
        cameraImage = camera.getImage(x+cameraX, y+cameraY, theta, field);
    }
    
};

#endif /* myPrinter_hpp */
