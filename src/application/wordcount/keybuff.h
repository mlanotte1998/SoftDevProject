#include "../../store/key.h"
#include "../../utility-classes/string.h"

/* author: jv */
class KeyBuff : public Object {
  public:
  Key* orig_; // external
  StrBuff buf_;


  KeyBuff(Key* orig) : orig_(orig), buf_(*orig) {}
  KeyBuff(const char* name) : orig_(new Key("ignore")), buf_(name) {}
  
  KeyBuff& c(String &s) { buf_.c(s); return *this;  }
  KeyBuff& c(size_t v) { buf_.c(v); return *this; }
  KeyBuff& c(const char* v) { buf_.c(v); return *this; }


  Key* get() {   
    String* s = buf_.get();
    buf_.c(orig_->c_str());
    Key* k = new Key(s->steal(), orig_->home());
    delete s;
    return k;
  } 
}; // KeyBuff
