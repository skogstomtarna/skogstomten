#include "ros/ros.h"
#include <image_transport/image_transport.h>
#include <opencv2/highgui/highgui.hpp>
#include <cv_bridge/cv_bridge.h>
#include "sensor_msgs/Image.h"
#include <iostream>
#include "algorithm.hpp"
#define range_20M 78
#define range_15M 59
#define range_10M 39
#define range_05M 20
/*
cv::Mat Watersgm(cv::Mat image);
cv::Mat Horizon_Mat(cv::Mat image);
cv::Mat Cut_Horizon(cv::Mat img);
double Horizon_double(cv::Mat img);
*/
std::vector<std::vector<cv::Point> > tree_marker(cv::Mat img);
std::string type2str(int type);
std::vector<std::vector<cv::Point> > RGBcontours;
//std::vector<std::vector<cv::Point> > RGB_parameter::Contours = {};
RGB_parameter left;
/**

* This tutorial demonstrates simple receipt of messages over the ROS system.
*/
void left_image_callback(const sensor_msgs::ImageConstPtr& msg){
	left.RGBimage = cv_bridge::toCvShare(msg)-> image;
	std::cout<<"hello"<<std::endl;
	int object = 0;
	int count = 0;
	std::vector<cv::Vec4i> hierarchy;
	std::vector<cv::Rect> boundRect( RGBcontours.size() );
	cv::Mat drawing;//cv::Mat drawing = cv::Mat::zeros( image.size(), CV_8UC3 );
	left.RGBimage.copyTo(drawing);
	for( int i = 0; i< RGBcontours.size(); i++ )
	{
		float ctArea= cv::contourArea(RGBcontours[i]);
		if (ctArea > 5000){
			cv::Scalar color = cv::Scalar(0, 0, 255);
			drawContours( drawing, RGBcontours, i, color, 1, 8, hierarchy, 0, cv::Point() );
			boundRect[i] = cv::boundingRect( cv::Mat(RGBcontours[i]) );
			cv::rectangle( drawing, boundRect[i].tl(), boundRect[i].br(), color, 10, 8, 0 );
			count++;
		}
	}
	object = count;
	try
	{
		cv::imshow("Result", drawing);
		//cv::imshow("depth", left.Marker);
		//cv::imshow("horizon", img_cut_05m);
		cv::waitKey(30);
	}
	catch (cv_bridge::Exception& e)
	{
		ROS_ERROR("cv_bridge exception: %s", e.what());
		return;
	}
}

void depth_image_callback(const sensor_msgs::ImageConstPtr& msg)
{
	//std::cout << "image received " <<std::endl;
	//cv_bridge::CvImagePtr cv_ptr;
	//std::string frameId;
	//ros::Time t;
	cv::Mat img;
	cv::Mat img_depth, img_depth_show;
	cv::Mat img_presgm;
	cv::Mat img_sgm;
	cv::Mat img_horizon;
	cv::Mat img_watersgm_10m, img_horizon_10m; //after get rid of area below horizon
	cv::Mat img_cut_20m, img_cut_15m, img_cut_10m, img_cut_05m; //after get rid of area below horizon

	double min, max;
	std::string encodingType;
	//t = msg->header.stamp;
	//frameId = msg->header.frame_id;
	//img.rows = msg.height;
	//img.cols = msg.width;
	//img = &msg.data;
	encodingType = msg->encoding;
	//std::cout << "format is "<< encodingType<< std::endl;
	img = cv_bridge::toCvShare(msg)-> image;
	//std::cout<<type2str(img.type())<<std::endl;
	cv::minMaxIdx(img, &min, &max);
	//std::cout << min << " /"<< max<< std::endl;
	img.convertTo(img_depth_show, CV_8UC1, 255/(max-min), 0);
	img.convertTo(img_depth, CV_8UC1, (float)255/65536, 0);
	img_depth.copyTo(img_presgm);
	RangeSegmenter Seg_20m(img_presgm, range_20M);
	img_cut_20m = Seg_20m.layer_process();
	RangeSegmenter Seg_15m(img_presgm, range_15M);
	img_cut_15m = Seg_15m.layer_process();
	RangeSegmenter Seg_10m(img_presgm, range_10M);
	img_cut_10m = Seg_10m.layer_process();
	img_watersgm_10m = Seg_10m.img_watershed;
	img_horizon_10m = Seg_10m.Horizon_Mat(img_watersgm_10m);
	RangeSegmenter Seg_05m(img_presgm, range_05M);
	img_cut_05m = Seg_05m.layer_process();
	//cv::Mat img_fuse = img_cut_05m + img_cut_15m;
	//cv::Mat img_fuse2 = img_cut_15m + img_cut_20m;
	RGBcontours = tree_marker(img_cut_10m);
	std::vector<cv::Vec4i> hierarchy;
	cv::Mat drawing = cv::Mat::zeros( img_cut_10m.size(), CV_8UC3 );
	for( int i = 0; i< RGBcontours.size(); i++ )
	{
		cv::Scalar color = cv::Scalar(0, 0, 255);
		drawContours( drawing, RGBcontours, i, color, 1, 8, hierarchy, 0, cv::Point() );
	}
	//left.Marker = img_marker;
	//cvtColor(img_depth, img_depth, CV_GRAY2RGB);
	/*
	img_sgm = Watersgm(img_presgm, range_20M);
	//img_horizon = Horizon_Mat(img_sgm);
	double y = Horizon_double(img_sgm);
	img_cut = Cut_Horizon(img_sgm, y);
	//img.convertTo(img_depth, CV_8UC1, 255/8000);
	*/
	try
	{

		cv::imshow("depth", img_depth);
		cv::imshow("watershed segmentation", img_watersgm_10m);
		cv::imshow("horizon", img_horizon_10m);
		cv::imshow("horizon cut", img_cut_10m);
		cv::imshow("Contours", drawing);
		cv::waitKey(30);
	}
	catch (cv_bridge::Exception& e)
	{
		ROS_ERROR("cv_bridge exception: %s", e.what());
		return;
	}
	//cv::imshow("depth", cv_bridge::toCvShare(msg)->image);
}

void camera_info_callback( const sensor_msgs::CameraInfo::ConstPtr& cmd_msg){
	std::cout << "Camerainfo received " <<std::endl;
}


int main(int argc, char **argv)
{
	ros::init(argc, argv, "depthvideo");
	ros::NodeHandle n;
	//std::cerr << "Depth image receiving start" << std::endl;
	cv::namedWindow("depth", CV_WINDOW_NORMAL);
	cv::namedWindow("horizon", CV_WINDOW_NORMAL);
	cv::namedWindow("horizon cut", CV_WINDOW_NORMAL);
	cv::namedWindow("watershed segmentation", CV_WINDOW_NORMAL);
	cv::namedWindow("Contours", CV_WINDOW_NORMAL);
	cv::namedWindow("Result", CV_WINDOW_NORMAL);
	cv::startWindowThread();
	image_transport::ImageTransport it(n);
	//image_transport::Subscriber left_image = it.subscribe("zed/rgb/img_raw_color", 2, left_image_callback);
	image_transport::Subscriber depth_image = it.subscribe("zed/depth/depth_registered", 1, depth_image_callback);
	image_transport::Subscriber left_image = it.subscribe("/zed/rgb/image_raw_color", 1000, left_image_callback);
	ros::Subscriber camera_info = n.subscribe("zed/left/camera_info", 1000, camera_info_callback);
	ros::spin();
	cv::destroyWindow("depth");
	cv::destroyWindow("horizon");
	cv::destroyWindow("horizon cut");
	cv::destroyWindow("Contours");
	cv::destroyWindow("watershed segmentation");
	cv::destroyWindow("Result");
	return 0;
}


