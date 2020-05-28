#include "EasyRapidJson.h"

using namespace rapidjson;

int EasyRapidJson::Load(const std::string &jsonfile, rapidjson::Document &doc){
    int ret = 0;
    char buf[1024];
    std::string stringFromStream = "";
    FILE *fp = fopen(jsonfile.c_str(), "a+");
    if(!fp){
        printf("faile to open file:%s\n", jsonfile.c_str());
        return -1;
    }
    do{
        memset(buf, 0, 1024);
        ret = fread(buf, 1, 1024, fp);
        stringFromStream.append(buf);
    }while(ret > 0 && !feof(fp) && !ferror(fp));
    fclose(fp);
 
    return Parse(doc, stringFromStream);
}
int EasyRapidJson::Parse(rapidjson::Document &doc, const std::string &stringFromStream){
    doc.Parse<0>(stringFromStream.c_str());
    if (doc.HasParseError()) {
        doc.Parse<0>("{}");
        if(doc.HasParseError()){
            rapidjson::ParseErrorCode code = doc.GetParseError();
            printf("parse failed: code %d\n", code);
            return -1;
        }else{
            printf("revert to default empty json OK\n");
        }
    }
    return 0;
}

int EasyRapidJson::HasMember(const rapidjson::Value &parentNode, const std::string &key){
    return parentNode.HasMember(key.c_str());
}
int EasyRapidJson::HasMember(const rapidjson::Document &parentNode, const std::string &key){
    return parentNode.HasMember(key.c_str());
}
 
int EasyRapidJson::Insert(rapidjson::Document &parentNode, const std::string &key, rapidjson::Value &value, bool updateIfExist){
    int ret = 0;
    rapidjson::Document::AllocatorType& allocator = parentNode.GetAllocator();
    rapidjson::Value::MemberIterator itr = parentNode.FindMember(key.c_str());
    Value vKey(rapidjson::kStringType);
    vKey.SetString(key.c_str(), allocator);
    if (itr == parentNode.MemberEnd()){
        parentNode.AddMember(vKey, value, allocator);
    }else{
        if(updateIfExist){
            parentNode.RemoveMember(itr);
            parentNode.AddMember(vKey, value, allocator);
        }
    }

    return ret;
}

int  EasyRapidJson::Insert(rapidjson::Value &parentNode, rapidjson::Document::AllocatorType& allocator, const std::string &key, rapidjson::Value &value){
    Value vKey(rapidjson::kStringType);
    vKey.SetString(key.c_str(), allocator);
    if(value.IsObject()){
        Value vValue(kObjectType);
        //vValue.SetObject(value);
        parentNode.AddMember(vKey, value, allocator);
    }else if(value.IsArray()){
        Value vValue(kArrayType);
        //vValue.SetArray(value);
        parentNode.AddMember(vKey, value, allocator);
    }else{
        printf("unsupported insert operation, %s!\n", key.c_str());
        return -1;
    }
    return 0;
}

 
int EasyRapidJson::AddStringMember(Value &parentNode, Document::AllocatorType& allocator, const std::string &key, const std::string &value){
    if (parentNode.IsObject()) {
        Value Key(rapidjson::kStringType);
        Value StrValue(rapidjson::kStringType);
        Key.SetString(key.c_str(), allocator);
        StrValue.SetString(value.c_str(), allocator);

        parentNode.AddMember(Key, StrValue, allocator);
    }else{
        printf("Invalid usage: AddMember to non-Object\n");
        return -1;
    }

    return 0;
}

int EasyRapidJson::Dump(Document &doc, std::string &outString){
    StringBuffer buffer;
    PrettyWriter<StringBuffer> writer(buffer);
    doc.Accept(writer);
    outString = buffer.GetString();
    return 0;
}
 
int EasyRapidJson::ReLoad(const std::string &jsonfile, Document &doc){
	FILE* file = fopen(jsonfile.c_str(), "wb");
    if (file) {
        std::string outString;
        Dump(doc, outString);
        fputs(outString.c_str(), file);
        fclose(file);
    }else{
    	return -1;
    }
    return 0;	
}
 
//int main(int argc, char const *argv[])
//{
//	EasyRapidJson     m_test;
// 
//	m_test.read("test.json");
//	
//	m_test.AddMember("key","word7");
//	
//	m_test.ReLoad("test.json");
//	
//	return 0;
//}
