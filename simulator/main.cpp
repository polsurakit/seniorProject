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
#include "myCamera.hpp"
#include "myPrinter.hpp"
#include "myRandom.hpp"



struct color{
    int r,g,b;
};

//var
myRandom randomGenerator;
Mat field(10000,10000,CV_8UC3,Vec3b(0,0,0)); //0.1 millimeter / pixel
Mat image;
Mat image2;
Mat showedImage;
Mat expectedImage;
int TOPLEFTX = 100;
int TOPLEFTY = 100;



myPrinter printer(0,0,0,field);



void showResult(){
    printer.getCameraImage();
    Mat res;
    Mat res2;
    Mat res3;
    Mat res4;
    vector<Vec3b> v(250000);
    for(int i = -200 ; i < 300 ; i++){
        for(int j = -200 ; j < 300 ; j++){
            if(i*i+j*j > 100*100) continue;
            v[(i+200)*500+j+200] = field.at<Vec3b>(printer.y+i,printer.x+j);
            field.at<Vec3b>(printer.y+i,printer.x+j) = Vec3b(255,0,255);
        }
    }
    vector<Vec3b> v2;
    for(int i = -printer.camera.rowSize/2 ; i < printer.camera.rowSize/2 ; i++){
        int j = printer.camera.colSize/2;
        int posX = round(cos(printer.theta)*(i+0.5) - sin(printer.theta)*(j+0.5));
        int posY = round(sin(printer.theta)*(i+0.5) + cos(printer.theta)*(j+0.5));
        posX = posX + printer.x;
        posY = posY + printer.y;
        if(posX < 0 || posY < 0 || posX >= field.cols || posY >= field.rows){
            
        }else{
            //cout << posY << " " << posX << endl;
            v2.push_back(field.at<Vec3b>(j+printer.camera.rowSize/2,i+printer.camera.colSize/2));
            field.at<Vec3b>(j+printer.camera.rowSize/2,i+printer.camera.colSize/2) = Vec3b(0,0,255);
        }
        j = printer.camera.colSize/2-1;
        posX = round(cos(printer.theta)*(i+0.5) - sin(printer.theta)*(j+0.5));
        posY = round(sin(printer.theta)*(i+0.5) + cos(printer.theta)*(j+0.5));
        posX = posX + printer.x;
        posY = posY + printer.y;
        if(posX < 0 || posY < 0 || posX >= field.cols || posY >= field.rows){
            
        }else{
            //cout << posY << " " << posX << endl;
            v2.push_back(field.at<Vec3b>(j+printer.camera.rowSize/2,i+printer.camera.colSize/2));
            field.at<Vec3b>(j+printer.camera.rowSize/2,i+printer.camera.colSize/2) = Vec3b(0,0,255);
        }
        
    }
    
    resize(printer.cameraImage, res, cv::Size(250,250), 0, 0);
    resize(field, res2, cv::Size(500,500), 0, 0);
    
    vconcat(res,showedImage,res);
    hconcat(res,res2,res);
    hconcat(res,expectedImage,res);
    imshow( "Display window", res);
    for(int i = -200 ; i < 300 ; i++){
        for(int j = -200 ; j < 300 ; j++){
            field.at<Vec3b>(printer.y,printer.x) = v[(i+200)*500+j+200];
        }
    }
    int idx = 0;
    for(int i = -printer.camera.rowSize/2 ; i < printer.camera.rowSize/2 ; i++){
        int j = printer.camera.colSize/2;
        int posX = round(cos(printer.theta)*(i+0.5) - sin(printer.theta)*(j+0.5));
        int posY = round(sin(printer.theta)*(i+0.5) + cos(printer.theta)*(j+0.5));
        posX = posX + printer.x;
        posY = posY + printer.y;
        if(posX < 0 || posY < 0 || posX >= field.cols || posY >= field.rows){
            
        }else{
            //cout << posY << " " << posX << endl;
            //v2.push_back(field.at<Vec3b>(j+printer.camera.rowSize/2,i+printer.camera.colSize/2));
            field.at<Vec3b>(j+printer.camera.rowSize/2,i+printer.camera.colSize/2) = v2[idx];
            idx++;
        }
        j = printer.camera.colSize/2-1;
        posX = round(cos(printer.theta)*(i+0.5) - sin(printer.theta)*(j+0.5));
        posY = round(sin(printer.theta)*(i+0.5) + cos(printer.theta)*(j+0.5));
        posX = posX + printer.x;
        posY = posY + printer.y;
        if(posX < 0 || posY < 0 || posX >= field.cols || posY >= field.rows){
            
        }else{
            //cout << posY << " " << posX << endl;
            //v2.push_back(field.at<Vec3b>(j+printer.camera.rowSize/2,i+printer.camera.colSize/2));
            field.at<Vec3b>(j+printer.camera.rowSize/2,i+printer.camera.colSize/2) = v2[idx];
            idx++;
        }
        
    }
    waitKey(0);
}

void initialize(){
    image = imread("test.jpg");
    if(! image.data )                              // Check for invalid input
    {
        cout <<  "Could not open or find the image" << std::endl ;
    }
    resize(image,showedImage,Size(250,250),0,0);
    
    for(int i = TOPLEFTY ; i <= 10000-TOPLEFTY ; i++){
        for(int j = TOPLEFTX ; j <= 10000-TOPLEFTX ; j++){
            field.at<Vec3b>(i,j) = Vec3b(255,255,255);
        }
    }
    resize(image,image2,Size(7000,7000),0,0);
    
    for(int i = TOPLEFTY+1 ; i <= TOPLEFTY+7000 ; i++){
        for(int j = TOPLEFTX+1 ; j <= TOPLEFTX+7000 ; j++){
            field.at<Vec3b>(i,j) = image2.at<Vec3b>(i-TOPLEFTY-1,j-TOPLEFTX-1);
            //cout << field.at<Vec3b>(i,j) << " ";
        }
    }
    
    resize(field,expectedImage,Size(500,500),0,0);
    for(int i = TOPLEFTY+1 ; i <= TOPLEFTY+7000 ; i++){
        for(int j = TOPLEFTX+1 ; j <= TOPLEFTX+7000 ; j++){
            field.at<Vec3b>(i,j) = Vec3b(255,255,255);
            //cout << field.at<Vec3b>(i,j) << " ";
        }
    }

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
