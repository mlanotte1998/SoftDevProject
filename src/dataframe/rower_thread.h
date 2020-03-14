#pragma once
// lang:CwC

#include <iostream>
#include <functional>
#include "thread.h"
#include "row.h"

class RowerThread : public Thread {
public:

  ModifiedDataFrame* df_;
  Rower* rower_;
  size_t start_;
  size_t end_;

  RowerThread(ModifiedDataFrame* df, Rower* rower, size_t start, size_t end) {
    df_ = df;
    rower_ = rower;
    start_ = start;
    end_ = end;
  }

  virtual void run() {
    df_->indexed_map(*rower_, start_, end_);
  }

};
