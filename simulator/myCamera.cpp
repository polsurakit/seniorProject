//
//  myCamera.cpp
//  simulator
//
//  Created by Pol Surakitkoson on 2/18/19.
//  Copyright © 2019 Pol Surakitkoson. All rights reserved.
//

#include "myCamera.hpp"

myCamera::myCamera() {};

Mat myCamera::getImage(double x, double y, double theta, Mat field){
    Mat result(rowSize,colSize,CV_8UC3,Vec3b(0,0,0));
    for(int i = -colSize/2 ; i < colSize/2 ; i++ ){
        for(int j = -rowSize/2 ; j < rowSize/2 ; j++){
            int posX = round(cos(theta)*(i+0.5) - sin(theta)*(j+0.5));
            int posY = round(sin(theta)*(i+0.5) + cos(theta)*(j+0.5));
            posX = posX + x;
            posY = posY + y;
            if(posX < 0 || posY < 0 || posX >= field.cols || posY >= field.rows){
                result.at<Vec3b>(j+1500,i+1500) = Vec3b(0,0,255);
            }else{
                //cout << posY << " " << posX << endl;
                result.at<Vec3b>(j+rowSize/2,i+colSize/2) = field.at<Vec3b>(posY,posX);
            }
        }
    }
    return result;
}
