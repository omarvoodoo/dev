
#include "source.h"
#include "document.h"
#include "field/field_filter.h"
#include <map>
#include <memory>

int Source::addDocument(char* raw_data) {    
    // 필드화 + 색인 처리
    document_index_.add(doc_count_, raw_data);

    // 단어에 대한 역색인 처리
    inverted_index_.add(doc_count_, raw_data);
    doc_count_++;
    
    return 0;
}

void Source::search(BooleanQuery* query) {
    DocIdListType primary_id_list;      // 1차 검색 결과를 담는다.
    DocIdListType secondary_id_list;    // 2차 검색 결과를 담는다.

    // 단어 단위 검색
    inverted_index_.search(query, primary_id_list);

    // 필드 단위 검색
    document_index_.search(query, secondary_id_list, nullptr);
    
    

    /*
    Loop(doc_id in result_id_list) {
        int64_t out_value;
        Document doc;
        doc.fromStream("<stream>", <stream_lend>);
        doc.getValue(field_no, out_value);
    }
    */


}

