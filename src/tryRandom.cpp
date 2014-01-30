#include</usr/local/include/opencv2/core/core.hpp>
#include</usr/local/include/opencv2/highgui/highgui.hpp>
#include</usr/local/include/opencv2/imgproc/imgproc.hpp>
#include<iostream>

using namespace std;
using namespace cv;

void initializeCamera(VideoCapture camera, int initTime)
{
  Mat waste;
  
  for( int i=0; i<initTime ;i++ )
  {
    camera >> waste;
  }
}

long countWhites(Mat image)
{
  long count = 0;
  uchar* pix_row;
  
  for(int i=0;i<image.rows;i++)
  {
    pix_row = image.ptr<uchar>(i);
    for(int j=0;j<image.cols;j++)
    {
      if(pix_row[j]==255)
	count+=1;      
    }
  }  
  return count;
}

int main()
{
  int const threshold_value = 220;	//value of threshold_value obatined by experimentation
  int const max_binary_value = 255;
  VideoCapture camera(0);  
  initializeCamera(camera,25);
  Mat image_color;
  camera >> image_color;
  
  Mat image_bw;
  cvtColor(image_color,image_bw,CV_BGR2GRAY);
    
  Mat image_threshold;
  threshold(image_bw,image_threshold,threshold_value,max_binary_value,THRESH_BINARY); // last parameter to indicate BINARY THRESHOLD operation
  
  Mat img;
  img = image_threshold.clone();
  
  const int kernel_size = 16;
  
  Mat ROI;
  Mat image_new = Mat(img.rows,img.cols,CV_8UC(1),Scalar::all(0));
  
  
  for(int i=0;i<img.rows-kernel_size;i+=kernel_size)
  {
    for(int j=0;j<img.cols-kernel_size;j+=kernel_size)
    {
	ROI = Mat(img,Rect(j,i,kernel_size,kernel_size));	
	if(countWhites(ROI)>=(8*16))
	{
	  rectangle(image_color,Rect(j,i,kernel_size,kernel_size),Scalar(255,0,0),2,8,0);
	}
    }
  }
  namedWindow("work please!!!",CV_WINDOW_AUTOSIZE);
  imshow("work please!!!",image_color);  
  waitKey(0);
  //imwrite("./image_new.jpeg",image_new);
  return 0;
}