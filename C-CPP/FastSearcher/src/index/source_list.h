
#ifndef __fast_searcher_index_source_list_h_included__
#define __fast_searcher_index_source_list_h_included__

#include "../config.h"
#include "source.h"
#include <unordered_map>

typedef std::unordered_map<std::string, Source*> SourceListType;

class SourceList {
	SourceListType source_list_;

public:
	Source* getSource(std::string source_name);
};

#endif /* __fast_searcher_index_source_list_h_included__ */