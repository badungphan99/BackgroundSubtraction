#include <iostream>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

int width = 1280;
int height = 720;
cv::VideoWriter video("outcpp.mp4",CV_FOURCC('M','J','P','G'),30, cv::Size(width,height), false);
cv::Mat temp = cv::Mat::zeros(height,width,CV_8UC1);
cv::Mat result = cv::Mat::zeros(height,width,CV_8UC1);

void processImage(cv::Mat src)
{
    for(int i = 0; i < src.rows; i++)
    {
        for(int j = 0; j < src.cols; j++)
        {
            result.at<int>(i, j) = src.at<int>(i, j) - temp.at<int>(i, j);
            if(result.at<int>(i, j) <= 2 ) result.at<int>(i, j) = 0;
        }
    }
}

int main() {
    cv::VideoCapture videoCapture("test.mp4");
    cv::Mat frame;
    while(true)
    {
        if(!videoCapture.isOpened()) break;
        videoCapture >> frame;
        cv::cvtColor(frame, frame, CV_BGR2GRAY);
        processImage(frame);
        temp = frame;
        cv::imshow("test",result);
        video.write(result);
        char key = cv::waitKey(30);
        if(key == 27) break;
    }
    return 0;
}