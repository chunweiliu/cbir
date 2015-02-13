#include "Retrieval.h"
#include "TextRetrieval.h"
#include "ImageRetrieval.h"

int main(int argc, char** argv) {
  std::string dataset = "/Users/chunwei/Data/shopping/images";

  TextRetrieval tr(dataset);
  int num_lexicon = 30;
  tr.Build(num_lexicon);
  tr.Query("/Users/chunwei/Data/shopping/queryimages/descr_bags_clutch_1.txt");
  tr.PrintQuery();

  ImageRetrieval ir(dataset);
  ir.Build();
  ir.Query("/Users/chunwei/Data/shopping/queryimages/img_bags_clutch_1.jpg");
  ir.PrintQuery();
}