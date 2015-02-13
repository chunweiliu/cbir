#ifndef IMAGE_RETRIEVAL_H_
#define IMAGE_RETRIEVAL_H_

#include "Retrieval.h"

#include "boost/filesystem.hpp"
#include "opencv/cv.hpp"

typedef boost::filesystem::directory_iterator FileIterator;

class ImageRetrieval : public Retrieval {
 public:
  using Retrieval::Retrieval;

  void Build();
  cv::Mat ComputeFeature(const std::string&);
  void Query(const std::string&);
};

#endif