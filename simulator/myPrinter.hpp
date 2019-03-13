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
    bool noError = false;
    double movePositionError = 50; // *.1 mm.
    double moveRotationError = 5; // degree
    myRandom randomGenerator;
    
    double readPositionError = 50; // *.1 mm.
    double readRotationError = 0.1; //degree
    //func
    //constructor
    myPrinter(double cameraX, double cameraY, double cameraZ, Mat field) :
    cameraX(cameraX), cameraY(cameraY), cameraZ(cameraZ), field(field) {
        x = 5000;
        y = 5000;
        theta = 0;
        if(noError){
            movePositionError = 0;
            moveRotationError = 0;
            readPositionError = 0;
            readRotationError = 0;
        }
    }
    
    vector<double> getPosition(){
        double xerror = randomGenerator.normal(readPositionError);
        while (xerror > 2*readPositionError) xerror = randomGenerator.normal(readPositionError);
        double yerror = randomGenerator.normal(readPositionError);
        while (yerror > 2*readPositionError) yerror = randomGenerator.normal(readPositionError);
        double thetaerror = randomGenerator.normal(readRotationError/180*M_PI);
        while (thetaerror > 2*readRotationError/180*M_PI) thetaerror = randomGenerator.normal(readRotationError/180*M_PI);
        double retx = x+xerror;
        double rety = y+yerror;
        double rettheta = theta+thetaerror;
        vector<double> result;
        result.push_back(retx);
        result.push_back(rety);
        result.push_back(rettheta);
        return result;
    }
    
    void move(double newX, double newY){
        double xerror = randomGenerator.normal(movePositionError);
        while (xerror > 2*movePositionError) xerror = randomGenerator.normal(movePositionError);
        double yerror = randomGenerator.normal(movePositionError);
        while (yerror > 2*movePositionError) yerror = randomGenerator.normal(movePositionError);
        double thetaerror = randomGenerator.normal(moveRotationError/180*M_PI);
        while (thetaerror > 2*moveRotationError/180*M_PI) thetaerror = randomGenerator.normal(moveRotationError/180*M_PI);
        x = newX+xerror;
        y = newY+yerror;
        theta = thetaerror;
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
    
    void paint2(int xlocal, int ylocal, Vec3b c, Mat field){
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
        //if(field.at<Vec3b>(posY+y,posX+x) != Vec3b(255,255,255) ) return;
        Vec3b old = field.at<Vec3b>(posY+y,posX+x);
        int b = old.val[0];
        int g = old.val[1];
        int r = old.val[2];
        b = min(255,max(0,b+((int)(c.val[0]))-255));
        g = min(255,max(0,g+((int)(c.val[1]))-255));
        r = min(255,max(0,r+((int)(c.val[2]))-255));
        field.at<Vec3b>(posY+y,posX+x) = Vec3b(b,g,r);
    }
    
    void getCameraImage(){
        cameraImage = camera.getImage(x+cameraX, y+cameraY, theta, field);
    }
    
};

#endif /* myPrinter_hpp */
