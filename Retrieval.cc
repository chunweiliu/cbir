#include "Retrieval.h"

Retrieval::Retrieval(std::string data_folder) : data_folder_(data_folder) {
  categories_ = {"bags", "womens"};
}

void Retrieval::SetDataFolder(std::string data_folder) {
  data_folder_ = data_folder;
}

void Retrieval::SetQueryFile(std::string query_file) {
  query_file_ = query_file;
}

std::string Retrieval::GetDataFolder() const { return data_folder_; }

int Retrieval::GetNumData() const { return num_data_; }

std::string Retrieval::GetQueryFile() const { return query_file_; }

Index Retrieval::GetQueryResults() const { return query_results_; }

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

void Retrieval::PrintIndex(const Index &index, const int num_print) const {
  const int num = index.size() > num_print ? num_print : index.size();
  for (int i = 0; i < num; ++i) {
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

void Retrieval::PrintQuery(const int num_print) const {
  PrintIndex(query_results_, num_print);
}

double Retrieval::NSSD(const cv::Mat &mat1, const cv::Mat &mat2) {
  double ssd = 0;
  for (int i = 0; i < mat1.cols * mat1.rows; ++i) {
    ssd -= (mat1.at<uchar>(i) - mat2.at<uchar>(i)) *
           (mat1.at<uchar>(i) - mat2.at<uchar>(i));
  }
  return ssd / (mat1.cols * mat1.rows);
}

bool Retrieval::QueryMatch(int num_query) {
  const std::string query_category = ParseString(query_file_);

  Map category_votes;
  for (int i = 0; i < num_query; ++i) {
    category_votes[ParseString(query_results_[i].first)]++;
  }
  Index top_votes(categories_.size());
  Indexing(category_votes, top_votes);

  // debug message
  // std::cout << query_category << std::endl;
  // PrintIndex(top_votes, 2);

  if (query_category.compare(ParseString(top_votes[0].first)) == 0) {
    return true;
  } else {
    return false;
  }
}

std::string Retrieval::ParseString(const std::string &filename) {
  for (int i = 0; i < categories_.size(); ++i) {
    std::string::size_type n = filename.find(categories_[i]);
    if (n != std::string::npos) {
      return categories_[i];
    }
  }
  return "null";
}

void Retrieval::Query(const std::string &filename) {
  query_file_ = filename;

  cv::Mat query = ComputeFeature(filename);
  Map temp;
  for (int i = 0; i < features_.size(); ++i) {
    temp[features_[i].first] = NSSD(query, features_[i].second);
  }
  Indexing(temp, query_results_);
}
