
#ifndef __mecab_parser_h_included__
#define __mecab_parser_h_included__

#include <mecab.h>
#include <string>
#include <sstream>

using namespace std;

namespace test {

enum WordType {
	Hangul,		// 한글
	Special,	// 특수문자
	Other
};

class Feature {
public:
	std::string tag2_;
	std::string tag6_;
	Feature(std::string feature_str) {
		stringstream feature_stream(feature_str);
		string token;
		int count = 0;
		while(std::getline(feature_stream, token, ',')) {
			if(count == 2) {
				tag2_ = token;
			} else if(count == 6) {
				tag6_ = token.substr(0, token.find("/"));
			}
			count++;
		}
	}
};

class NodeInfo {
	int start_pos_;
	int end_pos_;
	std::string speech_;
	std::string surface_;
	std::string feature_str_;
	Feature* feature_;

public:
	NodeInfo() : feature_(nullptr) {}
	~NodeInfo() { if(feature_) delete feature_; }
	void SetPosition(int start_pos, int end_pos) { start_pos_=start_pos; end_pos_=end_pos; }
	void SetSurface(const char* surface, size_t len) { surface_ = string(surface, len); }
	void SetFeature(std::string feature_str) { 
		feature_str_ = feature_str; 
		speech_ = feature_str.substr(0, feature_str.find(","));
	}

	std::string& GetSpeech() { return speech_; }
	std::string& GetSurface() { return surface_; }
	std::string& GetTag2() { 
		if(feature_ == nullptr) {
			feature_ = new Feature(feature_str_);
		}

		return feature_->tag2_;
	}
	std::string& GetTag6() { 
		if(feature_ == nullptr) {
			feature_ = new Feature(feature_str_);
		}

		return feature_->tag6_;
	}
	int GetStartPosition() { return start_pos_; }
	int GetEndPosition() { return end_pos_; }
};

class MecabParser {
	bool is_init_;
	MeCab::Model* model_;
	MeCab::Tagger* tagger_;

	int  Init();
	bool IsInit() { return is_init_; }
public:
	MecabParser();	
	~MecabParser();

	// special_mode 사용안됨.
	// special_mode	true 이면, 특수문자와 인접한 한글 단어는 분석하지 않고 그대로 출력한다.


	int Parse(const char* src, char* out_buf, int out_buf_size);
	int Parse_old(const char* src, char* out_buf, int out_buf_size, bool special_mode=false);
	int Parse(const wchar_t* src, wchar_t* out_buf, int out_buf_size, bool special_mode=false);
	int ParseDetail(const char* src, char* out_buf, int out_buf_size);
	int ParseDetail(const wchar_t* src, wchar_t* out_buf, int out_buf_size);
};

}

#endif /* __mecab_parser_h_included__ */
