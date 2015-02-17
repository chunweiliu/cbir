#include "TextRetrieval.h"

TextRetrieval::TextRetrieval(const std::string &data_folder, int num_lexicon)
    : Retrieval(data_folder) {
  Build(num_lexicon);
  num_data_ = features_.size();
  query_results_.resize(num_data_);
}

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
  // BuildUnpurnedLexicon(num_lexicon);
  BuildPurnedLexicon(num_lexicon);

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

void TextRetrieval::BuildUnpurnedLexicon(int num_lexicon) {
  Index index(num_lexicon);
  Indexing(data_, index);
  lexicon_ = IndexToMap(index);
  // PrintIndex(index, index.size());
  // PrintLexicon();
}

void TextRetrieval::BuildPurnedLexicon(int num_lexicon) {
  Index index(num_lexicon);
  Indexing(data_, index);
  // remove stop words
  std::vector<std::string> stop_words = {
      "A",    "The", "This",  "With",  "a",    "an", "and",  "are", "as",
      "at",   "for", "from",  "in",    "is",   "of", "on",   "or",  "our",
      "that", "the", "their", "these", "this", "to", "with", "you", "your"};

  for (int i = 0; i < index.size(); ++i) {
    bool flag = false;
    for (int j = 0; j < stop_words.size(); ++j) {
      if (index[i].first.compare(stop_words[j]) == 0) {
        flag = true;
        break;
      }
    }
    if (flag == true) {
      std::swap(index[i--], index.back());
      index.pop_back();
    }
  }
  lexicon_ = IndexToMap(index);
  // PrintIndex(index, index.size());
  // PrintLexicon();
}

cv::Mat TextRetrieval::ComputeFeature(const std::string &filename) {
  Map map;
  WordsToMap(filename, lexicon_, map);

  cv::Mat mat(1, map.size(), cv::DataType<double>::type);
  MapToMat(map, mat);
  return mat;
}

void TextRetrieval::MapToMat(const Map &map, cv::Mat &mat) const {
  int i = 0, max = 1;
  for (MapIterator itr = map.begin(); itr != map.end(); ++itr) {
    mat.at<double>(0, i++) = itr->second;

    // if (itr->second > max) {
    //   max = itr->second;
    // }
  }
  // for (MapIterator itr = map.begin(); itr != map.end(); ++itr) {
  //   mat.at<double>(0, i++) /= max;
  // }
}

void TextRetrieval::PrintLexicon() const {
  std::cout << "Lexicon (" << lexicon_.size() << ")" << std::endl;
  PrintMap(lexicon_);
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

double TextRetrieval::NSSD(const cv::Mat &mat1, const cv::Mat &mat2) {
  double ssd = 0;
  for (int i = 0; i < mat1.cols * mat1.rows; ++i) {
    ssd -= (mat1.at<double>(i) - mat2.at<double>(i)) *
           (mat1.at<double>(i) - mat2.at<double>(i));
  }
  return ssd / (mat1.cols * mat1.rows);
}
