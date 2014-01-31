#include<iostream>
#include</usr/local/include/opencv2/core/core.hpp>
#include</usr/local/include/opencv2/highgui/highgui.hpp>
#include</usr/local/include/opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;

const int avgThresh = 20;
int noThresh = 100;

int compareAvgVar(Mat patch_new,Mat patch_static)
{  
  int count1 = 0;
  for(int i=0;i<patch_new.rows;i++)
  {
    for(int j=0;j<patch_new.cols;j++)
    {
	if(abs(patch_new.at<uchar>(j,i)-patch_static.at<uchar>(j,i))>=avgThresh)
	{
	  count1++;
	}		
    }
  }
  
  if(count1>=noThresh)
    return 1;
  else
    return 0;  
}


int main()
{
  
  //Load Images
  Mat img,img_new;
  cout<<"Loading Images..."<<endl;
  img = imread("../images/bw.jpeg");  
  img_new = imread("../images/bw1.jpeg");
  cout<<"Displaying Loaded Images..."<<endl;
  
  //Printing Image Stats
  cout<<"\nOriginal Images"<<endl;
  cout<<"Rows->"<<img.rows<<"\tCols->"<<img.cols<<endl;
  
  //Get Kernel Size
  int kernel_size;
  cout<<"\nKernel Size ->\t";
  cin>>kernel_size;
  if((kernel_size<1) || (img.rows%kernel_size!=0) || (img.cols%kernel_size!=0))
  {
    cout<<"Illegal Kernel Size"<<endl;
    return -1;
  }
  noThresh = kernel_size * (kernel_size);    
  
  //Background Subtraction in the form of Kernel
  cout<<"Computing the resultant image..."<<endl;
  Mat roi_static,roi_new;
  Mat result = Mat(img.rows,img.cols,CV_8UC(1),Scalar::all(0));  
  
  for(int i = 0;i<img.rows-kernel_size;i+=kernel_size)
  {
    for(int j= 0; j<img.cols-kernel_size; j+=kernel_size)
    {
	roi_static = img(Rect(j,i,kernel_size,kernel_size));
	roi_new = img_new(Rect(j,i,kernel_size,kernel_size));	
	if(compareAvgVar(roi_new,roi_static)==1)
	{
	  rectangle(result,Rect(j,i,kernel_size,kernel_size),Scalar(255,255,255),-1,8,0);
	}	
    }
  }

  //Saving RESULT image
  cout<<"Writing the RESULT image to drive..."<<endl;
  imwrite("../images/result.jpeg",result);
  
  //Display RESULT image
  namedWindow("Result",CV_WINDOW_AUTOSIZE);
  imshow("Result",result);
  
  return 0;
}
  
  