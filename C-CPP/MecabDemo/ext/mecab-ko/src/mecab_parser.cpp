
#include "mecab.h"
#include <sstream>
#include <iostream>
#include <string>
#include <vector>

#define CHECK(eval) if (! eval) { \
   const char *e = tagger_ ? tagger_->what() : MeCab::getTaggerError(); \
   std::cerr << "Exception:" << e << std::endl; \
   delete tagger_; \
   return -1; }

using namespace std;

namespace MeCab {

MecabParser::MecabParser() : is_init_(false) {
	Init();
}

MecabParser::~MecabParser() {
	MeCab::deleteTagger(tagger_);
	MeCab::deleteModel(model_);
}

int MecabParser::Init() {
	model_ = MeCab::createModel("-r ./mecabrc");
	tagger_ = model_->createTagger();
	CHECK(tagger_);

	is_init_ = true;

	return 0;
}

int MecabParser::Parse(const char* src, char* out_buf, int out_buf_size) {
	string str_out;
	std::vector<NodeInfo> node_info_list;

	if(!IsInit()) {
		return -1;
	}

	const MeCab::Node* prev_node = nullptr;
	const MeCab::Node* node = tagger_->parseToNode(src);
	if(!node) {
		return -1;
	}

	for(; node; node=node->next) {
		if(node->stat != MECAB_BOS_NODE && node->stat != MECAB_EOS_NODE) {
			NodeInfo node_info;

			int start_pos = (int)(node->surface-src);
			int end_pos   = (int)(node->surface-src+node->length);

			node_info.SetPosition(start_pos, end_pos);
			node_info.SetFeature(node->feature);
			node_info.SetSurface(node->surface, node->length);

#ifdef _DEBUG
			stringstream ss;

			ss << " --> " << node->feature
			   << " --> " << (int)(node->surface - src)
			   << " --> " << (int)(node->surface - src + node->length)
			   << " --> " << node->rcAttr
			   << " --> " << node->lcAttr
			   << " --> " << node->posid
			   << " --> " << (int)node->char_type
			   << " --> " << (int)node->stat
			   << " --> " << (int)node->isbest
			   << " --> " << node->alpha
			   << " --> " << node->beta
			   << " --> " << node->prob
			   << " --> " << node->cost << std::endl;

			WCHAR wszBuf[40960] = {0};
			MultiByteToWideChar(CP_UTF8, 0, ss.str().c_str(), -1, wszBuf, sizeof(wszBuf));
			OutputDebugStringW(wszBuf);
#endif

			node_info_list.push_back(node_info);
		}
	}

	for(int i=0; i<node_info_list.size(); ++i) {
		NodeInfo& node_info = node_info_list[i];

		string speech = node_info.GetSpeech();

		//bool continuity = false;	// 한글 어절 연속 됨을 표시함.
		if(!speech.empty()) {

			if( i != 0) {
				NodeInfo& prev_info = node_info_list[i-1];

				// 어절 사이에 구분자가 포함되어 있는지 체크하고
				// 포함되어 있다면, 구분 문자열을 출력한다.
				if(prev_info.GetEndPosition() != node_info.GetStartPosition()) {
					str_out.append(src+prev_info.GetEndPosition(), node_info.GetStartPosition()-prev_info.GetEndPosition());
				} else if ((prev_info.GetSpeech() == "NN" || prev_info.GetSpeech() == "VV" || prev_info.GetSpeech().find("VV+") != string::npos) &&
						   (node_info.GetSpeech() == "NN" || node_info.GetSpeech() == "VV" || node_info.GetSpeech().find("VV+") != string::npos) ) {
					str_out.append(" ");
				}
			}

			if(speech == "NN" || speech == "VV") {
				// 같은 어절의 앞뒤로 특수문자가 붙는다면
				// 분석 결과가 아닌 어절 원본을 출력한다.
/*
				for(int j=i; j>0; j--) {
					NodeInfo& prev_info = node_info_list[j-1];
					NodeInfo& cur_info  = node_info_list[j];

					if(prev_info.GetEndPosition() == cur_info.GetStartPosition()) {
						if (prev_info.GetSpeech().at(0) == 'S' && prev_info.GetSpeech() != "SL" && prev_info.GetSpeech() != "SH" && prev_info.GetSpeech() != "SN") {
							prev_info = node_info_list[i-1];
							str_out.append(src+node_info.GetStartPosition(), node_info.GetEndPosition()-node_info.GetStartPosition());
							goto next;
						} 
					} else {
						break;
					}
				}

				for(int j=i; j<(node_info_list.size()-1); j++) {
					NodeInfo& cur_info  = node_info_list[j];
					NodeInfo& next_info = node_info_list[j+1];

					if(cur_info.GetEndPosition() == next_info.GetStartPosition()) {
						if (next_info.GetSpeech().at(0) == 'S' && next_info.GetSpeech() != "SL" && next_info.GetSpeech() != "SH" && next_info.GetSpeech() != "SN") {
							if( i != 0 ) {
								NodeInfo& prev_info = node_info_list[i-1];
								str_out.append(src+node_info.GetStartPosition(), node_info.GetEndPosition()-node_info.GetStartPosition());
							} else {
								str_out.append(src, node_info.GetEndPosition());
							}

							goto next;
						}
					} else {
						break;
					}
				}
*/
				str_out.append(node_info.GetSurface());
next:			
				continue;
			} else if( speech.at(0) == 'S' ) {
				// 영문자 / 숫자 / 특수문자 등
				str_out.append(node_info.GetSurface());
			} else if( speech.find("VV+") != string::npos ) {
				// 같은 어절의 앞뒤로 특수문자가 붙는다면
				// 분석 결과가 아닌 어절 원본을 출력한다.
/*
				for(int j=i; j>0; j--) {
					NodeInfo& prev_info = node_info_list[j-1];
					NodeInfo& cur_info  = node_info_list[j];

					if(prev_info.GetEndPosition() == cur_info.GetStartPosition()) {
						if (prev_info.GetSpeech().at(0) == 'S' && prev_info.GetSpeech() != "SL" && prev_info.GetSpeech() != "SH" && prev_info.GetSpeech() != "SN") {
							prev_info = node_info_list[i-1];
							str_out.append(src+node_info.GetStartPosition(), node_info.GetEndPosition()-node_info.GetStartPosition());
							goto next3;
						} 
					} else {
						break;
					}
				}

				for(int j=i; j<(node_info_list.size()-1); j++) {
					NodeInfo& cur_info  = node_info_list[j];
					NodeInfo& next_info = node_info_list[j+1];

					if(cur_info.GetEndPosition() == next_info.GetStartPosition()) {
						if (next_info.GetSpeech().at(0) == 'S' && next_info.GetSpeech() != "SL" && next_info.GetSpeech() != "SH" && next_info.GetSpeech() != "SN") {
							if( i != 0 ) {
								NodeInfo& prev_info = node_info_list[i-1];
								str_out.append(src+node_info.GetStartPosition(), node_info.GetEndPosition()-node_info.GetStartPosition());
							} else {
								str_out.append(src, node_info.GetEndPosition());
							}

							goto next3;
						}
					} else {
						break;
					}
				}
*/
				string& tag6 = node_info.GetTag6();
				str_out.append(tag6.substr(0, tag6.find("/")));

next3:			
				continue;
			} else {
				// 같은 어절의 앞뒤로 특수문자가 붙는다면
				// 분석 결과가 아닌 어절 원본을 출력한다.
/*
				for(int j=i; j>0; j--) {
					NodeInfo& prev_info = node_info_list[j-1];
					NodeInfo& cur_info  = node_info_list[j];

					if(prev_info.GetEndPosition() == cur_info.GetStartPosition()) {
						if (prev_info.GetSpeech().at(0) == 'S' && prev_info.GetSpeech() != "SL" && prev_info.GetSpeech() != "SH" && prev_info.GetSpeech() != "SN") {
							prev_info = node_info_list[i-1];
							str_out.append(src+node_info.GetStartPosition(), node_info.GetEndPosition()-node_info.GetStartPosition());
							goto next2;
						} 
					} else {
						break;
					}
				}

				for(int j=i; j<(node_info_list.size()-1); j++) {
					NodeInfo& cur_info  = node_info_list[j];
					NodeInfo& next_info = node_info_list[j+1];

					if(cur_info.GetEndPosition() == next_info.GetStartPosition()) {
						if (next_info.GetSpeech().at(0) == 'S' && next_info.GetSpeech() != "SL" && next_info.GetSpeech() != "SH" && next_info.GetSpeech() != "SN") {
							if( i != 0 ) {
								NodeInfo& prev_info = node_info_list[i-1];
								str_out.append(src+node_info.GetStartPosition(), node_info.GetEndPosition()-node_info.GetStartPosition());
							} else {
								str_out.append(src, node_info.GetEndPosition());
							}

							goto next2;
						}
					} else {
						break;
					}
				}
*/

				// 나머지 형식에 대해서는 같은 어절에 "명사", "동사", "복합동사"를 포함하지 않는다면,
				// 그대로 출력한다.
				for(int j=i; j>0; j--) {
					NodeInfo& prev_info = node_info_list[j-1];
					NodeInfo& cur_info  = node_info_list[j];

					if(prev_info.GetEndPosition() == cur_info.GetStartPosition()) {
						if (prev_info.GetSpeech() == "NN" || prev_info.GetSpeech() == "VV" || prev_info.GetSpeech().find("VV+") != string::npos) {
							goto next2;
						} 
					} else {
						break;
					}
				}

				for(int j=i; j<(node_info_list.size()-1); j++) {
					NodeInfo& cur_info  = node_info_list[j];
					NodeInfo& next_info = node_info_list[j+1];

					if(cur_info.GetEndPosition() == next_info.GetStartPosition()) {
						if (next_info.GetSpeech() == "NN" || next_info.GetSpeech() == "VV" || next_info.GetSpeech().find("VV+") != string::npos) {
							goto next2;
						}
					} else {
						break;
					}
				}

				str_out.append(node_info.GetSurface());

next2:			
				continue;
			}
		}
	}

	if(str_out.empty())
		return -1;

	int len = str_out.size();

	if( out_buf_size > str_out.size() ) {
		memcpy(out_buf, str_out.c_str(), str_out.size());
		out_buf[str_out.size()] = '\0';
		return str_out.size();
	} else {
		memcpy(out_buf, str_out.c_str(), out_buf_size-2);
		out_buf[out_buf_size-1] = '\0';
		return out_buf_size-1;
	}
}

int MecabParser::Parse(const wchar_t* wstr_src, wchar_t* out_buf, int out_buf_size) {
	// Unicode -> utf8
	char strUtf8[40960] ={0,};
    char strOutBuf[40960];
	int nLen = WideCharToMultiByte(CP_UTF8, 0, wstr_src, wcslen(wstr_src), NULL, 0, NULL, NULL);
	WideCharToMultiByte (CP_UTF8, 0, wstr_src, wcslen(wstr_src), strUtf8, nLen, NULL, NULL);

	int size = Parse(strUtf8, strOutBuf, out_buf_size);

	// utf8 -> Unicode
	nLen = MultiByteToWideChar(CP_UTF8, 0, strOutBuf, size, NULL, NULL);
	MultiByteToWideChar(CP_UTF8, 0, strOutBuf, size, out_buf, nLen);

	if( out_buf_size > nLen ) {
        out_buf[nLen] = L'\0';
        return nLen;
    } else {
        out_buf[out_buf_size-1] = L'\0';
        return out_buf_size-1;
    }

	return nLen;
}

}
