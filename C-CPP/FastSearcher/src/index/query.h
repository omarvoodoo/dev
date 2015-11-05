
#ifndef __fast_searcher_index_query_h_included__
#define __fast_searcher_index_query_h_included__

#include "../config.h"
#include <string>

class Query {
protected:
    std::string field_name_;

public:
    Query(const char* field_name) : field_name_(field_name) {}

    virtual const char* getQueryName() = 0;
};

#endif /* __fast_searcher_index_query_h_included__ */
