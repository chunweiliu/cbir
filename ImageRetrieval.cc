#include <string>
#include <map>
#include <utility>
#include <cassert>

#include "opencv2/opencv.hpp"  // All headers for OpenCV C++
#include "opencv/cv.hpp"  // All headers for for OpenCV C
#include "boost/filesystem.hpp"


double SSD(const cv::Mat &im1, const cv::Mat &im2) {
  assert(im1.rows == im2.rows && im1.cols == im2.cols);
  double ssd = 0;
  for (int y = 0; y < im1.rows; ++y) {
    for (int x = 0; x < im1.cols; ++x) {
      cv::Scalar d = im1.at<uchar>(y,x) - im2.at<uchar>(y,x);
      ssd += d[0]*d[0];
    }
  }
  return ssd;
}

cv::Mat TinyGrayImage(const cv::Mat &image) {
  const int kSize = 32;
  cv::Mat ret;  
  cv::cvtColor(image, ret, CV_BGR2GRAY);
  cv::resize(ret, ret, cv::Size(kSize, kSize), 0, 0, cv::INTER_CUBIC);
  return ret;
}

int main(int argc, char** argv)
{
  // Input check
  if (argc < 3) {
    printf("Usage: %s query_image dataset\n", argv[0]);
    return -1;
  }
  std::string query_image_name(argv[1]);  
  std::string image_folder(argv[2]);

  // Load query image
  cv::Mat image;
  image = cv::imread(query_image_name, 1);
  if (!image.data) {
    printf("No image data \n");
    return -1;
  }

  // Load data set


  // Convert it to a gray one
  cv::Mat query_image = TinyGrayImage(image);

  // For all images
  std::map <std::string, double> indexing;
  boost::filesystem::directory_iterator end_itr;
  for (boost::filesystem::directory_iterator itr(image_folder); 
       itr != end_itr; ++itr) {
    if (boost::filesystem::is_regular_file(itr->path())) {  // not a directory
      std::string current_file = itr->path().string();
      if (current_file.compare(current_file.size()-3, 3, "jpg") == 0) {
        cv::Mat image;
        image = cv::imread(current_file, 1);
        cv::Mat feature = TinyGrayImage(image);
                
        // compute SSD
        double ssd = SSD(feature, query_image);

        // store into a hash map
        indexing[current_file] = ssd;
      }
    }
  }

  // Query
  std::vector<std::pair<std::string, double> > top_ten(10);
  std::partial_sort_copy(indexing.begin(),
                         indexing.end(),
                         top_ten.begin(),
                         top_ten.end(),
                         [](std::pair<const std::string, int> const& l,
                            std::pair<const std::string, int> const& r)
                         {
                           return l.second > r.second;
                         });

  // Display the results
  for (int i = 0; i < 10; ++i) {
    std::cout << top_ten[i].first << std::endl;
  }
  cv::imshow(query_image_name, query_image);

  cv::waitKey(0);

  return 0;
}