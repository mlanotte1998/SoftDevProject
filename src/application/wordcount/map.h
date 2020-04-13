#include "../../utility-classes/object.h"
#include "items.h"

/** A generic map class from Object to Object. Subclasses are responsibly of
 * making the types more specific.  author: jv */
class Map : public Object {
public:      
  size_t capacity_;
  // TODO this was not size of the map, but number of occupied item positions in the top level
  size_t size_ = 0;
  Items_* items_;  // owned

  Map() : Map(10) {}
  Map(size_t cap) {
    capacity_ = cap;
    items_ = new Items_[capacity_];
  }
  
  ~Map() { delete[] items_; }

  /** True if the key is in the map. */
  bool contains(Object& key)  { return items_[off_(key)].contains_(key); }
  
  /** Return the number of elements in the map. */
  size_t size()  {
		return size_;
  }

  size_t off_(Object& k) { return  k.hash() % capacity_; }
  
  /** Get the value.  nullptr is allowed as a value.  */
  Object* get_(Object &key) { return items_[off_(key)].get_(key); }

  /** Add item->val_ at item->key_ either by updating an existing Item_ or
   * creating a new one if not found.  */
  void set(Object &k, Object *v) {
    if (size_ >= capacity_)
			grow();
    size_ += items_[off_(k)].set_(k,v);
  }
  
  /** Removes element with given key from the map.  Does nothing if the
      key is not present.  */
  void erase(Object& k) {
    size_ -= items_[off_(k)].erase_(k);
  }
  
  /** Resize the map, keeping all Item_s. */
  void grow() {
		//LOG("Growing map from capacity " << capacity_);
		Map newm(capacity_ * 2);
		for (size_t i = 0; i < capacity_; i++) {
			size_t sz = items_[i].keys_.size();
			for (size_t j = 0; j < sz; j++) {
				Object* k = items_[i].keys_.get(j);
				Object* v = items_[i].vals_.get(j);
				newm.set(*k,v);
				// otherwise the values would get deleted (if the array's destructor was doing its job I found later:)
				items_[i].vals_.add(j, nullptr);
			}
		}
		delete[] items_;
		items_ = newm.items_;
		capacity_ = newm.capacity_;
		assert(size_ == newm.size_);
		newm.items_ = nullptr;
  } 
}; // Map
