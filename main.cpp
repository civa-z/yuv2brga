#include <iostream>

#include "opencv2/opencv.hpp"
#include "yuv2bgr_with_convert.hpp"


int main(int argc, char* argv[]) {

    int device_id = 0;
    if (argc != 2){
        std::cout<<"error parameter num\n";
        return -1;
    }

    std::cout<<"device: "<<argv[1]<<std::endl;
    device_id = atoi(argv[1]);

    cv::VideoCapture camera;
    camera.open(device_id);

    if (!camera.isOpened()){
        std::cout<<"Camera openfailed\n";
        return 0;
    }
    std::cout<<"Camera opened: "<<device_id<<std::endl;;

    int counter = 0;
    cv::Mat frame;
    cv::Mat yuv;
    cv::Mat bgr;
    while(true){
        camera>>frame;
        counter++;
        if (frame.empty())
        {
	        std::cout<<"empty frame\n";
            break;
        }
        std::cout<<"get frame "<<counter<<" "<<frame.rows<<" "<<frame.cols<<" "<<frame.type()<<"\n";
        cv::imshow("Camera 0", frame);
        cv::waitKey(1);

        cv::cvtColor(frame, yuv, cv::COLOR_BGR2YUV_YV12);
#if 0
        cv::imshow("Camera yuv", yuv);
        cv::waitKey(1);

        cv::cvtColor(yuv, bgr, cv::COLOR_YUV2BGR_I420);
        cv::imshow("Camera bgr", bgr);
        cv::waitKey(1);
#elif 0
        cv::Mat my_bgr(frame.cols, frame.rows, CV_8UC3);
        std::cout<<"yuv "<<" "<<yuv.rows<<" "<<yuv.cols<<" "<<yuv.type()<<"\n";
        std::cout<<"my_bgr "<<" "<<my_bgr.rows<<" "<<my_bgr.cols<<" "<<my_bgr.type()<<"\n";
  
        ConvertParameter c_p;
        c_p.img_in.cols = frame.cols;
        c_p.img_in.rows = frame.rows;
        c_p.img_in.data = yuv.data;
        c_p.img_out.channels = 3;
        c_p.img_out.data = my_bgr.data;
        c_p.rotate = 270;
        c_p.is_mirror = 1;
        nv21_to_bgr(&c_p);
        std::cout<<"my_bgr "<<" "<<my_bgr.rows<<" "<<my_bgr.cols<<" "<<my_bgr.type()<<"\n";
        cv::imshow("Camera my bgr", my_bgr);
        cv::waitKey(1);
#elif 1
        cv::Mat my_bgr(frame.rows, frame.cols, CV_8UC3);
        std::cout<<"yuv "<<" "<<yuv.rows<<" "<<yuv.cols<<" "<<yuv.type()<<"\n";
        std::cout<<"my_bgr "<<" "<<my_bgr.rows<<" "<<my_bgr.cols<<" "<<my_bgr.type()<<"\n";
  
        ConvertParameter c_p;
        c_p.img_in.cols = frame.cols;
        c_p.img_in.rows = frame.rows;
        c_p.img_in.data = yuv.data;
        c_p.img_out.channels = 3;
        c_p.img_out.data = my_bgr.data;
        c_p.rotate = 0;
        c_p.is_mirror = 0;
        nv21_to_bgr(&c_p);
        std::cout<<"my_bgr "<<" "<<my_bgr.rows<<" "<<my_bgr.cols<<" "<<my_bgr.type()<<"\n";
        cv::imshow("Camera my bgr", my_bgr);
        cv::waitKey(1);

#endif
    }

}
