#include "../application.h"
#include "../../dataframe/dataframe.h"
#include "../../store/kdstore.h"
#include "adder.h"
#include "filereader.h"
#include "keybuff.h"
#include "simap.h"
#include "summer.h"

/****************************************************************************
 * Calculate a word count for given file:
 *   1) read the data (single node)
 *   2) produce word counts per homed chunks, in parallel
 *   3) combine the results
 **********************************************************author: pmaj ****/
class WordCount: public Application {
public:
  static const size_t BUFSIZE = 1024;
  Key in;
  KeyBuff kbuf;
  SIMap all;
  const char* filename_;
  size_t num_nodes_;
 
  WordCount(size_t idx, KDStore *kd, const char* filename, size_t num_nodes) : Application(idx, kd), in("data"), kbuf(new Key("wc-map-",0)) {
    filename_ = filename;
    num_nodes_ = num_nodes;
  }

  /** The master nodes reads the input, then all of the nodes count. */
  void run_() override {
    if (this_node() == 0) {
      FileReader fr(filename_);
      delete DataFrame::fromVisitor(&in, kv, "S", fr);
    }
    local_count();
    reduce();
  }
 
  /** Returns a key for given node.  These keys are homed on master node
   *  which then joins them one by one. */
  Key* mk_key(size_t idx) {
    Key * k = kbuf.c(idx).get();
    printf("Created key %s\n", k->c_str());
    return k;
  }
 
  /** Compute word counts on the local node and build a data frame. */
  void local_count() {
    DataFrame* words = (kv->waitAndGet(in));
    p("Node ").p(index).pln(": starting local count...");
    SIMap map;
    Adder add(map);
    words->local_map(add);
    delete words;
    Summer cnt(map);
    delete DataFrame::fromVisitor(mk_key(this_node()), kv, "SI", cnt);
  }
 
  /** Merge the data frames of all nodes */
  void reduce() {
    if (index != 0) return;
    pln("Node 0: reducing counts...");
    SIMap map;
    Key* own = mk_key(0);
    merge(kv->get(*own), map);
    for (size_t i = 1; i < num_nodes_; ++i) { // merge other nodes
      Key* ok = mk_key(i);
      merge(kv->waitAndGet(*ok), map);
      delete ok;
    }
    p("Different words: ").pln(map.size());
    delete own;
  }
 
  void merge(DataFrame* df, SIMap& m) {
    Adder add(m);
    df->local_map(add);
    delete df;
  }
}; // WordcountDemo
