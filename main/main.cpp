#include <iostream>
#include <opencv2/opencv.hpp>

int main() {
    cv::utils::logging::setLogLevel(cv::utils::logging::LogLevel::LOG_LEVEL_ERROR);
    cv::VideoCapture cap(0);

    if(!cap.isOpened()){
        std::cerr << "Error open webcam" << std::endl;
        return -1;
    }

    while( true ){
        cv::Mat frame;
        cap >> frame;

        if(frame.empty()){
            break;
        }

        cv::imshow("Video", frame);

        if(cv::waitKey(1) == 'q'){
            break;
        }
    }
    cap.release();
    cv::destroyAllWindows();

    return 0;
}