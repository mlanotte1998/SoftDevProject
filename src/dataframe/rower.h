//
// Created by Michael Lanotte and Daniel Welch on 2/11/20.
//

/***
*  Rower::
*  An interface for iterating through each row of a data frame. The intent
*  is that this class should subclassed and the accept() method be given
*  a meaningful implementation. Rowers can be cloned for parallel execution.
*/
class Rower : public Object {
public:
    /** This method is called once per row. The row object is on loan and
        should not be retained as it is likely going to be reused in the next
        call. The return value is used in filters to indicate that a row
        should be kept. */
    virtual bool accept(Row& r) {
      return false;
    }

    /** Once traversal of the data frame is complete the rowers that were
        split off will be joined.  There will be one join per split. The
        original object will be the last to be called join on. The join method
        is reponsible for cleaning up memory. */
    virtual void join_delete(Rower* other) {
      return;
    }
};

/***
*  SumColumnRower:
*  Sums all of the values in a given column index if the column type is float or int
*/
class SumColumnRower : public Rower {
public :

  double sum_;
  size_t col_;

  SumColumnRower(int col) {
    sum_ = 0;
    col_ = col;
  }

  ~SumColumnRower() {
    sum_ = 0;
    col_ = 0;
  }

  bool accept(Row& r) {
    if (r.width() > col_) {
      if (r.col_type(col_) == 'I') {
        sum_ += r.get_int(col_);
      } else if( r.col_type(col_) == 'F') {
        sum_ += r.get_float(col_);
      }
    }
    return true;
  }

  void join_delete(Rower* other) {
    SumColumnRower* other_cast = dynamic_cast<SumColumnRower*>(other);
    if (other != nullptr) {
      sum_ += other_cast->sum_;
      delete other;
    }
  }

  Object* clone() {
    return new SumColumnRower(col_);
  }

  double get_sum_() {
    return sum_;
  }

};

/***
*  SumAllColumnRower:
*  Sums all of the values in a given column index if the column type is float or int
*/
class SumAllColumnRower : public Rower {
public :

  double sum_;

  SumAllColumnRower() {
    sum_ = 0;
  }

  ~SumAllColumnRower() {
    sum_ = 0;
  }

  bool accept(Row& r) {
    for (int i = 0; i < r.width(); i++) {
      if (r.col_type(i) == 'I') {
        sum_ += r.get_int(i);
      } else if( r.col_type(i) == 'F') {
        sum_ += r.get_float(i);
      } else if( r.col_type(i) == 'S') {
        sum_ += r.get_string(i)->size();
      } else if( r.col_type(i) == 'B') {
        if (r.get_bool(i)) sum_ += 1;
      }
    }
    return true;
  }

  void join_delete(Rower* other) {
    SumAllColumnRower* other_cast = dynamic_cast<SumAllColumnRower*>(other);
    if (other != nullptr) {
      sum_ += other_cast->sum_;
      delete other;
    }
  }

  Object* clone() {
    return new SumAllColumnRower();
  }

  double get_sum_() {
    return sum_;
  }

};

/***
*  MultiColumnRower:
*  Calculates percentages of columns
*/
class MultiColumnRower : public Rower {
public :

  double win_total_percent_;
  double lose_winner_percent_;
  int win_col_;
  int lose_col_;
  int total_col_;

  MultiColumnRower(int win_col, int lose_col, int total_col) {
    win_total_percent_ = 0;
    lose_winner_percent_ = 0;
    win_col_ = win_col;
    lose_col_ = lose_col;
    total_col_ = total_col;
  }

  ~MultiColumnRower() {
    win_total_percent_ = 0;
    lose_winner_percent_ = 0;
    win_col_ = 0;
    lose_col_ = 0;
    total_col_ = 0;
  }

  bool accept(Row& r) {
    if (r.width() > win_col_ && r.width() > lose_col_ && r.width() > total_col_) {
      double win_score = (double)r.get_int(win_col_);
      double lose_score = (double)r.get_int(lose_col_);
      double total_score = (double)r.get_int(total_col_);
      if (win_score == 0 || total_score == 0) {
        return true;
      }
      double win_total = win_score / total_score;
      double lose_winner = lose_score / win_score;
      if (win_total > win_total_percent_ && win_total != 1) {
        win_total_percent_ = win_total;
      }
      if (lose_winner > lose_winner_percent_ && lose_winner != 1) {
        lose_winner_percent_ = lose_winner;
      }
    }

    return true;
  }

  void join_delete(Rower* other) {
    MultiColumnRower* other_cast = dynamic_cast<MultiColumnRower*>(other);
    if (other != nullptr) {
      if (other_cast->win_total_percent_ > win_total_percent_) {
        win_total_percent_ = other_cast->win_total_percent_;
      }
      if (other_cast->lose_winner_percent_ > lose_winner_percent_) {
        lose_winner_percent_ = other_cast->lose_winner_percent_;
      }
      delete other;
    }
  }

  Object* clone() {
    return new MultiColumnRower(win_col_, lose_col_, total_col_);
  }

  double get_win_total_percent_() {
    return win_total_percent_;
  }

  double get_lose_winner_percent_() {
    return lose_winner_percent_;
  }

};

/***
*  FilterIntColumnNoZero:
*  Filters out all rows in dataframe where the value of the given int column index is 0
*  If column at given index is out of bounds or not an int column filters row out
*/
class FilterIntColumnNoZero : public Rower {
public:

  size_t col_;

  FilterIntColumnNoZero(int col){
    col_ = col;
  }

  ~FilterIntColumnNoZero() {
    col_ = 0;
  }

  bool accept(Row& r) {
    if (r.width() > col_) {
      if (r.col_type(col_) == 'I') {
        return r.get_int(col_) != 0;
      }
    }
    return false;
  }

  void join_delete(Rower* other) {

  }

};
