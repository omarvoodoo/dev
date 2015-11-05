
#include "document.h"
#include <map>

Document::Document() : 
	stream_buf_(nullptr), field_count_(0), field_status_count_(0) {	
}

bool Document::fromStream(char* stream_buf) {
    if(!stream_buf)
        return false;

    const int value_pos_size = 2;

	stream_buf_ = stream_buf;

	size_t pos = 0;
	memcpy((char*)&field_count_			, stream_buf_		, sizeof(uint8_t));	pos += sizeof(uint8_t);
	memcpy((char*)&field_status_count_	, stream_buf_+pos	, sizeof(uint8_t));	pos += sizeof(uint8_t);
    memcpy((char*)field_status_list_    , stream_buf_+pos   , sizeof(int32_t)*field_status_count_); pos += sizeof(int32_t)*field_status_count_;
    memcpy((char*)value_pos_list_       , stream_buf_+pos   , value_pos_size*field_count_); pos += value_pos_size*field_count_;
    value_start_pos_ = pos;

	return true;
}

bool Document::fromStream(char* stream_buf, size_t stream_buf_size) {
	stream_buf_ = stream_buf;

	size_t pos = 0;
	memcpy((char*)&field_count_			, stream_buf_		, sizeof(field_count_));		pos += sizeof(field_count_);
	memcpy((char*)&field_status_count_	, stream_buf_+pos	, sizeof(field_status_count_));	pos += sizeof(field_status_count_);

	return true;
}

Field::Type Document::getType(uint8_t field_no) {
    return Field::Type::Unknown;
}

Document::Value Document::getValue(uint8_t field_no) {
    Value val = {Field::Unknown, 0, nullptr};

	const uint8_t field_status_no = field_no / 32;
	const uint8_t field_shift_val = field_no % 32;

	uint32_t status = 0x80000000;
	status = status >> field_shift_val;

	// 해당 문서에는 주어진 이름에 대한 필드가 존재하지 않는다.
    if((field_status_list_[field_status_no] & status) == 0)
        return val;

    const size_t bit_count = 32;
    size_t before_field_count = 0; // 해당 문서가 포함하는 필드 중에서 주어진 필드 직전까지의 필드 개수.

    // source의 전체 필드 중에서 주어진 필드 직전까지 해당 문서가 포함한 필드의 개수를 구한다.
    for(int i=0; i<=field_status_no; ++i) {
        size_t shift_val = bit_count;
        if(field_status_no == i)
            shift_val = field_shift_val;

        int32_t t = 0x00000001;
        t <<= bit_count - shift_val;
        for(int j=0; j<shift_val; ++j) {
            if((field_status_list_[i] & t) != 0)
                ++before_field_count;
            t <<= 1;
        }
    }

    char* value_buf = stream_buf_ + value_pos_list_[before_field_count];
    memcpy((char*)(&val.type_), value_buf, 1);
    memcpy((char*)(&val.size_), value_buf+1, 1);
    val.buf_ = value_buf + 2;

    return val;
}

bool Document::getValue(uint8_t field_no, int64_t& out_value) {
    return true;
}

bool Document::getValue(uint8_t field_no, double& out_value) {
    return true;
}

bool Document::getValue(uint8_t field_no, char** out_value) {
    return true;
}