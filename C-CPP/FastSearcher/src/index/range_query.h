
#ifndef __fast_searcher_index_range_query_h_included__
#define __fast_searcher_index_range_query_h_included__

#include "query.h"

class RangeQuery : public Query {
    int64_t from_value_;
    int64_t to_value_;
public:
    RangeQuery(const char* field_name, int64_t from_value, int64_t to_value)
        : Query(field_name), from_value_(from_value), to_value_(to_value) {

    }

    virtual const char* getQueryName() { return "RangeQuery"; }
};

#endif /* __fast_searcher_index_range_query_h_included__ */
