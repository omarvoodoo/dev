
#ifndef __fast_searcher_index_term_query_h_included__
#define __fast_searcher_index_term_query_h_included__

#include "query.h"

class TermQuery : public Query {
    std::string token_;

public:
    TermQuery(const char* token)
        : Query("default"), token_(token) {

    }

    TermQuery(const char* field_name, const char* token)
        : Query(field_name), token_(token) {

    }

    virtual const char* getQueryName() { return "TermQuery"; }

    const char* getToken() { return token_.c_str(); }
};

#endif /* __fast_searcher_index_term_query_h_included__ */
