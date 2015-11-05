
#include "inverted_index.h"
#include "term_query.h"
#include <iterator>

void InvertedIndex::OperAnd(DocIdListType& doc_id_list1, DocIdListType& doc_id_list2, DocIdListType& out_doc_id_list) {
    DocIdListType::iterator iter_list1 = doc_id_list1.begin();
    DocIdListType::iterator iter_list2 = doc_id_list2.begin();

    // doc_id_list1과 doc_id_list2는 각각 오름차순으로 정렬이 되어 있는 상태이다.
    while(iter_list1!=doc_id_list1.end() && iter_list2!=doc_id_list2.end()) {
        if(*iter_list1 == *iter_list2) {
            out_doc_id_list.push_back(*iter_list1);
            ++iter_list1;
            ++iter_list2;
        } else if(*iter_list1 > *iter_list2) {
            ++iter_list2;
        } else {
            ++iter_list1;
        }
    }
}

int InvertedIndex::search(BooleanQuery* query, DocIdListType& result_id_list) {
    DocIdListType left;
    QueryListType::iterator iter = query->getQueryList().begin();
    for(; iter!=query->getQueryList().end(); iter++) {
        Query* query = (*iter).query_;

        if(strcmp(query->getQueryName(), "TermQuery") == 0) {
            TermQuery* tq = (TermQuery*)query;
            IndexType::iterator it_iter = index_list_.find((char*)tq->getToken());
                
            if(it_iter==index_list_.end())
                continue;

            if(left.empty()) {
                left = it_iter->second;
            } else {
                DocIdListType out_doc_id_list;
                OperAnd(left, it_iter->second, out_doc_id_list);
                left = out_doc_id_list;
            }
        } else if(strcmp(query->getQueryName(), "BooleanQuery") == 0) {
            BooleanQuery* bq = (BooleanQuery*)query;
            QueryListType& query_list = bq->getQueryList();

            if(query_list.empty())
                continue;

            ////////////////// 여기 작업하자 ////////////









            QueryItem left_qi;
            BooleanQuery* bq = (BooleanQuery*)query;
            QueryListType::iterator sub_iter = bq->getQueryList().begin();
            for(; sub_iter!=bq->getQueryList().end(); sub_iter++) {
                Query* sub_query = (*iter).query_;
                if(strcmp(sub_query->getQueryName(), "TermQuery") == 0) {

                }
            }
        }
    }



    /*
    std::vector<std::unique_ptr<Keyword>>::iterator kl_iter = keyword_list.begin();
    IndexType::iterator iter;

    
    // Facility and GPON or DISCOVERED
    // --> Facility GPON DISCOVERED and or

    const size_t keyword_count = 5;
    const std::unique_ptr<Keyword> kl[keyword_count] = {
        std::unique_ptr<Keyword>(new Keyword(Keyword::Type::Value, "Facility")),
        std::unique_ptr<Keyword>(new Keyword(Keyword::Type::Value, "system")),
        std::unique_ptr<Keyword>(new Keyword(Keyword::Type::Value, "port")),
        std::unique_ptr<Keyword>(new Keyword(Keyword::Type::And)),
        std::unique_ptr<Keyword>(new Keyword(Keyword::Type::Or))
    };

    // keyword_list에서 값은 단어 대신 해당 단어를 포함한 문서 번호 목록을 가리키는 포인터가 되야한다.
    // 

    std::vector<DocIdListType> stack;
    for(int i=0; i<keyword_count; ++i) {
        if(kl[i]->GetType() == Keyword::Value) {
            IndexType::iterator iter = index_list_.find((char*)kl[i]->GetValue());

            // 문서 번호 목록을 스택에 넣어준다.
            if(iter != index_list_.end()) {
                stack.push_back(iter->second);
            } else {
                // 해당 키워드에 대해 색인 데이터가 없을경우 빈 값을 넣어준다.
                stack.push_back(DocIdListType());
            }
        } else {
            // 스택에서 두개의 값(문서 번호 목록)을 가져온다.
            DocIdListType doc_id_list1 = stack.back();
            stack.pop_back();

            DocIdListType doc_id_list2 = stack.back();
            stack.pop_back();

            // 연산을 수행하고 결과(문서 번호 목록)를 스택에 넣는다.
            DocIdListType out_doc_id_list;
            OperAnd(doc_id_list1, doc_id_list2, out_doc_id_list);
            stack.push_back(out_doc_id_list);
        }
    }

    if(!stack.empty()) {
        DocIdListType& doc_id_list = stack.back();
        std::copy(doc_id_list.begin(), doc_id_list.end(), std::back_inserter(result_id_list));

        int a = 0;
    }
    */


    return 0;
}
