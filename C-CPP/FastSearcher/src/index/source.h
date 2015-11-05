
#ifndef __fast_searcher_index_source_h_included__
#define __fast_searcher_index_source_h_included__

#include "../config.h"
#include "document.h"
#include "token.h"
#include "keyword.h"
#include "inverted_index.h"
#include "document_index.h"
#include "boolean_query.h"
#include <string>
#include <unordered_map>
#include <vector>
#include <memory>

class Source {
	std::string source_name_;    	
    InvertedIndex inverted_index_;
    DocumentIndex document_index_;
    int64_t doc_count_;

public:
	Source(std::string source_name) : source_name_(source_name), doc_count_(0) {}

    // 원본 데이터를 필드화와 토큰화를 거쳐 메모리에 등록한다.
    // 문서 등록 과정으로 볼 수 있다.
    int addDocument(char* raw_data);

    void search(BooleanQuery* query);
};

#endif /* __fast_searcher_index_source_h_included__ */
