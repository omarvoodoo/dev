
#ifndef __fast_searcher_inverted_index_h_included__
#define __fast_searcher_inverted_index_h_included__

#include "../config.h"
#include "token.h"
#include "keyword.h"
#include "document.h"
#include "standard_tokenizer.h"
#include "boolean_query.h"
#include <unordered_map>
#include <vector>

struct cmp_str {
    bool operator()(const char* str1, const char* str2) {
		for (; *str1 && *str1 == *str2; ++str1, ++str2) {}
		return (*str1 == *str2);
	}
};

struct my_hash {
    size_t operator()(const char* str) const {
        unsigned long hash = 0;
        int c;

        while (c = *str++)
            hash = c + (hash << 6) + (hash << 16) - hash;

        return hash;
    }
};

// Key값이 되는 Token 이름은 토큰화할때 할당된 이후 토큰이 
// 해제 될때에도 일부로 제거하지 않는다.
// 때문에, Key가 사용이 끝나면 메모리를 해제해 주어야 한다.
typedef std::unordered_map<char*, DocIdListType, my_hash, cmp_str> IndexType;

class InvertedIndex {
    IndexType index_list_;

    /* 
    이 클래스에서 토큰에 대한 <시작 포지션>과 <끝 포지션>을 관리한다.
    역색인 데이터를 파일에 쓰는 주체이기도 하다.
    파일에 쓰기 직전에 토큰의 메타데이터(포지션 관리)를 읽어와서 쓰기에 참고한다.
    */

    void OperAnd(DocIdListType& doc_id_list1, DocIdListType& doc_id_list2, DocIdListType& out_doc_id_list);

public:
    void add(int64_t doc_id, char* raw_data) {  
        // 토큰화
        Token* tmp = nullptr;
        Token* token = StandardTokenizer::tokenize(raw_data);

        while(token) {
            IndexType::iterator iter = index_list_.find(token->data());
            if(iter==index_list_.end()) {
                index_list_[token->data()] = DocIdListType();
            }
            index_list_[token->data()].push_back(doc_id);
            tmp = token;
            token = token->next();
            delete tmp;
        }
    }

    // 쌓여있는 역색인 데이터를 파일로 떨군다.
    void flush() {

        // token Key 메모리 해제
        IndexType::iterator iter = index_list_.begin();
        for(;iter!=index_list_.end();iter++)
            free(iter->first);
    }

    int search(BooleanQuery* query, DocIdListType& result_id_list);
};

#endif /* __fast_searcher_inverted_index_h_included__ */
