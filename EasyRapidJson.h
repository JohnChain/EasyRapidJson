#ifndef __EASY_RAPID_JSON__
#define __EASY_RAPID_JSON__

#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"
#include "rapidjson/reader.h"
#include "rapidjson/prettywriter.h" // for stringify JSON
#include "rapidjson/stringbuffer.h"
 
#include <iostream>
#include <fstream>
class EasyRapidJson {
public:
	static int  Load(const std::string &jsonfile, rapidjson::Document &doc);
	static int  Parse(rapidjson::Document &doc, const std::string &stringFromStream);

    // insert if not exist
    // update if exit
	static int  Insert(rapidjson::Document &doc, const std::string &key, rapidjson::Value &value, bool updateIfExist = true);
	static int  Insert(rapidjson::Value &parentNode, rapidjson::Document::AllocatorType& allocator, const std::string &key, rapidjson::Value &value);
	static int  HasMember(const rapidjson::Value &parentNode, const std::string &key);
	static int  HasMember(const rapidjson::Document &parentNode, const std::string &key);
	static int  AddStringMember(rapidjson::Value &parentNode, rapidjson::Document::AllocatorType& allocator, const std::string &key, const std::string &value);

    template <typename T>
    static int  AddNumMember(rapidjson::Value &parentNode, rapidjson::Document::AllocatorType& allocator, const std::string &key, const T value);

	static int  Dump(rapidjson::Document &doc, std::string &outString);
	static int  ReLoad(const std::string &jsonfile, rapidjson::Document &doc);
private:
	EasyRapidJson(){};
	~EasyRapidJson(){};
};

// define in head file, otherwise build fail....
template <typename T>
int  EasyRapidJson::AddNumMember(rapidjson::Value &parentNode, rapidjson::Document::AllocatorType& allocator, const std::string &key, const T value){
    if (parentNode.IsObject()) {
        rapidjson::Value Key(rapidjson::kStringType);
        Key.SetString(key.c_str(), allocator);
        parentNode.AddMember(Key, value, allocator);
    }else if (parentNode.IsArray()){
        parentNode.PushBack(value, allocator);
    }else{
        printf("Invalid usage: AddMember to non-Object, key: %s\n", key.c_str());
        return -1; 
    }   

    return 0;
}

#endif
