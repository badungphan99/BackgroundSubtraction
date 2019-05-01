#include <iostream>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

int width = 640;
int height = 480;
// cv::VideoWriter video("outcpp.mp4",CV_FOURCC('M','J','P','G'),30, cv::Size(width,height), false);
cv::Mat temp = cv::Mat::zeros(height,width,CV_8UC1);
cv::Mat result = cv::Mat::zeros(height,width,CV_8UC1);

void processImage(cv::Mat src)
{
    for(int i = 0; i < src.rows; i++)
    {
        for(int j = 0; j < src.cols; j++)
        {
            result.at<uchar>(i, j) = src.at<uchar>(i, j) - temp.at<uchar >(i, j);
            if(result.at<uchar>(i, j) >= 230 ) result.at<uchar>(i, j) = 0;
            if(result.at<uchar>(i, j) <= 10) result.at<uchar>(i, j) = 0;
        }
    }
}

int main() {
    cv::VideoCapture videoCapture("videoplayback-3.mp4");
    cv::Mat frame,detected_edges;
    if(!videoCapture.isOpened()) return -1;
    int lowThreshold = 20;
    int ratio = 3;
    int kernel_size = 3;
    std::vector<std::vector<cv::Point> > contours;
    std::vector<cv::Vec4i> hierarchy;
    while(true)
    {
        videoCapture >> frame;
        if(frame.empty()) break;
        cv::imshow("origin", frame);
        cv::cvtColor(frame, frame, CV_BGR2GRAY);
        processImage(frame);
        temp = frame;
        cv::blur(result, result, cv::Size(3,3));
        dilate( result, result, getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(3, 3)) );
        erode(result, result, getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(3, 3)) );
        cv::Canny(result, detected_edges, lowThreshold, lowThreshold*ratio, kernel_size);
        cv::findContours( detected_edges, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0) );
        cv::Mat draft= cv::Mat::zeros( detected_edges.size(), CV_8UC1 );
        for( int i = 0; i< contours.size(); i++ )
        {
            cv::Rect boundingBox = boundingRect(contours[i]);
            if(boundingBox.area() < 500) continue;
            cv::rectangle(draft, boundingBox, cv::Scalar(255,0,255), -1);
//            std::cout << contours[i] << "\n";
//            cv::rectangle(detected_edges,boundingBox.tl(), boundingBox.br(), cv::Scalar(255,255,255),-1);
        }
        cv::Canny(draft, detected_edges, lowThreshold, lowThreshold*ratio, kernel_size);
        cv::findContours( detected_edges, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0) );

        cv::Mat drawing= cv::Mat::zeros( detected_edges.size(), CV_8UC1 );

        for( int i = 0; i< contours.size(); i++ )
        {
            cv::Rect boundingBox = boundingRect(contours[i]);
            if(boundingBox.area() < 10000) continue;
            cv::rectangle(drawing, boundingBox, cv::Scalar(255,0,255), 2);
//            std::cout << contours[i] << "\n";
//            cv::rectangle(detected_edges,boundingBox.tl(), boundingBox.br(), cv::Scalar(255,255,255),-1);
        }

        cv::imshow("origin", frame);
        cv::imshow("test",result);
        cv::imshow("edge", drawing);
        // video.write(result);
        char key = cv::waitKey(30);
        if(key == 27) break;
    }
    return 0;
}