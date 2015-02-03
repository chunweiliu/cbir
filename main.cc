#include <string>

#include "opencv2/highgui/highgui.hpp"
#include "opencv/cv.hpp"
#include "boost/filesystem.hpp"

using namespace std;
using namespace cv;

// namespace fs = ::boost::filesystem;

// void GetFiles(const fs::path& root, const string& ext, 
  // vector<fs::path>& ret) {
  // if (!fs::exists(root) || !fs::is_directory(root)) return;

  // fs::recursive_directory_iterator it(root);
  // fs::recursive_directory_iterator endit;

  // while (it != endit) {
  //   if (fs::is_regular_file(*it) && it->path().extension() == ext) {
  //     ret.push_back(it->path().filename());
  //     ++it;
  //   }
  // }
// }



Mat TinyGrayImage(const Mat &image) {
  const int kSize = 32;
  Mat ret;  
  cvtColor(image, ret, CV_BGR2GRAY);
  resize(ret, ret, Size(kSize, kSize), 0, 0, INTER_CUBIC);
  return ret;
}

int main(int argc, char** argv)
{
  // Input check
  if (argc < 3) {
    printf("Usage: %s query_image dataset\n", argv[0]);
    return -1;
  }
  string image_name(argv[1]);  
  string dataset(argv[2]);

  // Load query image
  Mat image;
  image = imread(image_name, 1);
  if (!image.data) {
    printf("No image data \n");
    return -1;
  }

  // Convert it to a gray one
  Mat tiny_image = TinyGrayImage(image);

  // Compute for the entire dataset


  imshow(image_name, tiny_image);

  waitKey(0);

  return 0;
}