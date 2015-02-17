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
  int num_data_;

  std::vector<std::pair<std::string, cv::Mat> > features_;
  Index query_results_;

  std::vector<std::string> categories_;

 public:
  Retrieval() {}
  Retrieval(std::string);

  void SetDataFolder(std::string);
  void SetQueryFile(std::string);
  std::string GetDataFolder() const;
  int GetNumData() const;
  std::string GetQueryFile() const;
  Index GetQueryResults() const;

  void Indexing(const Map&, Index&);
  Map IndexToMap(const Index&);
  void PrintIndex(const Index&, const int) const;
  void PrintMap(const Map&) const;
  void PrintQuery(const int) const;
  void Query(const std::string&);

  virtual cv::Mat ComputeFeature(const std::string&) = 0;
  virtual double NSSD(const cv::Mat&, const cv::Mat&) = 0;

  bool QueryMatch(int);
  std::string ParseString(const std::string&);
};

#endif
