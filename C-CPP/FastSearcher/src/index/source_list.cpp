
#include "source_list.h"

Source* SourceList::getSource(std::string source_name) {
	SourceListType::const_iterator iter = source_list_.find(source_name);

	if(iter != source_list_.end()) { 
		return iter->second;
	} else {
		Source* source = nullptr;
		try {
			source = new Source(source_name);
		} catch(std::bad_alloc& e) {
			throw e;
		}

		source_list_[source_name] = source;

		return source;
	}
}