#include "HybridRetrieval.h"

HybridRetrieval::HybridRetrieval(const std::string& data_folder,
                                 int num_lexicon, double alpha)
    : Retrieval(data_folder) {
  alpha_ = alpha;
  ir_ = ImageRetrieval(data_folder);
  tr_ = TextRetrieval(data_folder, num_lexicon);
  num_data_ = ir_.GetNumData();
}

bool HybridRetrieval::Replace(std::string& str, const std::string& from,
                              const std::string& to) {
  size_t start_pos = str.find(from);
  if (start_pos == std::string::npos) return false;
  str.replace(start_pos, from.length(), to);
  return true;
}

void HybridRetrieval::Query(const std::string& imagefile) {
  query_file_ = imagefile;

  std::string textfile = imagefile;
  Replace(textfile, "img", "descr");
  Replace(textfile, "jpg", "txt");
  tr_.Query(textfile);
  ir_.Query(imagefile);

  // two query list should be aligned
  query_results_ = ir_.GetQueryResults();  // deligation size of query

  Index ir_query = ir_.GetQueryResults();
  Index tr_query = tr_.GetQueryResults();

  Map ir_map = IndexToMap(ir_query);
  Map tr_map = IndexToMap(tr_query);
  int i = 0;
  for (MapIterator ir_itr = ir_map.begin(), tr_itr = tr_map.begin();
       ir_itr != ir_map.end(); ++ir_itr, ++tr_itr) {
    query_results_[i].first = ir_itr->first;
    query_results_[i].second =
        alpha_ * ir_itr->second + (1 - alpha_) * tr_itr->second;
    i++;
  }

  std::sort(query_results_.begin(), query_results_.end(),
            [](const std::pair<std::string, double>& l,
               const std::pair<std::string, double>& r) {
    return l.second > r.second;
  });
}

cv::Mat HybridRetrieval::ComputeFeature(const std::string& filename) {
  // dummy function
  cv::Mat mat;
  return mat;
}

double HybridRetrieval::NSSD(const cv::Mat&, const cv::Mat&) {
  // dummy function
  return 0.0;
}
