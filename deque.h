#include <iostream>
#include <type_traits>

static const size_t kInnerSize = 8;

template<typename T>
class DequeIterator;


/// ################################################################################
/// #############################  Dequeue Declaration #############################
/// ################################################################################


template<typename T>
struct Deque {
private:
  static const size_t kDefaultOuterSize = 2;

  size_t outer_size;
  T** outer;
  size_t start;
  size_t finish;

  void Expand();

public:
  typedef DequeIterator<T> iterator;
  typedef DequeIterator<const T> const_iterator;

  Deque();

  Deque(const Deque& other);

  Deque(int quantity);

  Deque(int quantity, const T& element);

  Deque& operator=(const Deque& other);

  size_t size() const;

  void push_back(const T& value);

  void push_front(const T& value);

  void pop_back();

  void pop_front();

  T& operator[](size_t pos);

  const T& operator[](size_t pos) const;

  T& at(size_t pos);

  const T& at(size_t pos) const;

  DequeIterator<T> begin();

  DequeIterator<T> end();

  DequeIterator<const T> begin() const;

  DequeIterator<const T> end() const;

  DequeIterator<const T> cbegin() const;

  DequeIterator<const T> cend() const;

  DequeIterator<T> rbegin();

  DequeIterator<T> rend();

  DequeIterator<const T> rbegin() const;

  DequeIterator<const T> rend() const;

  DequeIterator<const T> crbegin() const;

  DequeIterator<const T> crend() const;

  void insert(const DequeIterator<T>& it, const T& elem);

  void erase(const DequeIterator<T>& it);

  ~Deque();

  friend DequeIterator<T>& DequeIterator<T>::operator++();

  friend DequeIterator<T>& DequeIterator<T>::operator--();

  friend void DequeIterator<T>::Increase();

  friend void DequeIterator<T>::Decrease();
};


/// ########################################################################################
/// #############################  DequeueIterator Declaration #############################
/// ########################################################################################


template<typename T>
class DequeIterator {
  size_t position;
  T** outer_pointer;
  bool is_reverse;

  void Increase();

  void Decrease();

  DequeIterator(size_t pos, T** outer, bool rev);

public:
  using value_type = T;
  using difference_type = int;
  using iterator_category = std::random_access_iterator_tag;
  using pointer = value_type*;
  using reference = value_type&;

  operator DequeIterator<const T>() {
    return DequeIterator<const T>(position, outer_pointer, is_reverse);
  }

  DequeIterator& operator++();

  DequeIterator& operator--();

  DequeIterator operator++(int);

  DequeIterator operator--(int);

  DequeIterator& operator+=(int value);

  DequeIterator operator-=(int value);

  DequeIterator operator+(int value) const;

  DequeIterator operator-(int value) const;

  int operator-(const DequeIterator& other) const;

  int operator-(const DequeIterator<const typename std::remove_const<T>>& other) const;

  bool operator==(const DequeIterator& other) const;

  bool operator<(const DequeIterator& other) const;

  bool operator!=(const DequeIterator& other) const;

  bool operator>=(const DequeIterator& other) const;

  bool operator>(const DequeIterator& other) const;

  bool operator<=(const DequeIterator& other) const;

  T& operator*();

  const T& operator*() const;

  T* operator->();

  const T* operator->() const;

  friend struct Deque<T>;

  friend struct Deque<typename std::remove_const<T>::type>;
};


/// ################################################################################
/// #############################  Dequeue Realization #############################
/// ################################################################################


template<typename T>
void Deque<T>::Expand() {
  auto destination = new T*[outer_size * 2];
  for (size_t i = 0; i < outer_size / 2; ++i) {
    destination[i] = reinterpret_cast<T*>(new char[sizeof(T) * kInnerSize]);
  }
  for (size_t i = outer_size / 2; i <= outer_size / 2 + finish / kInnerSize - start / kInnerSize; ++i) {
    destination[i] = outer[i - outer_size / 2 + start / kInnerSize];
  }
  for (size_t i = outer_size / 2 + finish / kInnerSize - start / kInnerSize + 1; i < outer_size * 2; ++i) {
    destination[i] = reinterpret_cast<T*>(new char[sizeof(T) * kInnerSize]);
  }
  finish = (outer_size / 2 + finish / kInnerSize - start / kInnerSize) * kInnerSize + finish % kInnerSize;
  start = (outer_size / 2) * kInnerSize + start % kInnerSize;
  delete[] outer;
  outer = destination;
  outer_size *= 2;
}


template<typename T>
Deque<T>::Deque() : outer_size(kDefaultOuterSize), outer(new T*[outer_size]), start(kInnerSize * kDefaultOuterSize / 2), finish(start - 1){
  for (size_t i = 0; i < outer_size; ++i) {
    outer[i] = reinterpret_cast<T*>(new char[sizeof(T) * kInnerSize]);
  }
}

template<typename T>
Deque<T>::Deque(const Deque& other) : outer_size(other.outer_size), outer(new T*[outer_size]), start(other.start), finish(other.finish) {
  for (size_t i = 0; i < outer_size; ++i) {
    outer[i] = reinterpret_cast<T*>(new char[sizeof(T) * kInnerSize]);
  }
  size_t pos;
  try {
    for (pos = start; pos <= finish; ++pos) {
      new(outer[pos / kInnerSize] + pos % kInnerSize) T(other[pos - start]);
    }
  } catch (...) {
    finish = pos;
    this->~Deque();
  }
}

template<typename T>
Deque<T>::Deque(int quantity) : outer_size((quantity / kInnerSize + 1) * 2), outer(new T*[outer_size]), start(kInnerSize * kDefaultOuterSize / 2), finish(start + quantity - 1){
  for (size_t i = 0; i < outer_size; ++i) {
    outer[i] = reinterpret_cast<T*>(new char[sizeof(T) * kInnerSize]);
  }
  size_t pos;
  try {
    for (pos = start; pos <= finish; ++pos) {
      new(outer[pos / kInnerSize] + pos % kInnerSize) T();
    }
  } catch (...) {
    finish = pos - 1;
  }
}

template<typename T>
Deque<T>::Deque(int quantity, const T& element) : outer_size((quantity / kInnerSize + 1) * 2), outer(new T*[outer_size]), start(kInnerSize * kDefaultOuterSize / 2), finish(start + quantity - 1){
  for (size_t i = 0; i < outer_size; ++i) {
    outer[i] = reinterpret_cast<T*>(new char[sizeof(T) * kInnerSize]);
  }
  size_t pos;
  try {
    for (pos = start; pos <= finish; ++pos) {
      new(outer[pos / kInnerSize] + pos % kInnerSize) T(element);
    }
  } catch (...) {
    finish = pos;
  }
}

template<typename T>
Deque<T>& Deque<T>::operator=(const Deque& other) {
  T** buffer = outer;
  outer = new T*[other.outer_size];
  for (size_t i = 0; i < other.outer_size; ++i) {
    outer[i] = reinterpret_cast<T*>(new char[sizeof(T) * kInnerSize]);
  }
  size_t pos;
  try {
    for (pos = other.start; pos <= other.finish; ++pos) {
      new(outer[pos / kInnerSize] + pos % kInnerSize) T(other[pos - other.start]);
    }
    for (size_t i = start; i <= finish; ++i) {
      buffer[i / kInnerSize][i % kInnerSize].~T();
    }
    for (size_t i = 0; i < outer_size; ++i) {
      delete[] reinterpret_cast<char*>(buffer[i]);
    }
    delete[] buffer;
    start = other.start;
    finish = other.finish;
    outer_size = other.outer_size;
  } catch (...) {
    size_t saved = finish;
    finish = pos;
    this->~Deque();
    finish = saved;
    outer = buffer;
  }
  return *this;
}

template<typename T>
size_t Deque<T>::size() const { return finish - start + 1; }

template<typename T>
void Deque<T>::push_back(const T& value) {
  if (finish == outer_size * kInnerSize - 1) {
    Expand();
  }
  try {
    ++finish;
    new(outer[finish / kInnerSize] + finish % kInnerSize) T(value);
  } catch (...) {
    --finish;
  }
}

template<typename T>
void Deque<T>::push_front(const T& value) {
  if (start == 0) {
    Expand();
  }
  try {
    --start;
    new(outer[start / kInnerSize] + start % kInnerSize) T(value);
  } catch (...) {
    ++start;
  }
}

template<typename T>
void Deque<T>::pop_back() {
  outer[finish / kInnerSize][finish % kInnerSize].~T();
  --finish;
}

template<typename T>
void Deque<T>::pop_front() {
  outer[start / kInnerSize][start % kInnerSize].~T();
  ++start;
}

template<typename T>
T& Deque<T>::operator[](size_t pos) {
  pos += start;
  return outer[pos / kInnerSize][pos % kInnerSize];
}

template<typename T>
const T& Deque<T>::operator[](size_t pos) const {
  pos += start;
  return outer[pos / kInnerSize][pos % kInnerSize];
}

template<typename T>
T& Deque<T>::at(size_t pos) {
  if (pos < 0 or pos >= size()) {
    throw std::out_of_range("Deque index out of range");
  }
  pos += start;
  return outer[pos / kInnerSize][pos % kInnerSize];
}

template<typename T>
const T& Deque<T>::at(size_t pos) const {
  if (pos < start or pos >= finish) {
    throw std::out_of_range("Deque index out of range");
  }
  pos += start;
  return outer[pos / kInnerSize][pos % kInnerSize];
}

template<typename T>
DequeIterator<T> Deque<T>::begin() { return DequeIterator<T>(start, outer + (start / kInnerSize), false); }

template<typename T>
DequeIterator<T> Deque<T>::end() { return DequeIterator<T>((finish + 1), outer + ((finish + 1) / kInnerSize), false); }

template<typename T>
DequeIterator<const T> Deque<T>::begin() const { return DequeIterator<const T>(start, const_cast<const T**>(outer + (start / kInnerSize)), false); }

template<typename T>
DequeIterator<const T> Deque<T>::end() const { return DequeIterator<const T>((finish + 1), const_cast<const T**>(outer + ((finish + 1) / kInnerSize)), false); }

template<typename T>
DequeIterator<const T> Deque<T>::cbegin() const { return DequeIterator<const T>(start, const_cast<const T**>(outer + (start / kInnerSize)), false); }

template<typename T>
DequeIterator<const T> Deque<T>::cend() const { return DequeIterator<const T>((finish + 1), const_cast<const T**>(outer + ((finish + 1) / kInnerSize)), false); }

template<typename T>
DequeIterator<T> Deque<T>::rbegin() { return DequeIterator<T>(finish, outer + (finish / kInnerSize), true); }

template<typename T>
DequeIterator<T> Deque<T>::rend() { return DequeIterator<T>((start - 1), outer + ((start - 1) / kInnerSize), true); }

template<typename T>
DequeIterator<const T> Deque<T>::rbegin() const { return DequeIterator<const T>(finish, const_cast<const T**>(outer + (finish / kInnerSize)), true); }

template<typename T>
DequeIterator<const T> Deque<T>::rend() const { return DequeIterator<const T>((start - 1), const_cast<const T**>(outer + ((start - 1) / kInnerSize)), true); }

template<typename T>
DequeIterator<const T> Deque<T>::crbegin() const { return DequeIterator<const T>(finish, const_cast<const T**>(outer + (finish / kInnerSize), true)); }

template<typename T>
DequeIterator<const T> Deque<T>::crend() const { return DequeIterator<const T>((start - 1), const_cast<const T**>(outer + ((start - 1) / kInnerSize)), true); }

template<typename T>
void Deque<T>::insert(const DequeIterator<T>& it, const T& elem) {
  if (it == end()) {
    push_back(elem);
    return;
  }
  auto x = *rbegin();
  push_back(x);
  DequeIterator forward = --(--end());
  DequeIterator backward = --end();
  while (backward != it) {
    *backward = *forward;
    --backward;
    --forward;
  }
  *backward = elem;
}

template<typename T>
void Deque<T>::erase(const DequeIterator<T>& it) {
  DequeIterator forward = DequeIterator(it.position, it.outer_pointer, false);
  forward.is_reverse = false;
  DequeIterator backward = forward;
  ++forward;
  DequeIterator stop = end();
  while (forward != stop) {
    *backward = *forward;
    ++backward;
    ++forward;
  }
  pop_back();
}

template<typename T>
Deque<T>::~Deque() {
  if (finish >= start) {
    for (size_t j = 0; j <= finish - start; ++j) {
      (*this)[j].~T();
    }
  }
  for (size_t i = 0; i < outer_size; ++i) {
    delete[] reinterpret_cast<char*>(outer[i]);
  }
  delete[] outer;
}


/// ########################################################################################
/// #############################  DequeueIterator Realization #############################
/// ########################################################################################


template<typename T>
void DequeIterator<T>::Increase() {
  if (position % kInnerSize == kInnerSize - 1) {
    ++outer_pointer;
  }
  ++position;
}

template<typename T>
void DequeIterator<T>::Decrease() {
  if (position % kInnerSize == 0) {
    --outer_pointer;
  }
  --position;
}

template<typename T>
DequeIterator<T>::DequeIterator(size_t pos, T** outer, bool rev) : position(pos), outer_pointer(outer), is_reverse(rev) {}

// template<typename T>
// DequeIterator<T>::DequeIterator(const DequeIterator& other) : position(other.position), outer_pointer(other.outer_pointer), is_reverse(other.is_reverse) {}

template<typename T>
DequeIterator<T>& DequeIterator<T>::operator++() {
  if (!is_reverse) {
    Increase();
  } else {
    Decrease();
  }
  return *this;
}

template<typename T>
DequeIterator<T>& DequeIterator<T>::operator--() {
  if (!is_reverse) {
    Decrease();
  } else {
    Increase();
  }
  return *this;
}

template<typename T>
DequeIterator<T> DequeIterator<T>::operator++(int) {
  DequeIterator buff(*this);
  ++(*this);
  return buff;
}

template<typename T>
DequeIterator<T> DequeIterator<T>::operator--(int) {
  DequeIterator buff(*this);
  --(*this);
  return buff;
}

template<typename T>
DequeIterator<T>& DequeIterator<T>::operator+=(int value) {
  if (value == 0) {
    return *this;
  }
  if ((value > 0 && !is_reverse) || (value < 0 && is_reverse)) {
    if (value < 0) {
      value = -value;
    }
    outer_pointer += value / kInnerSize;
    if (value % kInnerSize + position % kInnerSize >= kInnerSize) {
      ++outer_pointer;
    }
    position += value;
  } else {
    if (value < 0) {
      value = -value;
    }
    outer_pointer -= value / kInnerSize;
    if (position % kInnerSize - value % kInnerSize >= kInnerSize) {
      --outer_pointer;
    }
    position -= value;
  }
  return *this;
}

template<typename T>
DequeIterator<T> DequeIterator<T>::operator-=(int value) { return *this += -value; }

template<typename T>
DequeIterator<T> DequeIterator<T>::operator+(int value) const {
  DequeIterator buff(*this);
  buff += value;
  return buff;
}

template<typename T>
DequeIterator<T> DequeIterator<T>::operator-(int value) const { return *this + -value; }

template<typename T>
int DequeIterator<T>::operator-(const DequeIterator& other) const {
  int buff = position - other.position;
  if (is_reverse) {
    buff *= -1;
  }
  return buff;
}

template<typename T>
int DequeIterator<T>::operator-(const DequeIterator<const typename std::remove_const<T>>& other) const {
  int buff = position - other.position;
  if (is_reverse) {
    buff *= -1;
  }
  return buff;
}

template<typename T>
bool DequeIterator<T>::operator==(const DequeIterator& other) const { return (position == other.position); }

template<typename T>
bool DequeIterator<T>::operator<(const DequeIterator& other) const { return (position < other.position) ^ (is_reverse); }

template<typename T>
bool DequeIterator<T>::operator!=(const DequeIterator& other) const { return !(*this == other); }

template<typename T>
bool DequeIterator<T>::operator>=(const DequeIterator& other) const { return !(*this < other); }

template<typename T>
bool DequeIterator<T>::operator>(const DequeIterator& other) const { return (*this != other && *this >= other); }

template<typename T>
bool DequeIterator<T>::operator<=(const DequeIterator& other) const { return (*this == other || *this < other); }

template<typename T>
T& DequeIterator<T>::operator*() { return (*outer_pointer)[position % kInnerSize]; }

template<typename T>
const T& DequeIterator<T>::operator*() const { return (*outer_pointer)[position % kInnerSize]; }

template<typename T>
T* DequeIterator<T>::operator->() { return &(*outer_pointer)[position % kInnerSize]; }

template<typename T>
const T* DequeIterator<T>::operator->() const { return &(*outer_pointer)[position % kInnerSize]; }