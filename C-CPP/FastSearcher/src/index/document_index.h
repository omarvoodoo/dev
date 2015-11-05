
#ifndef __fast_searcher_index_document_index_h_included__
#define __fast_searcher_index_document_index_h_included__

#include "../config.h"
#include "field\field.h"
#include "document.h"
#include "boolean_query.h"
#include <unordered_map>

typedef std::unordered_map<int64_t, char*> DocStreamListType;
typedef std::vector<Field*> FieldListType;
typedef std::unordered_map<std::string, uint8_t> FieldNameListType;

class DocumentIndex {
    FieldNameListType field_name_list_;
    DocStreamListType doc_stream_list_;

    // 주어진 필드 목록을 가지고 스트림 버퍼 형태로 만든다.
    char* toStream(FieldListType& field_list);

    // 원본 데이터에서 필드를 추출한다.
    void extractFields(char* raw_data, FieldListType& field_list);

    // str의 형식을 확인하여 반환한다.
    Field::Type valueType(const char* str, int32_t str_len);
public:
    // 원본 데이터를 색인한다.
    void add(int64_t doc_id, char* raw_data);

    // 검색한다.
    // argv는 검색 조건이다. 비교 연산, 정렬, 통계 등을 처리한다.
    void search(BooleanQuery* query, DocIdListType& out_doc_id_list, void* argv);
};

#endif /* __fast_searcher_index_document_index_h_included__ */
