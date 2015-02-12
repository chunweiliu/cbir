#include <string>
#include <map>
#include <utility>
#include <cassert>
#include <fstream>
#include <iomanip>
#include <cstdlib>

#include "opencv2/opencv.hpp"  // All headers for OpenCV C++
#include "opencv/cv.hpp"  // All headers for for OpenCV C
#include "boost/filesystem.hpp"

typedef std::vector<std::pair<std::string, double> > idx_type;
typedef std::map<std::string, double> dic_type;
typedef std::map<std::string, double>::const_iterator itr_dic_type;

idx_type Indexing(const dic_type &dic, int num_top) {
  idx_type top(num_top);
  std::partial_sort_copy(dic.begin(),
                         dic.end(),
                         top.begin(),
                         top.end(),
                         [](std::pair<const std::string, double> const &l,
                            std::pair<const std::string, double> const &r)
                         { return l.second > r.second; }
                         );
  return top;
}

dic_type IndexToDict(const idx_type & idx) {
  dic_type ret;
  for (int i = 0; i < idx.size(); ++i) {
    ret[idx[i].first] = idx[i].second;
  }
  return ret; 
}

void PrintIndex(const idx_type &dict) {
  for (int i = 0; i < dict.size(); ++i) {
    std::cout << std::setw(10) << dict[i].second << " [" << dict[i].first << "]" << std::endl;
  }
}

void PrintDict(const dic_type &dict) {
  for (itr_dic_type itr = dict.begin();
       itr != dict.end();
       ++itr) {
    std::cout << std::setw(10) << itr->second << " [" << itr->first << "]" << std::endl;
  }
}

double SSD(const dic_type &dic1, const dic_type &dic2) {
  assert(dic1.size() == dic2.size());
  double ssd = 0;
  for (itr_dic_type itr1 = dic1.begin(), itr2 = dic2.begin();
       itr1 != dic1.end();
       ++itr1, ++itr2) {
    ssd -= (itr1->second - itr2->second)*(itr1->second - itr2->second);
  }
  return ssd;
}

void WordsToVector(const std::string &filename,
                   const dic_type &lexicon,
                   dic_type &vec) {
  std::ifstream infile(filename);
  std::string word;
  for (itr_dic_type itr = lexicon.begin();
       itr != lexicon.end();
       ++itr) {
    vec[itr->first] = 0;
  }
  while (infile >> word) {
    if (lexicon.find(word) != lexicon.end()) {
      vec[word]++;
    }
  }
}

void WordsToVector(const std::string &filename,
                   dic_type &vec) {
  std::ifstream infile(filename);
  std::string word;
  while (infile >> word) {
    vec[word]++;
  }
}

int main(int argc, char** argv)
{
  // Input check
  if (argc < 4) {
    printf("Usage: %s <query> <dataset> <num_lexicon>\n", argv[0]);
    return -1;
  }
  std::string query_name(argv[1]);
  std::string folder(argv[2]);
  const int num_lexicon = atoi(argv[3]);  // defalut: 40

  // Load a data set and build a lexicon
  dic_type dictionary;

  boost::filesystem::directory_iterator end_itr;
  for (boost::filesystem::directory_iterator itr(folder);
       itr != end_itr;
       ++itr) {
    if (boost::filesystem::is_regular_file(itr->path())) {
      std::string current_file = itr->path().string();
      if (current_file.compare(current_file.size()-3, 3, "txt") == 0) {
        // Compute for each file here
        WordsToVector(current_file, dictionary);
      }
    }
  }

  idx_type indexing = Indexing(dictionary, num_lexicon);
  dic_type lexicon = IndexToDict(indexing);
  // PrintDict(lexicon);

  // Load a query
  dic_type query_feature;
  WordsToVector(query_name, lexicon, query_feature);

  // Given a lexicon, compute feature vectors
  dic_type result;
  for (boost::filesystem::directory_iterator itr(folder);
       itr != end_itr;
       ++itr) {
    if (boost::filesystem::is_regular_file(itr->path())) {
      std::string current_file = itr->path().string();
      if (current_file.compare(current_file.size()-3, 3, "txt") == 0) {
        // Compute for each file here
        dic_type data_feature;// = lexicon;        
        WordsToVector(current_file, lexicon, data_feature);
        result[current_file] = SSD(query_feature, data_feature);
        // PrintDict(feature_vector);
      }
    }
  }
  idx_type top = Indexing(result, 10);
  PrintIndex(top);

  return 0;
}