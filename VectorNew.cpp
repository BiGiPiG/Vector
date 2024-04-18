#include "VectorNew.h"
#include <cstring>

Vector::Vector(const ValueType *rawArray, const size_t size, float coef) {
  _data = new ValueType[size];
  _size = size;
  _capacity = size;
  _multiplicativeCoef = coef;
  for (size_t i = 0; i != size; ++i) {
    _data[i] = rawArray[i];
  }
}

Vector::Vector(const Vector &obj) {
  if (this != &obj) {
    _data = new ValueType[obj._size];
    _size = obj._size;
    _capacity = obj._size;
    _multiplicativeCoef = obj._multiplicativeCoef;
    for (size_t i = 0; i < _size; ++i) {
      _data[i] = obj._data[i];
    }
  }
}

Vector &Vector::operator=(const Vector &obj) {
  if (this != &obj) {
    _data = new ValueType[obj._size];
    _size = obj._size;
    _capacity = obj._size;
    _multiplicativeCoef = obj._multiplicativeCoef;
    for (size_t i = 0; i != _size; ++i) {
      _data[i] = obj._data[i];
    }
  }
  return *this;
}

Vector::Vector(Vector &&obj) noexcept {
  if (_data != obj._data) {
    _data = obj._data;
    _size = obj._size;
    _capacity = obj._size;
    _multiplicativeCoef = obj._multiplicativeCoef;

    obj._data = nullptr;
    obj._size = 0;
    obj._capacity = 0;
  }
}

Vector &Vector::operator=(Vector &&obj) noexcept {
  if (this != &obj) {
    _data = obj._data;
    _size = obj._size;
    _capacity = obj._size;
    _multiplicativeCoef = obj._multiplicativeCoef;

    obj._data = nullptr;
    obj._size = 0;
    obj._capacity = 0;
  }
  return *this;
}

Vector::~Vector() { delete[] _data; }

void Vector::pushBack(const ValueType &value) {
  if (_size >= _capacity) {
    if (_capacity == 0) {
      _capacity = _multiplicativeCoef;
    }
    while (_size >= _capacity) {
      _capacity *= _multiplicativeCoef;
    }

    ValueType *tmp = new ValueType[_capacity];
    for (int i = 0; i < _size; i++) {
      tmp[i] = _data[i];
    }
    delete[] _data;
    _data = tmp;
  }
  _data[_size] = value;
  _size++;
}

void Vector::pushFront(const ValueType &value) {
  if (_capacity == 0) {
    _capacity = _multiplicativeCoef;
    _data = new ValueType[_capacity];
    _data[_size] = value;
  } else if (_capacity <= _size) {
    _capacity = _capacity * _multiplicativeCoef;
    ValueType *tmp = new ValueType[_capacity];
    std::memmove(tmp, _data, sizeof(ValueType) * _size);
    tmp[0] = value;
    delete[] _data;
    _data = tmp;
  }
  _size++;
}

void Vector::insert(const ValueType &value, size_t pos) {
  if (_size >= _capacity) {
    _capacity = int(_capacity * _multiplicativeCoef);
  }
  ValueType *tmp = new ValueType[_capacity];
  std::memmove(tmp, _data, sizeof(ValueType) * pos);
  tmp[pos] = value;
  for (size_t i = pos + 1; i <= _size; ++i) {
    tmp[i - 1] = _data[i];
  }
  _size++;
}

void Vector::insert(const ValueType *values, size_t size, size_t pos) {
  if (_size + size > _capacity) {
    _capacity = _size + size;
  }
  ValueType *tmp = new ValueType[_capacity];

  for (size_t i = 0; i < pos; ++i) {
    tmp[i] = _data[i];
  }
  for (size_t i = pos; i < pos + size; ++i) {
    tmp[i] = values[i - pos];
  }
  for (size_t i = pos + size; i <= _size + size; ++i) {
    tmp[i] = _data[i - size];
  }

  delete[] _data;
  _data = tmp;
  _size += size;
}

void Vector::insert(const Vector &vector, size_t pos) {
  if (vector._size + _size > _capacity) {
    _capacity = vector._size + _size;
  }
  ValueType *tmp = new ValueType[_capacity];
  for (size_t i = 0; i < pos; ++i) {
    tmp[i] = _data[i];
  }
  for (size_t i = pos; i < pos + vector._size; ++i) {
    tmp[i] = vector._data[i - pos];
  }
  for (size_t i = pos + vector._size; i < _size + vector._size; ++i) {
    tmp[i] = _data[i - vector._size];
  }
  delete[] _data;
  _size = _size + vector._size;
  _data = tmp;
}

void Vector::popBack() {
  if (_size > 0) {
    _size--;
  } else {
    throw "popb_from_empty_vec";
  }
}

void Vector::popFront() {
  if (_size > 0) {
    for (size_t i = 1; i < _size; ++i) {
      _data[i - 1] = _data[i];
    }
    _size--;
  } else {
    throw "popf_from_empty_vec";
  }
}

void Vector::erase(size_t pos, size_t count) {
  if (pos + count > _size) {
    ValueType *tmp = new ValueType[_capacity];
    for (size_t i = 0; i < pos; ++i) {
      tmp[i] = _data[i];
    }
    delete[] _data;
    _data = tmp;
    _size = pos;
  } else {
    for (size_t i = pos; i + count < _size; ++i) {
      _data[i] = _data[i + count];
    }
    _size -= count;
  }
}

void Vector::eraseBetween(size_t beginPos, size_t endPos) {
  if (beginPos < 0) {
    beginPos = 0;
  }
  if (endPos > _size) {
    endPos = _size;
  }
  // ValueType *tmp = new ValueType[_size - endPos + beginPos];
  // std::copy(_data, _data + beginPos, tmp);
  // std::copy(_data + endPos, _data + _size, tmp + beginPos);
  std::memcpy(&_data[beginPos], &_data[endPos], _size - endPos + beginPos);
  // delete[] _data;
  _size = _size - (endPos - beginPos);
}

size_t Vector::size() const { return _size; }

size_t Vector::capacity() const { return _capacity; }

double Vector::loadFactor() const { return double(_size / _capacity); }

ValueType &Vector::operator[](size_t idx) { return _data[idx]; }

const ValueType &Vector::operator[](size_t idx) const { return _data[idx]; }

long long Vector::find(const ValueType &value) const {
  long long res = -1;
  for (long long i = 0; i < _size; ++i) {
    if (_data[i] == value) {
      res = i;
      break;
    }
  }
  return res;
}

void Vector::reserve(size_t capacity) {
  if (capacity > _capacity) {
    ValueType *tmp = new ValueType[capacity];
    for (size_t i = 0; i < _size; ++i) {
      tmp[i] = _data[i];
    }
    delete[] _data;
    _data = tmp;
    _capacity = capacity;
  }
}

void Vector::shrinkToFit() {
  if (_capacity >= _size) {
    _capacity = _size;
    ValueType *tmp = new ValueType[_size];
    for (size_t i = 0; i < _size; ++i) {
      tmp[i] = _data[i];
    }
    delete[] _data;
    _data = tmp;
  }
}

Vector::Iterator::Iterator(ValueType *ptr) {
  if (!ptr) {
    _ptr = nullptr;
  } else {
    _ptr = ptr;
  }
}

ValueType &Vector::Iterator::operator*() { return *_ptr; }

const ValueType &Vector::Iterator::operator*() const { return *_ptr; }

ValueType *Vector::Iterator::operator->() { return _ptr; }

const ValueType *Vector::Iterator::operator->() const { return _ptr; }

Vector::Iterator Vector::Iterator::operator++() {
  ++_ptr;
  return *this;
}

Vector::Iterator Vector::Iterator::operator++(int) {
  Iterator tmp(_ptr);
  ++(*this);
  return tmp;
}

bool Vector::Iterator::operator==(const Iterator &other) const {
  return _ptr == other._ptr;
}

bool Vector::Iterator::operator!=(const Iterator &other) const {
  return _ptr != other._ptr;
}

Vector::Iterator Vector::begin() { return Iterator(&_data[0]); }

Vector::Iterator Vector::end() { return Iterator(&_data[_size - 1]); }
