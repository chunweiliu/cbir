#include "TextRetrieval.h"

void TextRetrieval::Build(int num_lexicon) {
  // Build the lexicon of the dataset
  FileIterator end_itr;
  for (FileIterator itr(data_folder_); itr != end_itr; ++itr) {
    if (boost::filesystem::is_regular_file(itr->path())) {
      std::string current_file = itr->path().string();
      if (current_file.compare(current_file.size() - 3, 3, "txt") == 0) {
        WordsToMap(current_file, data_);
      }
    }
  }
  Index index(num_lexicon);
  Indexing(data_, index);
  lexicon_ = IndexToMap(index);

  // Compute features for each image
  for (FileIterator itr(data_folder_); itr != end_itr; ++itr) {
    if (boost::filesystem::is_regular_file(itr->path())) {
      std::string current_file = itr->path().string();
      if (current_file.compare(current_file.size() - 3, 3, "txt") == 0) {
        cv::Mat mat = ComputeFeature(current_file);
        std::pair<std::string, cv::Mat> feature(current_file, mat);
        features_.push_back(feature);
      }
    }
  }
}

cv::Mat TextRetrieval::ComputeFeature(const std::string &filename) {
  Map map;
  WordsToMap(filename, lexicon_, map);

  cv::Mat mat(1, map.size(), cv::DataType<double>::type);
  MapToMat(map, mat);
  return mat;
}

void TextRetrieval::MapToMat(const Map &map, cv::Mat &mat) const {
  int i = 0;
  for (MapIterator itr = map.begin(); itr != map.end(); ++itr) {
    mat.at<double>(0, i++) = itr->second;
  }
}

void TextRetrieval::PrintLexicon() const { PrintMap(lexicon_); }

void TextRetrieval::Query(const std::string &filename) {
  cv::Mat query = ComputeFeature(filename);
  Map temp;
  for (int i = 0; i < features_.size(); ++i) {
    temp[features_[i].first] = NSSD(query, features_[i].second);
  }
  Indexing(temp, query_results_);
}

void TextRetrieval::WordsToMap(const std::string &filename, const Map &lexicon,
                               Map &vectors) {
  for (MapIterator itr = lexicon.begin(); itr != lexicon.end(); ++itr) {
    vectors[itr->first] = 0;
  }

  std::ifstream infile(filename);
  std::string word;
  while (infile >> word) {
    if (lexicon.find(word) != lexicon.end()) {
      vectors[word]++;
    }
  }
}

void TextRetrieval::WordsToMap(const std::string &filename, Map &vectors) {
  std::ifstream infile(filename);
  std::string word;
  while (infile >> word) {
    vectors[word]++;
  }
}