#include "Retrieval.h"
#include "TextRetrieval.h"
#include "ImageRetrieval.h"
#include "HybridRetrieval.h"

#include <utility>

#include "boost/filesystem.hpp"

typedef boost::filesystem::directory_iterator FileIterator;
typedef std::vector<std::pair<std::string, double> > Index;

int main(int argc, char** argv) {
  const int kNumLexicon = atoi(argv[1]);
  const float kAlpha = atof(argv[2]);
  const int kNumQuery = 10;

  const std::string dataset = "/Users/chunwei/Data/shopping/images";
  const std::string queryset = "/Users/chunwei/Data/shopping/queryimages";
  double correct = 0, total = 0;
  FileIterator end_itr;

  // Test image retrieval
  ImageRetrieval ir(dataset);
  correct = 0, total = 0;
  for (FileIterator itr(queryset); itr != end_itr; ++itr) {
    if (boost::filesystem::is_regular_file(itr->path())) {
      std::string current_file = itr->path().string();
      if (current_file.compare(current_file.size() - 3, 3, "jpg") == 0) {
        ir.Query(current_file);
        if (ir.QueryMatch(kNumQuery)) correct++;
        total++;
      }
    }
  }
  std::cout << "Image retrieval accuracy: " << std::setprecision(3)
            << correct / total << std::endl;

  // Test textual retrieval
  TextRetrieval tr(dataset, kNumLexicon);
  correct = 0, total = 0;
  for (FileIterator itr(queryset); itr != end_itr; ++itr) {
    if (boost::filesystem::is_regular_file(itr->path())) {
      std::string current_file = itr->path().string();
      if (current_file.compare(current_file.size() - 3, 3, "txt") == 0) {
        tr.Query(current_file);
        if (tr.QueryMatch(kNumQuery)) correct++;
        total++;
      }
    }
  }
  std::cout << "Text retrieval accuracy: " << std::setprecision(3)
            << correct / total << std::endl;

  // Test hyber retrieval
  HybridRetrieval hr(dataset, kNumLexicon, kAlpha);
  correct = 0, total = 0;
  for (FileIterator itr(queryset); itr != end_itr; ++itr) {
    if (boost::filesystem::is_regular_file(itr->path())) {
      std::string current_file = itr->path().string();
      if (current_file.compare(current_file.size() - 3, 3, "jpg") == 0) {
        hr.Query(current_file);
        if (hr.QueryMatch(kNumQuery)) correct++;
        total++;
      }
    }
  }
  std::cout << "Hybrid retrieval accuracy: " << std::setprecision(3)
            << correct / total << std::endl;
}