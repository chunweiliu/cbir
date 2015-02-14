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

  ir_.Query(imagefile);

  std::string textfile = imagefile;
  Replace(textfile, "img", "descr");
  Replace(textfile, "jpg", "txt");
  tr_.Query(textfile);

  query_results_ = ir_.GetQueryResults();  // deligation size of query results
  Index text_query = tr_.GetQueryResults();
  for (int i = 0; i < query_results_.size(); ++i) {
    query_results_[i].second = (alpha_ * query_results_[i].second +
                                (1 - alpha_) * text_query[i].second);
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
