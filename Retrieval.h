#ifndef RETRIEVAL_H_
#define RETRIEVAL_H_

#include <iomanip>
#include <map>
#include <string>
#include <unordered_map>
#include <vector>

#include "opencv2/opencv.hpp"

typedef std::vector<std::pair<std::string, double> > Index;
typedef std::map<std::string, double> Map;
typedef std::map<std::string, double>::const_iterator MapIterator;

class Retrieval {
 protected:
  std::string data_folder_;
  std::string query_file_;

  std::vector<std::pair<std::string, cv::Mat> > features_;
  Index query_results_;

 public:
  Retrieval(std::string);

  void SetDataFolder(std::string);
  void SetQueryFile(std::string);
  std::string GetDataFolder() const;
  std::string GetQueryFile() const;

  void Indexing(const Map&, Index&);
  Map IndexToMap(const Index&);
  void PrintIndex(const Index&) const;
  void PrintMap(const Map&) const;
  void PrintQuery() const;
  double NSSD(const cv::Mat&, const cv::Mat&);
};

#endif
