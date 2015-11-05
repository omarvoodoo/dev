
#ifndef __fast_searcher_index_boolean_query_h_included__
#define __fast_searcher_index_boolean_query_h_included__

#include "query.h"
#include <vector>

enum OperationType {
    And=111, Or, Not, Null
};

struct QueryItem {
    Query* query_;
    OperationType   type_;
};

typedef std::vector<QueryItem> QueryListType;

class BooleanQuery : public Query { 
    QueryListType query_list_;

public:
    BooleanQuery() : Query("default") {}

    void add(Query* query, OperationType oper_type) {
        QueryItem qi = {query, oper_type};
        query_list_.push_back(qi);
    }

    void add(QueryItem qi) {
        query_list_.push_back(qi);
    }

    QueryListType& getQueryList() { return query_list_; }

    virtual const char* getQueryName() { return "BooleanQuery"; }
};

#endif /* __fast_searcher_index_boolean_query_h_included__ */
