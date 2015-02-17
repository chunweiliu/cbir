# CBIR
A demo program of a content-based image retrieval system using visual and textual features

## Prerequisites
- OpenCV (v3.0, built from the source code)
- Boost (the latest version download from Homebrew)
- CMake

## Overview of the program
1. Pre-compute the image/text feature for all images in a data set
2. Given a query image from a separated data set, compute the image/text feature of that image, and calculate the sum of square (SSD) distance between the feature of the query image and the entire data set
3. Pick the top `kNumQuery` images in terns of minimum SSD, and then choose the majority of the ensemble as a final prediction
4. Evaluate the accuracy of the predictions through the entire query image set

### The image feature
- The image feature is a simple 32 by 32 black and white patch.

### The text feature
- The text feature is a histogram of lexicon choosing from top `kNumLexicon`.
- Here is an example of a lexicon built with the 20 most frequent words (i.e. `kNumLexicon=20`) in the data set:

~~~
3393 [a]
3220 [and]
2586 [with]
2382 [the]
1297 [;]
1212 [leather]
1185 [of]
1058 [in]
1013 [The]
1004 [for]
 980 [to]
 872 [your]
 836 [-]
 811 [from]
 764 [is]
 646 [This]
 520 [A]
 494 [clutch]
 488 [Leather]
 432 [features]
~~~

### The hybrid feature
- The hybrid feature is a linear combination of the SSDs computed from image and text features.
- E.g `kAlpha` * SSD_image + (1-`kAlpha`) * SSD_text


##Evaluations

### Th image feature
Using the image feature only have achieved a high accuracy of 0.925 (37/40).

### The textual feature
I tried both not pruning and pruning the lexicon by removing the stop words, such as "a", "for", "the", ...etc.

My observation is the accuracy is roughly proportional to  the number of words in the lexicon.
Even using the unpruned lexicon, the accuracy would slowly converge to an accuracy of 0.95.
Here is a summary of the number of words in the lexicon to the accuracy:
![kNumLexicon](images/kNumLexicon.png)

The pruned lexicon reaches the convergence accuracy rate 0.95 only used about 50 words.
Yet the unpruned lexicon needs 100 words to get an 0.95 accuracy rate.
You can see some keywords helps to boost the accuracy.
The burned lexicon has the following benefits:
1. Gathering such keywords faster than the unpruned one
2. Having no distraction from the stop words

### The hybrid feature




## Log
1. 01/29/2015 System setup (installed the latest OpenCV and Boost, and wrote the CMakeLists.txt)
2. 02/04/2015 Commit ImageRetrieval v1.0
3. 02/12/2015 Commit TextRetrieval v1.0
5. 02/14/2015 Commit HybirdRetrieval v.10
6. 02/16/2015 Setup gflags in CMakeLists.txt failed (TODO)

## Appendix
### Top 100 words in the lexicon
~~~
  1  3.39e+03 [a]
  2  3.22e+03 [and]
  3  2.59e+03 [with]
  4  2.38e+03 [the]
  5   1.3e+03 [;]
  6  1.21e+03 [leather]
  7  1.18e+03 [of]
  8  1.06e+03 [in]
  9  1.01e+03 [The]
 10     1e+03 [for]
 11       980 [to]
 12       872 [your]
 13       836 [-]
 14       811 [from]
 15       764 [is]
 16       646 [This]
 17       520 [A]
 18       494 [clutch]
 19       488 [Leather]
 20       432 [features]
 21       429 [hobo]
 22       417 [you]
 23       413 [zip]
 24       412 [this]
 25       411 [Hobo]
 26       396 [upper]
 27       392 [bag]
 28       385 [or]
 29       384 [Clutch]
 30       367 [at]
 31       364 [style]
 32       362 [by]
 33       344 [shoulder]
 34       331 [on]
 35       323 [an]
 36       300 [Pumps]
 37       292 [With]
 38       287 [that]
 39       287 [classic]
 40       281 [Handbags]
 41       275 [patent]
 42       274 [zipper]
 43       274 [lining]
 44       256 [toe]
 45       256 [look]
 46       252 [x]
 47       249 [heel.]
 48       249 [has]
 49       242 [are]
 50       239 [strap.]
 51       239 [top]
 52       236 [flats]
 53       227 [heel]
 54       224 [footbed]
 55       221 [made]
 56       218 [Made]
 57       217 [rubber]
 58       217 [flat]
 59       214 [strap]
 60       210 [shoe]
 61       206 [Holds]
 62       206 [suede]
 63       203 [these]
 64       203 [women's]
 65       202 [personal]
 66       202 [pumps]
 67       201 [closure.]
 68       198 [Bags]
 69       198 [These]
 70       197 [perfect]
 71       196 [Heel]
 72       194 [closure]
 73       194 [all]
 74       192 [leather.]
 75       189 [Evening]
 76       189 [wallet,]
 77       188 [google_extractor.sh]
 78       188 [readme.txt]
 79       188 [like_extractor.sh]
 80       188 [data]
 81       187 [wide]
 82       185 [100%]
 83       185 [Black]
 84       184 [sole]
 85       182 [pocket]
 86       181 [handbag]
 87       181 [stylish]
 88       180 [slip-on]
 89       179 [while]
 90       178 [&]
 91       176 [be]
 92       173 [Pump]
 93       173 [sole.]
 94       167 [chic]
 95       166 [casual]
 96       165 [Single]
 97       164 [will]
 98       164 [day]
 99       162 [sunglasses,]
100       162 [great]

~~~
