#ifndef HYBRID_RETRIEVAL_H_
#define HYBRID_RETRIEVAL_H_

#include "ImageRetrieval.h"
#include "TextRetrieval.h"

class HybridRetrieval : public Retrieval {
 private:
  ImageRetrieval ir_;
  TextRetrieval tr_;

  double alpha_;

 public:
  HybridRetrieval(const std::string &, int, double);

  void Build();
  void Query(const std::string &);
  bool Replace(std::string &, const std::string &, const std::string &);

  cv::Mat ComputeFeature(const std::string &);
  double NSSD(const cv::Mat &, const cv::Mat &);
};

#endif