//
// Created by eyal on 24/11/19.
//

#ifndef EX2_EX2_H
#define EX2_EX2_H

#endif //EX2_EX2_H

#include <iostream>
#include <string>
#include <list>
#include "unordered_map"
#include "iterator"
#include "list"

using namespace std;

template <class T> class CacheManager {
 private:
  unordered_map<string, pair<T, list<string>::iterator>> cacheMap;
  int size;
  list<string> orderKeys;

 public:
  void insert(string key, T obj) {

  }
  T get(string key) {

  }
  void foreach() {

  }

  CacheManager (int capacity) {
    this->size = capacity;
  }
};