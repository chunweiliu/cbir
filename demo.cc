#include "Retrieval.h"
#include "TextRetrieval.h"
#include "ImageRetrieval.h"
#include "HybridRetrieval.h"

#include <utility>

#include "boost/filesystem.hpp"
// #include "gflags/gflags.h"

typedef boost::filesystem::directory_iterator FileIterator;
typedef std::vector<std::pair<std::string, double> > Index;

int main(int argc, char** argv) {
  // gflags::ParseCommandLineFlags(&argc, &argv, true);

  if (argc < 5) {
    std::cout << "Usage: ./demo <dataset> <queryset> <kNumQuery> "
                 "<kNumLexicon> <kAlpha>" << std::endl;
    return -1;
  }
  const std::string dataset(argv[1]);
  const std::string queryset(argv[2]);
  const int kNumQuery = atoi(argv[3]);
  const int kNumLexicon = atoi(argv[4]);
  const float kAlpha = atof(argv[5]);
  std::cout << "CBIR DEMO" << std::endl;
  std::cout << "Dataset: " << dataset << std::endl;
  std::cout << "Queryset: " << queryset << std::endl;
  std::cout << "kNumQuery=" << kNumQuery << ", "
            << "kNumLexicon(unpruned)=" << kNumLexicon << ", "
            << "kAlpha=" << kAlpha << std::endl;

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
            << correct / total << " (" << correct << "/" << total << ")"
            << std::endl;

  // Test textual retrieval
  TextRetrieval tr(dataset, kNumLexicon);
  correct = 0, total = 0;
  for (FileIterator itr(queryset); itr != end_itr; ++itr) {
    if (boost::filesystem::is_regular_file(itr->path())) {
      std::string current_file = itr->path().string();
      if (current_file.compare(current_file.size() - 3, 3, "txt") == 0) {
        tr.Query(current_file);
        if (tr.QueryMatch(kNumQuery)) {
          correct++;
        }
        total++;
      }
    }
  }
  tr.PrintLexicon();
  std::cout << "Text retrieval accuracy: " << std::setprecision(3)
            << correct / total << " (" << correct << "/" << total << ")"
            << std::endl;

  // Test hyber retrieval
  HybridRetrieval hr(dataset, kNumLexicon, kAlpha);
  correct = 0, total = 0;
  for (FileIterator itr(queryset); itr != end_itr; ++itr) {
    if (boost::filesystem::is_regular_file(itr->path())) {
      std::string current_file = itr->path().string();
      if (current_file.compare(current_file.size() - 3, 3, "jpg") == 0) {
        hr.Query(current_file);
        if (hr.QueryMatch(kNumQuery)) {
          correct++;
        }
        total++;
      }
    }
  }
  std::cout << "Hybrid retrieval accuracy: " << std::setprecision(3)
            << correct / total << " (" << correct << "/" << total << ")"
            << std::endl;
}