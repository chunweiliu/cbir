#include "ImageRetrieval.h"

void ImageRetrieval::Build() {
  // Compute features for each image
  FileIterator end_itr;
  for (FileIterator itr(data_folder_); itr != end_itr; ++itr) {
    if (boost::filesystem::is_regular_file(itr->path())) {
      std::string current_file = itr->path().string();
      if (current_file.compare(current_file.size() - 3, 3, "jpg") == 0) {
        cv::Mat mat = ComputeFeature(current_file);
        std::pair<std::string, cv::Mat> feature(current_file, mat);
        features_.push_back(feature);
      }
    }
  }
}

cv::Mat ImageRetrieval::ComputeFeature(const std::string &filename) {
  cv::Mat image, feature;
  image = cv::imread(filename, 1);
  cv::cvtColor(image, feature, CV_BGR2GRAY);
  cv::resize(feature, feature, cv::Size(32, 32), 0, 0, cv::INTER_CUBIC);
  return feature;
}

void ImageRetrieval::Query(const std::string &filename) {
  cv::Mat query = ComputeFeature(filename);
  Map temp;
  for (int i = 0; i < features_.size(); ++i) {
    temp[features_[i].first] = NSSD(query, features_[i].second);
  }
  Indexing(temp, query_results_);
}