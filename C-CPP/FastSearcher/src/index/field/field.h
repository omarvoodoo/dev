
#ifndef __fast_searcher_index_field_h_included__
#define __fast_searcher_index_field_h_included__

#include "../../config.h"
#include <memory>

static const char*  DefaultFieldName = "_d-f_";
static const int8_t DefaultFieldNameSize = 5;

class Field {	

	int8_t value_type_;
	char   field_name_[64];
	char   field_value_[512];
	int8_t field_name_size_;
	int8_t field_value_size_;

public:
    enum Type { String=100, Integer, Double, Unknown };

	Field(char* field_name, int8_t field_name_size, char* field_value, int8_t field_value_size) 
		: value_type_(Type::String), field_name_size_(field_name_size), field_value_size_(field_value_size) {        
        memcpy(field_name_ , field_name , field_name_size_);
		memcpy(field_value_, field_value, field_value_size_);
    }

	Field(char* field_value, int8_t field_value_size) 
		: value_type_(Type::String), field_name_size_(DefaultFieldNameSize), field_value_size_(field_value_size) {
        memcpy(field_name_ , (char*)DefaultFieldName , field_name_size_);
		memcpy(field_value_, field_value, field_value_size_);
    }

	Field(char* field_name, int8_t field_name_size, int64_t value) 
		: value_type_(Type::Integer), field_name_size_(field_name_size), field_value_size_(8) {
		memcpy(field_name_ , (char*)DefaultFieldName , field_name_size_);
		memcpy(field_value_, &value, 8);
	}

	Field(char* field_name, int8_t field_name_size, double value) 
		: value_type_(Type::Double), field_name_size_(field_name_size), field_value_size_(8) {
		memcpy(field_name_ , (char*)DefaultFieldName , field_name_size_);
		memcpy(field_value_, &value, 8);
	}

	char*  getFieldName() { return field_name_; }
	char*  getFieldValue() { return field_value_; }
	int8_t getFieldNameSize() { return field_name_size_; }
	int8_t getFieldValueSize() { return field_value_size_; }
	int8_t getFieldValueType() { return value_type_; }
};

#endif /* __fast_searcher_index_field_h_included__ */
