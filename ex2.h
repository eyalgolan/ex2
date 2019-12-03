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
  unsigned long size;
  list<string> orderKeys;

 public:

  bool isInCache(string key) {                                                  //check if item in cache
    return (this->cacheMap.find(key) != this->cacheMap.end());
  }

  bool isInFile(string key) {
    typename unordered_map<string, pair<T, list<string>::iterator>>::iterator it
        = cacheMap.begin();
    T obj = it->second.first;
    string filename = "../" + key + obj.class_name;
    ifstream ifile;
    ifile.open(filename, ios::in | ios::binary);
    if(ifile.fail()) {
      return false;
    }
    ifile.close();
    return true;
  }

  bool sizeIsOk() {                                                             //check if cache not full
    return (cacheMap.size() + 1 <= size);
  }

  void updatePriority(string key, T obj) {
    orderKeys.remove(key);                                                      //remove key from LRU list
    orderKeys.push_front(key);                                                  //push key to start of LRU list
    this->cacheMap.erase(key);                                                  //erase it from cache
    list<string>::iterator order = orderKeys.begin();                           //get pointer to start of LRU list
    cacheMap.insert(pair<string, pair<T, list<string>::iterator>>(key,
        pair<T, list<string>::iterator>(obj, order)));                          //enter to cache, with pointer to start of LRU list
  }

  void writeToFile(string key, T obj) {                                         //write to file BINARY
    string filename = "../" + key + obj.class_name;                             //build filename
    fstream out_file {filename,ios::out | ios::binary};                         //create file
    if(!out_file) {                                                             //check if file not created properly
      throw "an error";
    }
    else {
      out_file.write((char *)&obj, sizeof(obj));                                //write data to file
      out_file.close();                                                         //close the file
    }
  }

  void createItem(string key, T obj) {
    orderKeys.push_front(key);                                                  //push key to start of LRU list
    list<string>::iterator order = orderKeys.begin();
    cacheMap.insert(pair<string, pair<T, list<string>::iterator>>(key,
        pair<T, list<string>::iterator>(obj, order)));                          //enter to cache
    writeToFile(key, obj);                                                      //write to file BINARY
  }

  void removeLastFromCache() {
    string key = orderKeys.back();                                              //get key of last member in LRU
    cacheMap.erase(key);                                                        //erase it from cache
    orderKeys.remove(key);                                                      //erase it from LRU list
  }

  T getObjFromCache(string key) {
    typename unordered_map<string, pair<T, list<string>::iterator>>::iterator it
        = cacheMap.find(key);
    return it->second.first;
  }

  T getObjFromFile(string key) {
    typename unordered_map<string, pair<T, list<string>::iterator>>::iterator it = cacheMap.begin();
    T obj = it->second.first;
    string filename = "../" + key + obj.class_name;

    ifstream ifile;
    ifile.open(filename, ios::in | ios::binary);
    T retrievedObj;
    ifile.read((char *)&retrievedObj, sizeof(retrievedObj));
    ifile.close();
    return retrievedObj;
  }

 public:

  void insert(string key, T obj) {
    if(isInCache(key)) {                                                        //if already in cache -> only need to update priority
      updatePriority(key, obj);                                                 //update priority
      writeToFile(key, obj);                                                    //update file (obj might be different)
    }
    else{                                                                       //if not in cache
      if(sizeIsOk()) {                                                          //and if cache is smaller then limit -> insert new item
        createItem(key, obj);                                                   //insert new item
      }
      else {                                                                    //if not in cache
        writeToFile(key, obj);                                                  //write to file (even if in file, override)
        updatePriority(key, obj);                                               //update priorities
        removeLastFromCache();                                                  //remove last from cache
      }
    }
  }
  T get(string key) {
    if(isInCache(key)) {
      T obj = getObjFromCache(key);
      updatePriority(key, obj);
      return obj;
    }
    else if(isInFile(key)) {
      T obj = getObjFromFile(key);
      updatePriority(key, obj);
      removeLastFromCache();
      return obj;
    }
    else {
      throw "an error";
    }
  }

  template <class p>
  void foreach(p func) {
    if(cacheMap.size() != 0) {
      unsigned long lruSize = this->orderKeys.size();
      typename list<string>::iterator it = this->orderKeys.begin();
      for(unsigned long i=0 ; i< lruSize ; i++) {
        T obj = getObjFromCache(*it);
        func(obj);
        it++;
      }
    }
  }

  CacheManager (unsigned long capacity) {                                       //CacheManager CTOR
    this->size = capacity;
  }
};