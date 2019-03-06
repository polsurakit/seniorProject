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
#include <vector>
#include "opencv2/opencv.hpp"
#include "myCamera.hpp"
#include "myRandom.hpp"
using namespace cv;
using namespace std;

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
    const double movePositionError = 50; // *.1 mm.
    const double moveRotationError = 5; // degree
    myRandom randomGenerator;
    
    double readPositionError = 5; // *.1 mm.
    double readRotationError = 0.15; //degree
    //func
    //constructor
    myPrinter(double cameraX, double cameraY, double cameraZ, Mat field) :
    cameraX(cameraX), cameraY(cameraY), cameraZ(cameraZ), field(field) {
        x = 5000;
        y = 5000;
        theta = 0;
    }
    
    vector<double> getPosition(){
        double retx = x+randomGenerator.normal(readPositionError);
        double rety = y+randomGenerator.normal(readPositionError);
        double rettheta = theta+randomGenerator.normal(readRotationError/180*M_PI);
        vector<double> result;
        result.push_back(retx);
        result.push_back(rety);
        result.push_back(rettheta);
        return result;
    }
    
    void move(double newX, double newY){
        x = newX+randomGenerator.normal(movePositionError);
        y = newY+randomGenerator.normal(movePositionError);
        theta = randomGenerator.normal(moveRotationError/180*M_PI);
    }
    
    //add color to field
    //xlocal, ylocal is compare to center of printer
    void paint(int xlocal, int ylocal, Vec3b c, Mat field){
//        if(y+ylocal < 1000 || y+ylocal >= 14000) return;
//        if(x+xlocal < 1000 || x+xlocal >= 14000) return;
//        int posX = round(cos(-theta)*(xlocal) - sin(-theta)*(ylocal));
//        int posY = round(sin(-theta)*(xlocal) + cos(-theta)*(ylocal));
//        if(posX < -1500 || posY < -1500 || posX > 1499 || posY > 1499) return;
//        if(field.at<Vec3b>(ylocal+y,xlocal+x) != Vec3b(255,255,255)) return;
        //ylocal = -ylocal;
        if(xlocal < -1500 || xlocal > 1499 || ylocal < -1500 || ylocal > 1499) return;
        int posX = round(cos(theta)*(xlocal) + sin(theta)*(ylocal));
        int posY = round(-sin(theta)*(xlocal) + cos(theta)*(ylocal));
        if(y+posY < 1000 || y+posY >= 14000) return;
        if(x+posX < 1000 || x+posX >= 14000) return;
        if(field.at<Vec3b>(posY+y,posX+x) != Vec3b(255,255,255) ) return;
        field.at<Vec3b>(posY+y,posX+x) = c;
    }
    
    void getCameraImage(){
        cameraImage = camera.getImage(x+cameraX, y+cameraY, theta, field);
    }
    
};

#endif /* myPrinter_hpp */
