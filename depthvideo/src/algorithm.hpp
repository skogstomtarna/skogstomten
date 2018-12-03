#include "opencv2/opencv.hpp"
#include <string>
#include <opencv2/highgui/highgui.hpp>
extern int object;
extern std::vector<std::vector<cv::Point> > RGBcontours;
class WatershedSegmenter{
	private:
	cv::Mat markers;
	public:
	void setMarkers(cv::Mat& markerImage)
	{
		markerImage.convertTo(markers, CV_32SC1);
	}
	cv::Mat process(cv::Mat &image)
	{
		cv::watershed(image, markers);
		//markers.convertTo(markers,CV_8U);
		return markers;
	}
};

class RangeSegmenter{
	private:
	cv::Mat src;
	int range;
	public:
	cv::Mat img_watershed;
	double horizon_y;
	//y coordinate of horizon
	RangeSegmenter(cv::Mat image, int x){
		image.copyTo(src);
		range = x;
	}
	cv::Mat Watersgm(cv::Mat image, int x);
	cv::Mat Horizon_Mat(cv::Mat img);
	double Horizon_double(cv::Mat img);
	cv::Mat Cut_Horizon(cv::Mat img, double y);
	cv::Mat layer_process(){
		img_watershed = Watersgm(src, range);
		double horizon_y = Horizon_double(img_watershed );
		return Cut_Horizon(img_watershed, horizon_y);
	}
};

class RGB_parameter{
	public:
	cv::Mat RGBimage;
	//static std::vector<std::vector<cv::Point> > Contours;
	RGB_parameter(){
		RGBimage = cv::Mat::zeros(720,1280,CV_8UC3);
	}
};

// to check mat type
std::string type2str(int type) {
	std::string r;
	uchar depth = type & CV_MAT_DEPTH_MASK;
	uchar chans = 1 + (type >> CV_CN_SHIFT);
	switch ( depth ) {
		case CV_8U: r = "8U"; break;
		case CV_8S: r = "8S"; break;
		case CV_16U: r = "16U"; break;
		case CV_16S: r = "16S"; break;
		case CV_32S: r = "32S"; break;
		case CV_32F: r = "32F"; break;
		case CV_64F: r = "64F"; break;
		default: r = "User"; break;
	}
	r += "C";
	r += (chans+'0');
	return r;
}


// the detection range could be changed.
cv::Mat RangeSegmenter::Watersgm(cv::Mat image, int x)
{
	int rows = image.rows;
	//std::cout<<rows<<std::endl;
	int cols = image.cols;
	//std::cout<<cols<<std::endl;
	cv::Mat img_C3;
	cvtColor(image, img_C3, CV_GRAY2RGB);
	//std::cout<< type2str(img_C3.type()).c_str()<<std::endl;
	cv::Mat binary = image;// = cv::imread(argv[2], 0);
	cv::Mat binary2;
	cv::threshold(binary, binary2, 1, 255, cv::THRESH_BINARY_INV);
	cv::threshold(binary, binary, x, 255, cv::THRESH_BINARY); //15meters.
	binary = binary + binary2;
	// Eliminate noise and smaller objects
	cv::Mat fg;
	cv::erode(binary,fg,cv::Mat(),cv::Point(-1,-1),1);
	// Identify image pixels without objects
	cv::Mat bg;
	cv::dilate(binary,bg,cv::Mat(),cv::Point(-1,-1),1);
	cv::threshold(bg,bg, 1, 1,cv::THRESH_BINARY_INV);
	// Create markers image
	cv::Mat markers(binary.size(),CV_8UC3,cv::Scalar(0));
	markers= fg+bg;
	// Create watershed segmentation object
	WatershedSegmenter segmenter;
	segmenter.setMarkers(markers);
	cv::Mat result = segmenter.process(img_C3);
	result.convertTo(result,CV_8UC1);
	return result;
}


// try to find horizon of filtered img. return filter_img
cv::Mat RangeSegmenter::Horizon_Mat(cv::Mat img){
	cv::Mat img_filtered;
	float max_rho = 0;
	int max_rho_index = 0;
	float max_theta = 0;
	// Filter kernel for detecting vertical edges
	float vertical_fk[5][5] = {{0,0,0,0,0}, {0,0,0,0,0}, {-1,-2,6,-2,-1}, {0,0,0,0,0}, {0,0,0,0,0}};
	// Filter kernel for detecting horizontal edges
	float horizontal_fk[5][5] = {{0,0,-1,0,0}, {0,0,-2,0,0}, {0,0,6,0,0}, {0,0,-2,0,0}, {0,0,-1,0,0}};
	cv::Mat filter_kernel = cv::Mat(5 ,5 ,CV_32FC1, horizontal_fk);
	// Apply filter
	cv::filter2D(img, img_filtered, -1, filter_kernel);
	std::vector<cv::Vec2f> lines;
	cv::HoughLines(img_filtered, lines, 10, CV_PI/2.f, 150 );
	cv::Mat img_show = img_filtered.clone();
	for(int i = 0; i < lines.size(); i ++){
		float theta = lines[i][1];
		if (theta > CV_PI/4.f){
			float rho = lines[i][0];
			if (rho >= max_rho && rho < 700){
				max_rho=rho;
				max_rho_index=i;
				max_theta=theta;
			}
		}
	}
	//std::cout<<max_rho<<std::endl;
	double a = cos(max_theta), b = sin(max_theta);
	double x0 = a * max_rho, y0 = b * max_rho;
	cv::Point pt1(cvRound(x0 + 1000 * (-b)), cvRound(y0 + 1000 * (a)));
	cv::Point pt2(cvRound(x0 - 1000 * (-b)), cvRound(y0 - 1000 * (a)));
	cv::line(img_show, pt1, pt2, cv::Scalar(255));
	return img_show;
}


// return horizon coordinate
double RangeSegmenter::Horizon_double(cv::Mat img){
	cv::Mat img_filtered;
	float max_rho = 0;
	int max_rho_index = 0;
	float max_theta = 0;
	// Filter kernel for detecting vertical edges
	float vertical_fk[5][5] = {{0,0,0,0,0}, {0,0,0,0,0}, {-1,-2,6,-2,-1}, {0,0,0,0,0}, {0,0,0,0,0}};
	// Filter kernel for detecting horizontal edges
	float horizontal_fk[5][5] = {{0,0,-1,0,0}, {0,0,-2,0,0}, {0,0,6,0,0}, {0,0,-2,0,0}, {0,0,-1,0,0}};
	cv::Mat filter_kernel = cv::Mat(5 ,5 ,CV_32FC1, horizontal_fk);
	// Apply filter
	cv::filter2D(img, img_filtered, -1, filter_kernel);
	std::vector<cv::Vec2f> lines;
	cv::HoughLines(img_filtered, lines, 10, CV_PI/2.f, 150 );
	cv::Mat img_show = img_filtered.clone();
	for(int i = 0; i < lines.size(); i ++){
		float theta = lines[i][1];
		if (theta > CV_PI/4.f){
			float rho = lines[i][0];
			if (rho >= max_rho && rho < 700){
				max_rho=rho;
				max_rho_index=i;
				max_theta=theta;
			}
		}
	}
	//std::cout<<max_rho<<std::endl;
	double a = cos(max_theta), b = sin(max_theta);
	double x0 = a * max_rho, y0 = b * max_rho;
	cv::Point pt1(cvRound(x0 + 1000 * (-b)), cvRound(y0 + 1000 * (a)));
	cv::Point pt2(cvRound(x0 - 1000 * (-b)), cvRound(y0 - 1000 * (a)));
	cv::line(img_show, pt1, pt2, cv::Scalar(255));
	return y0;
}


//cut the area blow horizon
cv::Mat RangeSegmenter::Cut_Horizon(cv::Mat img, double y){
	cv::Mat img_cut;
	img.copyTo(img_cut);
	int cols = img_cut.cols;
	int rows = img_cut.rows;
	for(int i = 0; i<rows; i++){
		for(int j = 0; j<cols; j++){
			if(i>(y-40)){
				img_cut.at<uchar>(i,j) = (uchar) 255;
			}
		}
	}
	//cv::erode(img_cut,img_cut,cv::Mat(),cv::Point(-1,-1),3);
	//cv::dilate(img_cut,img_cut,cv::Mat(),cv::Point(-1,-1),5);
	return img_cut;
}

std::vector<std::vector<cv::Point> > tree_marker(cv::Mat img)
{
	// convert to grayscale (you could load as grayscale instead)
	cv::Mat img_tree;
	img.copyTo(img_tree);
	cv::dilate(img_tree,img_tree,cv::Mat(),cv::Point(-1,-1),1);
	// compute mask (you could use a simple threshold if the image is always as good as the one you provided)
	cv::Mat mask;
	cv::threshold(img_tree, mask, 0, 255, CV_THRESH_BINARY_INV | CV_THRESH_OTSU);
	// find contours (if always so easy to segment as your image, you could just add the black/rect pixels to a vector)
	int object = 0;
	int count = 0;
	std::vector<std::vector<cv::Point> > contours;
	std::vector<cv::Vec4i> hierarchy;
	cv::findContours(mask,contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
	return contours;
}


