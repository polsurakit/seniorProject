//
//  main.cpp
//  printer
//
//  Created by Pol Surakitkoson on 12/27/18.
//  Copyright © 2018 Pol Surakitkoson. All rights reserved.
//

#include "opencv2/opencv.hpp"
#include "opencv2/imgproc/imgproc.hpp"
using namespace cv;
#include <iostream>
using namespace std;
#include "myCamera.hpp"
#include "myPrinter.hpp"
#include "myRandom.hpp"
#include <map>

//var
string INPUT_NAME = "test.jpg";
string OUTPUT_NAME = "algor5_new_2.jpg";
bool isBlur = false;

const int TARGET_H_SIZE = 10000; //0.1 millimeter / pixel
const int TARGET_W_SIZE = 10000; //0.1 millimeter / pixel
myRandom randomGenerator;
const int fieldSize = 15000; //1.5m x 1.5m field
Mat field(fieldSize,fieldSize,CV_8UC3,Vec3b(0,0,0)); //0.1 millimeter / pixel
//int zone[fieldSize][fieldSize];
//int zone2[fieldSize][fieldSize];
vector<vector<int> > zone;
vector<vector<int> > zone2;
vector<vector<vector<double> > > zone3;

//Mat zone(fieldSize,fieldSize,CV_8UC1,0);
Mat image;
Mat image2;
Mat showedImage;
Mat expectedImage;
Mat expectedResult;
int TOPLEFTX = 1000;
int TOPLEFTY = 1000;
const int smallWindowSize = 250;
const int bigWindowSize = 500;
const int moveStep = 2000;

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
    cout << 1 ;
    resize(image,showedImage,Size(smallWindowSize, smallWindowSize),0,0);
    for(int i = TOPLEFTY ; i < fieldSize-TOPLEFTY ; i++){
        for(int j = TOPLEFTX ; j < fieldSize-TOPLEFTX ; j++){
            field.at<Vec3b>(i,j) = Vec3b(255,255,255);
        }
    }
    cout << 1 ;
    resize(image,image2,Size(TARGET_H_SIZE, TARGET_W_SIZE),0,0);
    //blur
    if(isBlur){
        blur(image2,image2,Size(50,50));
    }
    for(int i = TOPLEFTY+1 ; i <= TOPLEFTY+TARGET_H_SIZE ; i++){
        for(int j = TOPLEFTX+1 ; j <= TOPLEFTX+TARGET_W_SIZE ; j++){
            field.at<Vec3b>(i,j) = image2.at<Vec3b>(i-TOPLEFTY-1,j-TOPLEFTX-1);
        }
    }
    cout << 1 ;
    resize(field,expectedImage,Size(bigWindowSize,bigWindowSize),0,0);
    field.copyTo(expectedResult);
    for(int i = TOPLEFTY+1 ; i <= TOPLEFTY+TARGET_H_SIZE ; i++){
        for(int j = TOPLEFTX+1 ; j <= TOPLEFTX+TARGET_W_SIZE ; j++){
            field.at<Vec3b>(i,j) = Vec3b(255,255,255);
        }
    }
    cout << "end init" << endl;
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

void fillzone(){
    cout << "fillzone" << endl;
    zone.resize(fieldSize,vector<int>(fieldSize));
    zone2.resize(fieldSize,vector<int>(fieldSize));
    for(int i = 1500 ; i < 14000 ; i+=2000){
        for(int j = 1500 ; j < 14000 ; j+=2000){
            for(int k = -1300; k < 1300 ; k++){
                for(int l = -1300 ; l < 1300 ; l++){
                    zone[i+k][j+l] += 1;
                    
                }
            }
            if(TARGET_W_SIZE+1000 <= j + 1300) {cout << j << endl;break;}
        }
        if(TARGET_H_SIZE+1000 <= i + 1300) {cout << i << endl;break;}
    }
}

void fillzone2(){
    cout << "fillzone2" << endl;
    zone.resize(fieldSize,vector<int>(fieldSize));
    zone2.resize(fieldSize,vector<int>(fieldSize));
    zone3.resize(fieldSize,vector<vector<double> >(fieldSize));
    for(int i = 1500 ; i < 14000 ; i+=2000){
        for(int j = 1500 ; j < 14000 ; j+=2000){
            for(int k = -1500; k < 1500 ; k++){
                for(int l = -1500 ; l < 1500 ; l++){
                    zone[i+k][j+l] += 1;
                    
                }
            }
            if(TARGET_W_SIZE+1000 <= j + 1300) {break;}
        }
        if(TARGET_H_SIZE+1000 <= i + 1300) {break;}
    }
    int a = 1000;
    int b = 2000;
    cout << "fillcol" << endl;
    for(int j = b ; j < 14000 ; j+= 2000){
        for(int i = 1000 ; i < 14000 ; i++){
            if(zone[i][j]>2){
                i+=a-1;
                continue;
            }
            if(zone[i][j]==0) break;
            for(int k = 0 ; k < a ; k++){
                double w1 = 1./(k+1);
                double w2 = 1./(a-k);
                zone3[i][j+k].push_back(w2/(w1+w2));
                zone3[i][j+k].push_back(w1/(w1+w2));
                if(k==0) {
//                    cout << i << " " << j << " " << w1 << " " << w2 << " " << w2/(w1+w2) << " " << w1/(w1+w2) << endl;
//                    cout << (50+(int)((255-50)*w2/(w1+w2))) << " " << (50+((int)((255-50)*w2/(w1+w2)))) << endl;
                }
                //cout << zone3[i][j+k][0] << " " << zone3[i][j+k][1] << endl;
            }
        }
    }
    cout << "fillrow" << endl;
    for(int i = b ; i < 14000 ; i+= 2000){
        for(int j = 1000 ; j < 14000 ; j++){
            if(zone[i][j]>2){
                j+=a-1;
                continue;
            }
            if(zone[i][j]==0) break;
            
            for(int k = 0 ; k < a ; k++){
                if(i+k==2214 && j==1000) cout << "asdfsafsdaf" << endl;
                double w1 = 1./(k+1);
                double w2 = 1./(a-k);
                zone3[i+k][j].push_back(w2/(w1+w2));
                zone3[i+k][j].push_back(w1/(w1+w2));
                //cout << w1 << " " << w2 << " " << zone3[i+k][j][0] << " " << zone3[i+k][j][1] << endl;
            }
        }
    }
    cout << "fill4" << endl;
    for(int i = b ; i < 14000 ; i+=2000){
        for(int j = b ; j < 14000 ; j+=2000){
            for(int k = 0 ; k < a ; k++){
                for(int l = 0 ; l < a ; l++){
                    if(zone[i+k][j+l]==0) break;
                    double w1 = 1./sqrt((k+1)*(k+1)+(l+1)*(l+1));
                    double w2 = 1./sqrt((k+1)*(k+1)+(a-l)*(a-l));
                    double w3 = 1./sqrt((a-k)*(a-k)+(l+1)*(l+1));
                    double w4 = 1./sqrt((a-k)*(a-k)+(a-l)*(a-l));
                    double w = w1+w2+w3+w4;
                    zone3[i+k][j+l].push_back(1-w1/w);
                    zone3[i+k][j+l].push_back(1-w2/w);
                    zone3[i+k][j+l].push_back(1-w3/w);
                    zone3[i+k][j+l].push_back(1-w4/w);
                }
                if(zone[i+k][j]==0) break;
            }
        }
    }
    cout << "end fillzone2" << endl;
}

void fillzone3(){
    cout << "fillzone3" << endl;
    zone.resize(fieldSize,vector<int>(fieldSize));
    zone2.resize(fieldSize,vector<int>(fieldSize));
    zone3.resize(fieldSize,vector<vector<double> >(fieldSize));
    for(int i = 1500 ; i < 14000 ; i+=2000){
        for(int j = 1500 ; j < 14000 ; j+=2000){
            for(int k = -1500; k < 1500 ; k++){
                for(int l = -1500 ; l < 1500 ; l++){
                    zone[i+k][j+l] += 1;
                    
                }
            }
            if(TARGET_W_SIZE+1000 <= j + 1300) {break;}
        }
        if(TARGET_H_SIZE+1000 <= i + 1300) {break;}
    }
    int a = 1000;
    int b = 2000;
    cout << "fillcol" << endl;
    for(int j = b ; j < 14000 ; j+= 2000){
        for(int i = 1000 ; i < 14000 ; i++){
            if(zone[i][j]>2){
                i+=a-1;
                continue;
            }
            if(zone[i][j]==0) break;
            for(int k = 0 ; k < a ; k++){
                double w1 = (k+1);
                double w2 = (a-k);
                zone3[i][j+k].push_back(w1/(w1+w2));
                zone3[i][j+k].push_back(w2/(w1+w2));
                if(k==0) {
                    //                    cout << i << " " << j << " " << w1 << " " << w2 << " " << w2/(w1+w2) << " " << w1/(w1+w2) << endl;
                    //                    cout << (50+(int)((255-50)*w2/(w1+w2))) << " " << (50+((int)((255-50)*w2/(w1+w2)))) << endl;
                }
                //cout << zone3[i][j+k][0] << " " << zone3[i][j+k][1] << endl;
            }
        }
    }
    cout << "fillrow" << endl;
    for(int i = b ; i < 14000 ; i+= 2000){
        for(int j = 1000 ; j < 14000 ; j++){
            if(zone[i][j]>2){
                j+=a-1;
                continue;
            }
            if(zone[i][j]==0) break;
            
            for(int k = 0 ; k < a ; k++){
                if(i+k==2214 && j==1000) cout << "asdfsafsdaf" << endl;
                double w1 = (k+1);
                double w2 = (a-k);
                zone3[i+k][j].push_back(w1/(w1+w2));
                zone3[i+k][j].push_back(w2/(w1+w2));
                //cout << w1 << " " << w2 << " " << zone3[i+k][j][0] << " " << zone3[i+k][j][1] << endl;
            }
        }
    }
    cout << "fill4" << endl;
    for(int i = b ; i < 14000 ; i+=2000){
        for(int j = b ; j < 14000 ; j+=2000){
            for(int k = 0 ; k < a ; k++){
                for(int l = 0 ; l < a ; l++){
                    if(zone[i+k][j+l]==0) break;
                    double w1 = sqrt((k+1)*(k+1)+(l+1)*(l+1));
                    double w2 = sqrt((k+1)*(k+1)+(a-l)*(a-l));
                    double w3 = sqrt((a-k)*(a-k)+(l+1)*(l+1));
                    double w4 = sqrt((a-k)*(a-k)+(a-l)*(a-l));
                    double w = w1+w2+w3+w4;
                    zone3[i+k][j+l].push_back(3*w1/w);
                    zone3[i+k][j+l].push_back(3*w2/w);
                    zone3[i+k][j+l].push_back(3*w3/w);
                    zone3[i+k][j+l].push_back(3*w4/w);
                }
                if(zone[i+k][j]==0) break;
            }
        }
    }
    cout << "end fillzone3" << endl;
}



bool checkDirection(int i, int j, int direction){
    int movei[4] = {0,2000,0,-2000};
    int movej[4] = {2000,0,-2000,0};
    if ( i+movei[direction] < 0 || j+movej[direction] < 0) return false;
    if ( i+movei[direction] >= fieldSize || j+movej[direction] >= fieldSize) return false;
    if ( zone[i+movei[direction]][j+movej[direction]] == 0) return false;
    return true;
}

void changeDirection(int &i, int &j, int &direction){
    int movei[4] = {0,2000,0,-2000};
    int movej[4] = {2000,0,-2000,0};
    int cnt = 0;
    while(cnt < 4){
        if (checkDirection(i, j, direction)){
            break;
        }else{
            direction += 1;
            direction %= 4;
            cnt++;
        }
    }
    if(cnt==4) direction = 4;
    else{
        i += movei[direction];
        j += movej[direction];
    }
}
void algorithm2(){
    showResult();
    fillzone();
    cout << "start" << endl;
    int direction = 0;
    int i = 1500;
    int j = 1500;
    while(direction <= 3){
        printer.move(i, j);
        printer.getCameraImage();
        vector<double> pos = printer.getPosition(); // x, y, theta
        double theta = pos[2];
        showResult();
        cout << "paint" << endl;
        for(double ii = -1500; ii < 1500 ; ii+=1){
            for(double jj = -1500; jj < 1500 ; jj+=1){
                //theta = 0;
                int posX = round(cos(theta)*(ii) + sin(theta)*(jj));
                int posY = round(-sin(theta)*(ii) + cos(theta)*(jj));
                if((int)pos[1] + posY < 0 || (int)pos[1] + posY >= fieldSize) continue;
                if((int)pos[0] + posX < 0 || (int)pos[0] + posX >= fieldSize) continue;
                int z = zone[(int)pos[1] + posY][(int)pos[0]+posX];
                if (z > 1){
                    if (z==2 && (((int)pos[1] + posY + (int)pos[0]+posX)%z != ((i-1500)/2000+(j-1500)/2000)%z) ) continue;
                    if (z==4 && (((int)pos[1] + posY + (int)pos[0]+posX)%z != ((i-1500)/2000+(j-1500)/1000)%z) ) continue;
                }else{
                    //if (((int)pos[1] + posY + (int)pos[0]+posX)%2 != 0) continue;
                }
                
                printer.paint2(ii,jj, expectedResult.at<Vec3b>((int)pos[1] + posY, (int)pos[0]+posX), field);
            }
        }
        //show result
        printer.getCameraImage();
        showResult();
        cout << "show result" << endl;
        zone[i][j] = 0;
        changeDirection(i, j, direction);
    }
}

void algorithm3(){
    showResult();
    fillzone();
    cout << "start" << endl;
    int direction = 0;
    int i = 1500;
    int j = 1500;
    while(direction <= 3){
        printer.move(i, j);
        printer.getCameraImage();
        vector<double> pos = printer.getPosition(); // x, y, theta
        double theta = pos[2];
        showResult();
        cout << "paint" << endl;
        for(double ii = -1500; ii < 1500 ; ii+=1){
            for(double jj = -1500; jj < 1500 ; jj+=1){
                //theta = 0;
                int posX = round(cos(theta)*(ii) + sin(theta)*(jj));
                int posY = round(-sin(theta)*(ii) + cos(theta)*(jj));
                if((int)pos[1] + posY < 0 || (int)pos[1] + posY >= fieldSize) continue;
                if((int)pos[0] + posX < 0 || (int)pos[0] + posX >= fieldSize) continue;
                int z = zone[(int)pos[1] + posY][(int)pos[0]+posX];

                if(z==0 || z==zone2[(int)pos[1] + posY][(int)pos[0]+posX]) continue;
                Vec3b exColor = expectedResult.at<Vec3b>((int)pos[1] + posY, (int)pos[0]+posX);
                Vec3b color;
                if(z!=1) {
                    color = Vec3b(exColor.val[0]+(255-exColor.val[0])/z*(z-1),exColor.val[1]+(255-exColor.val[1])/z*(z-1),exColor.val[2]+(255-exColor.val[2])/z*(z-1));
                }
                else{
                    color = Vec3b(exColor.val[0],exColor.val[1],exColor.val[2]);
                }
                zone2[(int)pos[1] + posY][(int)pos[0]+posX]++;
                printer.paint2(ii,jj, color, field);
            }
        }
        //show result
        printer.getCameraImage();
        showResult();
        cout << "show result" << endl;
        zone[i][j] = 0;
        changeDirection(i, j, direction);
    }
}

int checkCase2(int y, int x, int py, int px){
    for(int i = 2000 ; i < 14000 ; i+=2000){
        if(i <= y && y < i+1000){
            if(py > y){
                return 1;
            }else{
                return 0;
            }
        }
        if(i <= x && x < i+1000){
            if(px > x){
                return 1;
            }else{
                return 0;
            }
        }
    }
    return 0;
}

int checkCase4(int y,int x, int py, int px){
    return 0;
}

void algorithm4(){
    //showResult();
    fillzone2();
    showResult();
    cout << "start" << endl;
    int direction = 0;
    int i = 1500;
    int j = 1500;
    map<pair<int,int>,int > mp;
    while(direction <= 3){
        printer.move(i, j);
        printer.getCameraImage();
        vector<double> pos = printer.getPosition(); // x, y, theta
        double theta = pos[2];
        showResult();
        cout << "paint" << endl;
        for(double ii = -2000; ii < 2000 ; ii+=1){
            for(double jj = -2000; jj < 2000 ; jj+=1){
                //theta = 0;
                int posX = round(cos(theta)*(ii) + sin(theta)*(jj));
                int posY = round(-sin(theta)*(ii) + cos(theta)*(jj));
                if((int)pos[1] + posY < 0 || (int)pos[1] + posY >= fieldSize) continue;
                if((int)pos[0] + posX < 0 || (int)pos[0] + posX >= fieldSize) continue;
                
                int Y = (int)pos[1] + posY;
                int X = (int)pos[0]+posX;
                if(Y<j-1500 || Y >= j+1500) continue;
                if(X<i-1500 || X >= i+1500) continue;
                int z = zone[(int)pos[1] + posY][(int)pos[0]+posX];
                if(z==0 || z==zone2[(int)pos[1] + posY][(int)pos[0]+posX]) continue;
                if(Y<1000 || X<1000) continue;
                //if(z==0 || z==zone2[(int)pos[1] + posY][(int)pos[0]+posX]) continue;
                Vec3b exColor = expectedResult.at<Vec3b>((int)pos[1] + posY, (int)pos[0]+posX);
                Vec3b color;
                if(z!=1) {
                    if(z==2){
                        int c = checkCase2(Y,X,(int)pos[1], (int)pos[0]);
                        color = Vec3b(exColor.val[0]+(int)((255-exColor.val[0])*zone3[Y][X][c]),
                                      exColor.val[1]+(int)((255-exColor.val[1])*zone3[Y][X][c]),
                                      exColor.val[2]+(int)((255-exColor.val[2])*zone3[Y][X][c]));
//                        if(posY>=300 || posX >= 300){
////                            if(1990 <=X && X <= 2010){
////                                cout << Y << " " << X << " " << zone3[Y][X][0] << " " << (int)((255-exColor.val[0])*zone3[Y][X][0]) << " " << exColor.val[0]+(int)((255-exColor.val[0])*zone3[Y][X][0]) << " " << exColor.val[1]+(int)((255-exColor.val[1])*zone3[Y][X][0]) << " " << exColor.val[2]+(int)((255-exColor.val[2])*zone3[Y][X][0]) << endl;
////                            }
//                            //cout << posY << " " << posX << " " << Y << " " << X << " " << zone3[Y][X].size() << " " << zone3[Y][X][0] << endl;
//                            color = Vec3b(exColor.val[0]+(int)((255-exColor.val[0])*zone3[Y][X][0]),exColor.val[1]+(int)((255-exColor.val[1])*zone3[Y][X][0]),exColor.val[2]+(int)((255-exColor.val[2])*zone3[Y][X][0]));
//                        }else if(posY <= -300 || posX <= -300){
//                            color = Vec3b(exColor.val[0]+(int)((255-exColor.val[0])*zone3[Y][X][1]),exColor.val[1]+(int)((255-exColor.val[1])*zone3[Y][X][1]),exColor.val[2]+(int)((255-exColor.val[2])*zone3[Y][X][1]));
//                        }else{
//                            //cout << posY << " " << posX << " " << Y << " " << X << " 2" <<  endl;
//                        }
                    }else{
                        if(posY >= 300 && posX >= 300){
                            color = Vec3b(exColor.val[0]+(int)((255-exColor.val[0])*zone3[Y][X][0]),exColor.val[1]+(int)((255-exColor.val[1])*zone3[Y][X][0]),exColor.val[2]+(int)((255-exColor.val[2])*zone3[Y][X][0]));
                        }else if(posY >= 300 && posX <= -300){
                            color = Vec3b(exColor.val[0]+(int)((255-exColor.val[0])*zone3[Y][X][1]),exColor.val[1]+(int)((255-exColor.val[1])*zone3[Y][X][1]),exColor.val[2]+(int)((255-exColor.val[2])*zone3[Y][X][1]));
                        }else if(posY <= -300 && posX >= 300){
                            color = Vec3b(exColor.val[0]+(int)((255-exColor.val[0])*zone3[Y][X][2]),exColor.val[1]+(int)((255-exColor.val[1])*zone3[Y][X][2]),exColor.val[2]+(int)((255-exColor.val[2])*zone3[Y][X][2]));
                        }else if(posY <= -300 && posX <= -300){
                            color = Vec3b(exColor.val[0]+(int)((255-exColor.val[0])*zone3[Y][X][3]),exColor.val[1]+(int)((255-exColor.val[1])*zone3[Y][X][3]),exColor.val[2]+(int)((255-exColor.val[2])*zone3[Y][X][3]));
                        }else{
                            //cout << posY << " " << posX << " " << Y << " " << X << " 4" << endl;
                        }
                    }
                    
                }
                else{
                    
                        color = Vec3b(exColor.val[0],exColor.val[1],exColor.val[2]);
                    
                }
                zone2[Y][X]++;
                printer.paint2(ii,jj, color, field);
            }
        }
        //show result
        printer.getCameraImage();
        showResult();
        cout << "show result" << endl;
        zone[i][j] = 0;
        changeDirection(i, j, direction);
    }
}

void algorithm5(){
    //showResult();
    fillzone3();
    showResult();
    cout << "start" << endl;
    int direction = 0;
    int i = 1500;
    int j = 1500;
    //map<pair<int,int>,int > mp;
    while(direction <= 3){
        printer.move(i, j);
        cout << i << " " << j << endl;
        printer.getCameraImage();
        vector<double> pos = printer.getPosition(); // x, y, theta
        double theta = pos[2];
        showResult();
        cout << "paint" << endl;
        for(double ii = -2000; ii < 2000 ; ii+=1){
            for(double jj = -2000; jj < 2000 ; jj+=1){
                //theta = 0;
                int posX = round(cos(theta)*(ii) + sin(theta)*(jj));
                int posY = round(-sin(theta)*(ii) + cos(theta)*(jj));
                if((int)pos[1] + posY < 0 || (int)pos[1] + posY >= fieldSize) continue;
                if((int)pos[0] + posX < 0 || (int)pos[0] + posX >= fieldSize) continue;
                
                int Y = (int)pos[1] + posY;
                int X = (int)pos[0]+posX;
                if(Y<j-1500 || Y >= j+1500) continue;
                if(X<i-1500 || X >= i+1500) continue;
                int z = zone[Y][X];
                if(z==0 || z==zone2[(int)pos[1] + posY][(int)pos[0]+posX]) continue;
                if(Y<1000 || X<1000) continue;
                //if(z==0 || z==zone2[(int)pos[1] + posY][(int)pos[0]+posX]) continue;
                Vec3b exColor = expectedResult.at<Vec3b>((int)pos[1] + posY, (int)pos[0]+posX);
                Vec3b color;
                if(z!=1) {
                    if(z==2){
                        int c = checkCase2(Y,X,(int)pos[1], (int)pos[0]);
                        color = Vec3b(exColor.val[0]+(int)((255-exColor.val[0])*zone3[Y][X][c]),exColor.val[1]+(int)((255-exColor.val[1])*zone3[Y][X][c]),exColor.val[2]+(int)((255-exColor.val[2])*zone3[Y][X][c]));
                        //                        if(posY>=300 || posX >= 300){
                        ////                            if(1990 <=X && X <= 2010){
                        ////                                cout << Y << " " << X << " " << zone3[Y][X][0] << " " << (int)((255-exColor.val[0])*zone3[Y][X][0]) << " " << exColor.val[0]+(int)((255-exColor.val[0])*zone3[Y][X][0]) << " " << exColor.val[1]+(int)((255-exColor.val[1])*zone3[Y][X][0]) << " " << exColor.val[2]+(int)((255-exColor.val[2])*zone3[Y][X][0]) << endl;
                        ////                            }
                        //                            //cout << posY << " " << posX << " " << Y << " " << X << " " << zone3[Y][X].size() << " " << zone3[Y][X][0] << endl;
                        //                            color = Vec3b(exColor.val[0]+(int)((255-exColor.val[0])*zone3[Y][X][0]),exColor.val[1]+(int)((255-exColor.val[1])*zone3[Y][X][0]),exColor.val[2]+(int)((255-exColor.val[2])*zone3[Y][X][0]));
                        //                        }else if(posY <= -300 || posX <= -300){
                        //                            color = Vec3b(exColor.val[0]+(int)((255-exColor.val[0])*zone3[Y][X][1]),exColor.val[1]+(int)((255-exColor.val[1])*zone3[Y][X][1]),exColor.val[2]+(int)((255-exColor.val[2])*zone3[Y][X][1]));
                        //                        }else{
                        //                            //cout << posY << " " << posX << " " << Y << " " << X << " 2" <<  endl;
                        //                        }
                    }else{
                        int c1,c2,c0;
                        if(posY >= 300 && posX >= 300){
                            c0 = min(255,exColor.val[0]+(int)((255-exColor.val[0])*zone3[Y][X][0]));
                            c1 = min(255,exColor.val[1]+(int)((255-exColor.val[1])*zone3[Y][X][0]));
                            c2 = min(255,exColor.val[2]+(int)((255-exColor.val[2])*zone3[Y][X][0]));
                        }else if(posY >= 300 && posX <= -300){
                            c0 = min(255,exColor.val[0]+(int)((255-exColor.val[0])*zone3[Y][X][1]));
                            c1 = min(255,exColor.val[1]+(int)((255-exColor.val[1])*zone3[Y][X][1]));
                            c2 = min(255,exColor.val[2]+(int)((255-exColor.val[2])*zone3[Y][X][1]));
                        }else if(posY <= -300 && posX >= 300){
                            c0 = min(255,exColor.val[0]+(int)((255-exColor.val[0])*zone3[Y][X][2]));
                            c1 = min(255,exColor.val[1]+(int)((255-exColor.val[1])*zone3[Y][X][2]));
                            c2 = min(255,exColor.val[2]+(int)((255-exColor.val[2])*zone3[Y][X][2]));
                        }else if(posY <= -300 && posX <= -300){
                            c0 = min(255,exColor.val[0]+(int)((255-exColor.val[0])*zone3[Y][X][3]));
                            c1 = min(255,exColor.val[1]+(int)((255-exColor.val[1])*zone3[Y][X][3]));
                            c2 = min(255,exColor.val[2]+(int)((255-exColor.val[2])*zone3[Y][X][3]));
                        }else{
                            c0 = 255;
                            c1 = 255;
                            c2 = 255;
                            //cout << posY << " " << posX << " " << Y << " " << X << " 4" << endl;
                        }
                        color = Vec3b(c0,c1,c2);
                    }
                    
                }
                else{
                    
                        color = Vec3b(exColor.val[0],exColor.val[1],exColor.val[2]);
                    
                    
                }
                zone2[Y][X]++;
                printer.paint2(ii,jj, color, field);
            }
        }
        //show result
        printer.getCameraImage();
        showResult();
        cout << "show result" << endl;
        zone[i][j] = 0;
        changeDirection(i, j, direction);
    }
}


int main(int argc, char** argv)
{
    
    
//    Vec3b test(3,6,9);
//    cout << test << endl;
//    //test /= 3;
//    cout << test << endl;
//    Vec3b test1(3,4,5);
//    cout << test1 << endl;
//    test1 /= 3;
//    cout << test1 << endl;
//    cout << test.val[0]/3 << " " << test.val[1]/3 << " " << test.val[2]/3 << endl;
//    cout << test.val[0]/1 << " " << test.val[1]/2 << " " << test.val[2]/3 << endl;
//    uchar test2 = test.val[2];
//    cout << test2 << endl;
//    return 0;
    initialize();
    printer.getCameraImage();
    algorithm5();
    imwrite(OUTPUT_NAME, field);
    //imwrite("ex_"+OUTPUT_NAME, expectedResult);
    return 0;
}
