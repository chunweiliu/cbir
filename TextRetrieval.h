#ifndef TEXT_RETRIEVAL_H_
#define TEXT_RETRIEVAL_H_

#include "Retrieval.h"

#include <fstream>

#include "boost/filesystem.hpp"

typedef boost::filesystem::directory_iterator FileIterator;

class TextRetrieval : public Retrieval {
 private:
  Map data_;
  Map lexicon_;

 public:
  using Retrieval::Retrieval;

  void Build(int);  // Build lexicon given the number of words
  cv::Mat ComputeFeature(const std::string&);
  void Query(const std::string&);

  void PrintLexicon() const;
  void MapToMat(const Map&, cv::Mat&) const;
  void WordsToMap(const std::string&, const Map&, Map&);
  void WordsToMap(const std::string&, Map&);
};

#endif