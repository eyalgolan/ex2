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
#include "fstream"
#include <typeinfo>

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
    return (cacheMap.size() + 1 > size);
  }
  void updatePriority(string key, T obj) {
    orderKeys.remove(key);
    orderKeys.push_front(key);
    this->cacheMap.erase(key);
    list<string>::iterator order = orderKeys.begin();
    cacheMap.insert(pair<string, pair<T, list<string>::iterator>>(key, pair<T, list<string>::iterator>(obj, order)));
  }
  void writeToFile(string key, T obj) {
    string filename = "../" + key + typeid(obj).name() + ".txt";
    fstream out_file {filename,ios::out | ios::binary};                         //write to file BINARY
    out_file.write((char *)&obj, sizeof(obj));
  }
  void createItem(string key, T obj) {
    orderKeys.push_front(key);
    list<string>::iterator order = orderKeys.begin();
    cacheMap.insert(pair<string, pair<T, list<string>::iterator>>(key, pair<T, list<string>::iterator>(obj, order)));
    writeToFile(key, obj);                                                      //write to file BINARY
  }
  void removeLastFromCache() {
    string key = orderKeys.back();
    cacheMap.erase(key);
    orderKeys.remove(key);
  }
  void insert(string key, T obj) {
    if(isInCache(key)) {                                                        //if already in cache -> only need to update priority
      updatePriority(key, obj);
    }
    else{                                                                       //if not in cache
      if(sizeIsOk()) {                                                          //and if cache is smaller then limit -> insert new item
        createItem(key, obj);
      }
      else {                                                                    //if not in cache
        writeToFile(key, obj);                                                  //write to file (even if in file, override)
        updatePriority(key, obj);                                               //update priorities
        removeLastFromCache();                                                  //remove last from cache
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