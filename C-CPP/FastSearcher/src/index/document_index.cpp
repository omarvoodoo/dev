
#include "document_index.h"
#include "field\field_filter.h"
#include <map>

char* DocumentIndex::toStream(FieldListType& field_list) {
    if(field_list.empty())
		return nullptr;

	// 문서의 필드 정보를 저장하기 위한 stream 버퍼 크기.
	// 필드 정보의 형태는 아래와 같다.
	// field_count | field_status_count | field_status * field_status_count | field_value_len * field_count | field_value * field_count
	uint32_t stream_size = 0;

	stream_size += Document::FieldCountSize + Document::FieldStatusCountSize;

	// 문서상의 필드 개수
	// 필드는 문서당 최대 255개를 가질 수 있다.
	const uint8_t  field_count = field_list.size();	

	// 현재 기준으로 해당 source의 전체 field_status 개수
	const uint8_t  field_status_count = (field_name_list_.size() / 32) + 1;

	// field_status 크기 합산
	stream_size += (field_status_count * Document::FieldStatusSize);
		
	// 필드값에 대한 크기(필드값의 크기 + 필드값) 합산
	for(int i=0; i<field_list.size(); ++i) {
		stream_size += Document::FieldValueLenSize + field_list[i]->getFieldValueSize();
	}

	char* stream_buf = nullptr;

	try {
		// 버퍼 메모리 할당
		stream_buf = (char*)malloc(stream_size);
	} catch(std::bad_alloc& e) {
		throw e;
	}
		
	// 버퍼에 값 기록 시작 //////////////////////////////////////////////////////

	size_t  pos = 0;
    size_t  cp_len = 0;
    uint16_t value_start_pos = 0;
	

	cp_len = sizeof(field_count);
	memcpy(stream_buf+pos, &field_count		  , cp_len);	pos += cp_len;

	cp_len = sizeof(field_status_count);
	memcpy(stream_buf+pos, &field_status_count,	cp_len);	pos += cp_len;

	// 필드들을 필드 번호 순서대로 정렬한다.
	// 필드 번호는 source별로 등록된 순서대로 지정된다.
    std::map<uint8_t, Field*> ordered_field_list;
	std::map<uint8_t, Field*>::iterator ordered_field_iter;

	FieldNameListType::iterator iter;
	for(int i=0; i<field_list.size(); ++i) {
		std::string str_field_name(field_list[i]->getFieldName(), field_list[i]->getFieldNameSize());
		iter = field_name_list_.find(str_field_name);

		uint8_t field_no = 0;
		if(iter != field_name_list_.end()) {
			field_no = iter->second;
		} else {
			// 새로운 필드명에 대한 처리
			field_no = field_name_list_.size();
			field_name_list_.insert(std::make_pair(str_field_name, field_no));

			// new_field_name_list_?????????
		}

		ordered_field_list.insert(std::make_pair(field_no, field_list[i]));
	}

	// source상의 전체 필드중에서 문서가 보유한 필드의 위치를 비트값으로 표현한게 되는데
	// 표현을 위한 공간다. 4바이트 단위로 생성된다.
	// source의 전체 필드는 최대 255이다.
	int32_t field_status_list[32] = { 0x00000000, };

	// 문서가 보유한 필드의 위치를 비트값으로 표현한다.
	for(ordered_field_iter=ordered_field_list.begin(); ordered_field_iter!=ordered_field_list.end(); ordered_field_iter++) {
		uint8_t field_no = ordered_field_iter->first;
		const uint8_t field_status_no = field_no / 32;
		const uint8_t field_shift_val = field_no % 32;

		uint32_t status = 0x80000000;
		status = status >> field_shift_val;

		field_status_list[field_status_no] = field_status_list[field_status_no] | status;
        int a = 0;
	}

	cp_len = Document::FieldStatusSize*field_status_count;
	memcpy(stream_buf+pos, (char*)field_status_list, cp_len);	pos += cp_len;

    // 값의 버퍼상의 위치를 저장하기 위한 배열
    uint16_t value_pos_list[256];
        
    // value를 쓰기위한 시작 포지션 계산
    const int value_pos_size = 2;
    value_start_pos = pos + (field_count * value_pos_size);
	
	// 문서가 보유한 필드의 값을 기록한다.
    int pos_idx = 0;
	for(ordered_field_iter=ordered_field_list.begin(); ordered_field_iter!=ordered_field_list.end(); ordered_field_iter++) {
        value_pos_list[pos_idx] = value_start_pos;
		Field* field = ordered_field_iter->second;

		int8_t field_value_type = field->getFieldValueType();
		memcpy(stream_buf+value_start_pos, (char*)&field_value_type, 1);				value_start_pos += 1;
		int8_t field_value_size = field->getFieldValueSize();
		memcpy(stream_buf+value_start_pos, (char*)&field_value_size, 1);				value_start_pos += 1;
		memcpy(stream_buf+value_start_pos, field->getFieldValue(), field_value_size);	value_start_pos += field_value_size;

        ++pos_idx;
	}

    // 값 위치 저장 배열을 스트림에 쓴다.
    memcpy(stream_buf+pos, (char*)value_pos_list, field_count*value_pos_size);
	
	// 버퍼에 값 기록 완료 //////////////////////////////////////////////////////

	return stream_buf;
}

void DocumentIndex::extractFields(char* raw_data, FieldListType& field_list) {
    FieldFilter::Unit* unit = FieldFilter::filter(raw_data);
    FieldFilter::Unit* tmp = nullptr;
    FieldFilter::Unit* cur = unit;
    while(cur) {
        if(cur->name_ && cur->value_) {
            int32_t len = strlen(cur->value_);
            Field::Type value_type = valueType(cur->value_, len);

            if(value_type == Field::Type::Integer) {
                field_list.push_back(
                    new Field(cur->name_, strlen(cur->name_), _atoi64(cur->value_))
                    );
            } else if(value_type == Field::Type::Double) {
                field_list.push_back(
                    new Field(cur->name_, strlen(cur->name_), atof(cur->value_))
                    );
            } else {
                field_list.push_back(
                    new Field(cur->name_, strlen(cur->name_), cur->value_, len)
                    );
            }
        }
        
        tmp = cur;
        cur = cur->next();
        delete tmp;
    }
}

Field::Type DocumentIndex::valueType(const char* str, int32_t str_len) {
    size_t dot_count = 0;
    Field::Type type = Field::Type::Integer;

    for(int i=0; i<str_len; ++i) { 
        if(str[i] == '.') {
            if(i == 0)
                return Field::Type::String;
            else if(dot_count > 0)
                return Field::Type::String;
            
            type = Field::Type::Double;
            dot_count++;
        } else if(str[i] < '0' || str[i] > '9') {
            return Field::Type::String;
        }
    }

    return type;
}

void DocumentIndex::add(int64_t doc_id, char* raw_data) {
    FieldListType out_field_list;

    // 단어 단위 색인
    extractFields(raw_data, out_field_list);

    // 필드 리스트 --> 필드 스트림 버퍼
    char* stream_buf = toStream(out_field_list);

    // 문서 번호별로 스트림 버퍼 등록
    doc_stream_list_[doc_id] = stream_buf;

    // 필드 리스트 정리
    FieldListType::iterator iter = out_field_list.begin();
    for(; iter!=out_field_list.end(); iter++)
        delete *iter;
}

void DocumentIndex::search(BooleanQuery* query, DocIdListType& out_doc_id_list, void* argv) {

    /*
    uint8_t field_no = -1;
    FieldNameListType::iterator name_iter = field_name_list_.find("strkey");
    if(name_iter != field_name_list_.end()) 
        field_no = name_iter->second;    

    // 메모리나 파일로 부터 문서 목록에 해당하는 필드 스트림 데이터를 읽어온다.

    // 2차 검색을 실시한다.
    // 비교(부등호) 연산, 정렬, 통계 함수를 처리하고
    // 필드 출력 값이 지정되었다면 뽑아낸다.
    //DocStreamListType::iterator stream_iter;
    DocIdListType::iterator in_doc_iter = in_doc_id_list.begin();
    for(; in_doc_iter!=in_doc_id_list.end(); ++in_doc_iter) {
        if(doc_stream_list_.find(*in_doc_iter) != doc_stream_list_.end()) {
            Document doc;
            doc.fromStream(doc_stream_list_[*in_doc_iter]);
            Document::Value val = doc.getValue(field_no);
            if(val.type_ == Field::Integer) {
                int64_t value;
                memcpy((char*)&value, val.buf_, val.size_);
                int a = 0;
            } else if(val.type_ == Field::Double) {
                double value;
                memcpy((char*)&value, val.buf_, val.size_);
                int a = 0;
            } else if(val.type_ == Field::String) { 
                int a = 0;
            } else {
                int a = 0;
            }
        }
    }
    */
}
