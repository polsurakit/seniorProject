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
Mat field(10000,10000,CV_8UC3,Vec3b(0,0,0)); //0.1 millimeter / pixel
Mat image;
Mat image2;
Mat showedImage;
Mat expectedImage;
int TOPLEFTX = 100;
int TOPLEFTY = 100;

class myCamera{ // 0.1 mm. per pixel
    //var get 30cm*30cm picture

    //func
    //constructor
public:
    myCamera() {};
    int rowSize = 3000;
    int colSize = 3000;
    Mat getImage(double x, double y, double theta, Mat field){
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
};

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
    const double movePositionError = 5; //5 cm.
    const double moveRotationError = 5; //5 degree
    
    //func
    //constructor
    myPrinter(double cameraX, double cameraY, double cameraZ) :
    cameraX(cameraX), cameraY(cameraY), cameraZ(cameraZ) {
        x = 5000;
        y = 5000;
        theta = 0;
    };
    
    void move(double newX, double newY){
        x = newX+randomGenerator.normal(movePositionError);
        y = newY+randomGenerator.normal(movePositionError);
        theta = randomGenerator.normal(moveRotationError)/180;
        cout << "move to " << x << " " << y << " " << theta << endl;
    }
    
    //xlocal, ylocal is compare to center of printer
    void print(int xlocal, int ylocal, Scalar c, Mat field){
        field.at<Scalar>(ylocal+y,xlocal+x) = c;
    }
    
    void getCameraImage(){
        cameraImage = camera.getImage(x+cameraX, y+cameraY, theta, field);
    }
    
};

myPrinter printer(0,0,0);



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
    image = imread("test2.jpg");
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
