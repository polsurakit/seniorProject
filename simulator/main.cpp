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

//var
string INPUT_NAME = "test2.jpg";
string OUTPUT_NAME = "resultLine05mm2degree.jpg";

const int TARGET_H_SIZE = 10000; //0.1 millimeter / pixel
const int TARGET_W_SIZE = 10000; //0.1 millimeter / pixel
myRandom randomGenerator;
const int fieldSize = 15000; //1.5m x 1.5m field
Mat field(fieldSize,fieldSize,CV_8UC3,Vec3b(0,0,0)); //0.1 millimeter / pixel
Mat image;
Mat image2;
Mat showedImage;
Mat expectedImage;
Mat expectedResult;
int TOPLEFTX = 1000;
int TOPLEFTY = 1000;
const int smallWindowSize = 250;
const int bigWindowSize = 500;

myPrinter printer(0,0,0,field);

void showResult(){
    Mat window;
    Mat window2;
    int bigRatio = fieldSize/bigWindowSize;
    resize(printer.cameraImage, window, cv::Size(smallWindowSize, smallWindowSize), 0, 0);
    resize(field, window2, cv::Size(bigWindowSize, bigWindowSize), 0, 0);

    //plot printer
    for(int i = -200 ; i < 200 ; i++){
        for(int j = -200 ; j < 200 ; j++){
            if(i*i+j*j > 100*100) continue;
            window2.at<Vec3b>((printer.y+i)/bigRatio, (printer.x+j)/bigRatio) = Vec3b(255,0,255);
        }
    }
    double theta = printer.theta;
    //plot real line =
  	for(int i = -printer.camera.rowSize/2 ; i < printer.camera.rowSize/2 ; i++){
         int j = -printer.camera.colSize/2;
         int posX = round(cos(theta)*(j) - sin(theta)*(i));
         int posY = round(sin(theta)*(j) + cos(theta)*(i));
         posX = posX + printer.x;
         posY = -posY + printer.y;
         if(posX < 0 || posY < 0 || posX >= field.cols || posY >= field.rows){

         }else{
             window2.at<Vec3b>(posY/bigRatio, posX/bigRatio) = Vec3b(0,0,255);
         }
         j = printer.camera.colSize/2-1;
         posX = round(cos(theta)*(j) - sin(theta)*(i));
         posY = round(sin(theta)*(j) + cos(theta)*(i));
         posX = posX + printer.x;
         posY = -posY + printer.y;
         if(posX < 0 || posY < 0 || posX >= field.cols || posY >= field.rows){

         }else{
             window2.at<Vec3b>(posY/bigRatio, posX/bigRatio) = Vec3b(0,0,255);
         }
      }
    //plot real line ||
    for(int j = -printer.camera.colSize/2 ; j < printer.camera.colSize/2 ; j++){
       int i = -printer.camera.rowSize/2;
       int posX = round(cos(theta)*(j) - sin(theta)*(i));
       int posY = round(sin(theta)*(j) + cos(theta)*(i));
       posX = posX + printer.x;
       posY = -posY + printer.y;
       if(posX < 0 || posY < 0 || posX >= field.cols || posY >= field.rows){

       }else{
           window2.at<Vec3b>(posY/bigRatio, posX/bigRatio) = Vec3b(0,0,255);
       }
       i = printer.camera.rowSize/2-1;
       posX = round(cos(theta)*(j) - sin(theta)*(i));
       posY = round(sin(theta)*(j) + cos(theta)*(i));
       posX = posX + printer.x;
       posY = -posY + printer.y;
       if(posX < 0 || posY < 0 || posX >= field.cols || posY >= field.rows){

       }else{
           window2.at<Vec3b>(posY/bigRatio, posX/bigRatio) = Vec3b(0,0,255);
       }
    }

    vconcat(window,showedImage,window);
    hconcat(window,window2,window);
    hconcat(window,expectedImage,window);
    imshow( "Display window", window);
    waitKey(0);
}

void initialize(){
    image = imread(INPUT_NAME);
    if(! image.data )                              // Check for invalid input
    {
        cout <<  "Could not open or find the image" << std::endl ;
    }
    resize(image,showedImage,Size(smallWindowSize, smallWindowSize),0,0);
    for(int i = TOPLEFTY ; i < fieldSize-TOPLEFTY ; i++){
        for(int j = TOPLEFTX ; j < fieldSize-TOPLEFTX ; j++){
            field.at<Vec3b>(i,j) = Vec3b(255,255,255);
        }
    }
    resize(image,image2,Size(TARGET_H_SIZE, TARGET_W_SIZE),0,0);
    for(int i = TOPLEFTY+1 ; i <= TOPLEFTY+TARGET_H_SIZE ; i++){
        for(int j = TOPLEFTX+1 ; j <= TOPLEFTX+TARGET_W_SIZE ; j++){
            field.at<Vec3b>(i,j) = image2.at<Vec3b>(i-TOPLEFTY-1,j-TOPLEFTX-1);
        }
    }
    resize(field,expectedImage,Size(bigWindowSize,bigWindowSize),0,0);
    field.copyTo(expectedResult);
    for(int i = TOPLEFTY+1 ; i <= TOPLEFTY+TARGET_H_SIZE ; i++){
        for(int j = TOPLEFTX+1 ; j <= TOPLEFTX+TARGET_W_SIZE ; j++){
            field.at<Vec3b>(i,j) = Vec3b(255,255,255);
        }
    }
}

void algorithm(){
    showResult();
    for(int i = 2000 ; i <= 14000 ; i+=2000){
        for(int j = 2000 ; j <= 14000; j+=2000){
            
            printer.move(i, j);
            cout << "move to " << i << " " << j << endl;
            
            //get image and position
            cout << "get data" << endl;
            printer.getCameraImage();
            vector<double> pos = printer.getPosition(); // x, y, theta
            double theta = pos[2];
            
            //show result
            showResult();
            
            //paint
            cout << "paint" << endl;
            for(int ii = -1500; ii < 1500 ; ii++){
                for(int jj = -1500; jj < 1500 ; jj++){
                    int posX = round(cos(theta)*(ii) + sin(theta)*(jj));
                    int posY = round(-sin(theta)*(ii) + cos(theta)*(jj));
                    printer.paint(ii,jj, expectedResult.at<Vec3b>((int)pos[1] + posY, (int)pos[0]+posX), field);
                }
            }
            
            //show result
            printer.getCameraImage();
            showResult();
            cout << "show result" << endl;
        }
    }
}



int main(int argc, char** argv)
{
    initialize();
    printer.getCameraImage();
    algorithm();
    imwrite(OUTPUT_NAME, field);
    imwrite("ex_"+OUTPUT_NAME, expectedResult);
    return 0;
}
