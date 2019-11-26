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
  bool isInCache(string key) {
    return (this->cacheMap.find(key) != this->cacheMap.end());
  }
  bool sizeIsOk() {
    return (cacheMap.size() + 1 < size);
  }
  void updatePriority(string key, T obj) {
    orderKeys.remove(key);
    orderKeys.push_front(key);
    this->cacheMap.erase(key);
    list<string>::iterator order = orderKeys.begin();
    cacheMap.insert(pair<string, pair<T, list<string>::iterator>>(key, pair<T, list<string>::iterator>(obj, order)));
  }
  bool isInFile(string key) {

  }
  void insert(string key, T obj) {
    if(isInCache(key)) {
      updatePriority(key, obj);
    }
    else{
      if(sizeIsOk()) {
        list<string>::iterator order = orderKeys.begin();
        cacheMap.insert(pair<string, pair<T, list<string>::iterator>>(key, pair<T, list<string>::iterator>(obj, order)));
      }
      else {
        if(isInFile(key)) {

        }
      }
    }
  }
//  T get(string key) {
//
//  }
//
//  void foreach() {
//
//  }

  CacheManager (int capacity) {
    this->size = capacity;
  }
};