#include<iostream>
#include</usr/local/include/opencv2/core/core.hpp>
#include</usr/local/include/opencv2/highgui/highgui.hpp>
#include</usr/local/include/opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;

const float avgThresh = 10.0;
const float varTresh = 7;

float findKernelAverage(Mat patch,int power)
{
  float average = 0;
  uchar* row_header;
  
  for(int i=0;i<patch.rows;i++)
  {
    row_header = patch.ptr<uchar>(i);
    for(int j=0;j<patch.cols;j++)
    {
      average += pow(row_header[j],power);
    }
  }
  average /= (patch.rows*patch.cols);  
  return average;
}

float findKernelVariance(Mat patch, float average)
{
  float variance;
  
  variance = findKernelAverage(patch,2)-average;  
  return variance;
}

int compareAvgVar(Mat patch_new,Mat patch_static)
{
  float avgDiff,varDiff;
  float avgPatch_new,avgPatch_static,varPatch_new,varPatch_static;
  
  avgPatch_new = findKernelAverage(patch_new,1);
  avgPatch_static = findKernelAverage(patch_static,1);
  avgDiff = abs(avgPatch_new-avgPatch_static);
  varPatch_new = findKernelVariance(patch_new,avgPatch_new);
  varPatch_static = findKernelVariance(patch_static,avgPatch_static);
  varDiff = abs(varPatch_new-varPatch_static);
  //cout<<avgPatch_new<<" "<<avgPatch_static<<" "<<varPatch_new<<" "<<varPatch_static<<endl;
  //cout<<"Difference in avg:"<<avgDiff<<"\tvar:"<<varDiff<<endl<<endl;
  
  if(avgDiff<avgThresh)
  {    
    if(varDiff>varTresh)
    {  
      return 0;
    }
    else
    {
      return 1;
    }
  }
  else
  {
    //cout<<"Difference in avg:"<<avgDiff<<endl;
    return 0;
  }
}


int main()
{
  
  //Load Images
  Mat img,img_new;
  cout<<"Loading Images..."<<endl;
  img = imread("/home/rajath/Pictures/grad.jpeg");  
  img_new = imread("/home/rajath/Pictures/grad1.jpeg");
  cout<<"Displaying Loaded Images..."<<endl;
  namedWindow("Image_Static",CV_WINDOW_AUTOSIZE);
  namedWindow("Image_New",CV_WINDOW_AUTOSIZE);
  imshow("Image_Static",img);
  imshow("Image_New",img_new);
  waitKey(1000);
  
  //Printing Image Stats
  cout<<"\nOriginal Images"<<endl;
  cout<<"Rows->"<<img.rows<<"\tCols->"<<img.cols<<endl;
  
  //Get Kernel Size
  int kernel_size;
  cout<<"\nKernel Size ->\t";
  cin>>kernel_size;
  if(kernel_size<1 || kernel_size>10 || kernel_size%2==0)
  {
    cout<<"Illegal Kernel Size"<<endl;
    return -1;
  }
  
  //Padding Images
  cout<<"Padding Images on all sides..."<<endl;
  Mat pad_img, pad_img_new;
  copyMakeBorder(img,pad_img,(int)(kernel_size/2),(int)(kernel_size/2),(int)(kernel_size/2),(int)(kernel_size/2),BORDER_CONSTANT,0);
  copyMakeBorder(img_new,pad_img_new,(int)(kernel_size/2),(int)(kernel_size/2),(int)(kernel_size/2),(int)(kernel_size/2),BORDER_CONSTANT,0);
  
  //Printing Padded Images Stats
  cout<<"\nPadded Images"<<endl;
  cout<<"Rows->"<<pad_img.rows<<"\tCols->"<<pad_img.cols<<endl;
  
  //Comparing Images
  int top = 0;
  int bottom = pad_img.rows - (kernel_size/2) - 2;
  int left = 0;
  int right = pad_img.cols - (kernel_size/2) - 2;
  
  cout<<"\nBoundaries:"<<endl;
  cout<<"Top->"<<top<<"\tBottom->"<<bottom<<endl;
  cout<<"Left->"<<left<<"\tRight->"<<right<<endl;
  cout<<endl;
  
  cout<<"Computing the resultant image..."<<endl;
  Mat roi_static,roi_new;
  Mat result = Mat(img.rows,img.cols,CV_8UC(1),Scalar::all(0));  
  for(int i = top;i<bottom;i+=1)
  {
    for(int j= left; j<right; j+=1)
    {
	roi_static = pad_img(Rect(j,i,kernel_size,kernel_size));
	roi_new = pad_img_new(Rect(j,i,kernel_size,kernel_size));
	//cout<<"Pixel of Interest:\t("<<i<<","<<j<<")"<<endl;
	if(compareAvgVar(roi_new,roi_static)==0)
	{
	  result.at<uchar>(i,j) = 0;
	}
	else
	{
	  result.at<uchar>(i,j) = 255;
	}
    }
  }

  //Saving RESULT image
  cout<<"Writing the RESULT image to drive..."<<endl;
  imwrite("./result.jpeg",result);
  
  //Display RESULT image
  namedWindow("Result",CV_WINDOW_AUTOSIZE);
  imshow("Result",result);
  
  return 0;
}
  
  