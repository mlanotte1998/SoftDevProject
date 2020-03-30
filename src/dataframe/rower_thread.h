#pragma once
// lang:CwC

#include <iostream>
#include <functional>
#include "thread.h"
#include "row.h"

/**
 * RowerThread::
 *
 * This class is used by dataframe's pmap method to asynchronously row through
 * a dataframe. Each rower thread operates on a subset of rows of the dataframe
 * defined by the start_ and end_ index values. The rower operation to perform
 * is based on the rower_.
 */
class RowerThread : public Thread {
public:

  ModifiedDataFrame* df_;
  Rower* rower_;
  size_t start_;
  size_t end_;

  /** RowerThread constructor */
  RowerThread(ModifiedDataFrame* df, Rower* rower, size_t start, size_t end) {
    df_ = df;
    rower_ = rower;
    start_ = start;
    end_ = end;
  }

  /** Run rower thread on subset of dataframe */
  virtual void run() {
    df_->indexed_map(*rower_, start_, end_);
  }

};
