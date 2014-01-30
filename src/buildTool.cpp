#include</usr/local/include/opencv2/core/core.hpp>
#include</usr/local/include/opencv2/imgproc/imgproc.hpp>
#include</usr/local/include/opencv2/highgui/highgui.hpp>
#include</usr/local/include/opencv2/video/video.hpp>
#include<iostream>

using namespace std;
using namespace cv;

void initializeCamera(VideoCapture camera, int initTime)
{
  cout<<"Initializing Camera..."<<endl;  
  Mat waste;
  
  for( int i=0; i<initTime ;i++ )
  {
    camera >> waste;
  }
  
  cout<<"Wasted "<<initTime<<" frames..."<<endl;
}

int main()
{
  VideoCapture camera(0);
  
  initializeCamera(camera,25);
  
  Mat frame;
  camera >> frame;
  
  Mat frame_bw;
  cvtColor(frame,frame_bw,CV_BGR2GRAY);
    
  int scale = 1;
  int delta = 0;
  int ddepth = CV_16S;
  Mat frame_gradient;
  Mat grad_x,grad_y,abs_grad_x,abs_grad_y;  
  Sobel(frame_bw,grad_x,ddepth,1,0,3,scale,delta,BORDER_DEFAULT);
  Sobel(frame_bw,grad_y,ddepth,0,1,3,scale,delta,BORDER_DEFAULT);
  convertScaleAbs(grad_x,abs_grad_x);
  convertScaleAbs(grad_y,abs_grad_y);
  addWeighted(abs_grad_x,0.5,abs_grad_y,0.5,0.0,frame_gradient);
  
  cout<<"Writing Images to ../images"<<endl;
  imwrite("../images/color1.jpeg",frame);
  imwrite("../images/bw1.jpeg",frame_bw);
  imwrite("../images/grad1.jpeg",frame_gradient);
  
  return 0;
}
    