
#ifndef _fast_searcher_index_document_h_included_
#define _fast_searcher_index_document_h_included_

#include "../config.h"
#include "field/field.h"
#include <vector>
#include <string>
#include <memory>

typedef std::vector<int64_t> DocIdListType;

class Document {	
public:
    struct Value {
        int8_t  type_;
        uint8_t size_;
        char*   buf_;
    };

private:
	/*FieldNameListType& field_name_list_;*/

	char*		stream_buf_;
	uint8_t		field_count_;
	uint8_t		field_status_count_;    // source당 필드 개수는 최대 256개를 넘지 않을 것이라 가정함.
    int32_t     field_status_list_[8];
    uint16_t    value_pos_list_[256];
    uint16_t    value_start_pos_;
public:
    enum { FieldCountSize=1, FieldStatusCountSize=1, FieldStatusSize=4, FieldValueLenSize=1, };

	Document();

	// 스트림 버퍼를 읽어서 Document 객체를 구성한다.
    bool fromStream(char* stream_buf);	
	bool fromStream(char* stream_buf, size_t stream_buf_size);	

    // 해당 필드의 형식을 반환한다.
    // String, Integer, Double
    // Unknwon을 반환할 경우 해당 필드에 대한 값이 없음을 나타낸다.
    Field::Type getType(uint8_t field_no);

	Value getValue(uint8_t field_no);
	bool getValue(uint8_t field_no, int64_t& out_value);
	bool getValue(uint8_t field_no, double& out_value);
	bool getValue(uint8_t field_no, char** out_value);
};

#endif /* _fast_searcher_index_document_h_included_ */
