#include "Retrieval.h"

Retrieval::Retrieval(std::string data_folder)
    : data_folder_(data_folder), query_results_(10) {}

void Retrieval::SetDataFolder(std::string data_folder) {
  data_folder_ = data_folder;
}

void Retrieval::SetQueryFile(std::string query_file) {
  query_file_ = query_file;
}

std::string Retrieval::GetDataFolder() const { return data_folder_; }

std::string Retrieval::GetQueryFile() const { return query_file_; }

void Retrieval::Indexing(const Map &list, Index &index) {
  // Sort num_index in the vector <index>
  std::partial_sort_copy(list.begin(), list.end(), index.begin(), index.end(),
                         [](std::pair<const std::string, double> const &l,
                            std::pair<const std::string, double> const &r) {
    return l.second > r.second;
  });
}

Map Retrieval::IndexToMap(const Index &index) {
  Map ret;
  for (int i = 0; i < index.size(); ++i) {
    ret[index[i].first] = index[i].second;
  }
  return ret;
}

void Retrieval::PrintIndex(const Index &index) const {
  for (int i = 0; i < index.size(); ++i) {
    std::cout << std::setw(10) << std::setprecision(3) << index[i].second
              << " [" << index[i].first << "]" << std::endl;
  }
}

void Retrieval::PrintMap(const Map &map) const {
  for (MapIterator itr = map.begin(); itr != map.end(); ++itr) {
    std::cout << std::setw(10) << itr->second << " [" << itr->first << "]"
              << std::endl;
  }
}

void Retrieval::PrintQuery() const { PrintIndex(query_results_); }

double Retrieval::NSSD(const cv::Mat &mat1, const cv::Mat &mat2) {
  double ssd = 0;
  for (int i = 0; i < mat1.cols * mat1.rows; ++i) {
    ssd -= (mat1.at<uchar>(i) - mat2.at<uchar>(i)) *
           (mat1.at<uchar>(i) - mat2.at<uchar>(i));
  }
  return ssd;
}