
#include "field_filter.h"
#include <chrono>
#include <iomanip>
#include <ctime>
#include <stdlib.h>

namespace FieldFilter {
    Unit::~Unit() {
        if( name_ )
            delete [] name_;

        if( value_ ) 
            delete [] value_;
    }

    void Unit::print() {
        _tprintf(_T("%s %s\n"), name_, value_);
    }

    namespace Property {
        Unit* date(FieldTokenizer::Token* prv, FieldTokenizer::Token* cur, FieldTokenizer::Token* nxt) {
            Unit* unit = nullptr;

            if(!cur->checkDate())
                return unit;

            const TCHAR* name = TIMESTAMP_NAME;
            const size_t name_len = _tcslen(name);

            if( nxt && nxt->checkTime() && cur->depth_ == nxt->depth_ ) {
                // 다음 토큰이 time 속성이라면 병합하여 unit을 만든다.   

                const size_t val_len = TIMESTAMP_LEN;

                TCHAR* name_buf = new TCHAR[name_len+1];
                TCHAR* val_buf = new TCHAR[val_len + 1];

                memcpy(name_buf, name, name_len*sizeof(TCHAR));
                name_buf[name_len] = _T('\0');

                // date 값 입력
                val_buf[ 0] = cur->token_buf_pos_[0];
                val_buf[ 1] = cur->token_buf_pos_[1];
                val_buf[ 2] = cur->token_buf_pos_[2];
                val_buf[ 3] = cur->token_buf_pos_[3];
                val_buf[ 4] = cur->token_buf_pos_[5];
                val_buf[ 5] = cur->token_buf_pos_[6];
                val_buf[ 6] = cur->token_buf_pos_[8];
                val_buf[ 7] = cur->token_buf_pos_[9];
                val_buf[ 8] = nxt->token_buf_pos_[0];
                val_buf[ 9] = nxt->token_buf_pos_[1];
                val_buf[10] = nxt->token_buf_pos_[3];
                val_buf[11] = nxt->token_buf_pos_[4];
                val_buf[12] = nxt->token_buf_pos_[6];
                val_buf[13] = nxt->token_buf_pos_[7];

                if(nxt->token_size_ == 12) {
                    val_buf[14] = nxt->token_buf_pos_[ 9];
                    val_buf[15] = nxt->token_buf_pos_[10];
                    val_buf[16] = nxt->token_buf_pos_[11];
                } else {
                    val_buf[14] = _T('0');
                    val_buf[15] = _T('0');
                    val_buf[16] = _T('0');
                }

                val_buf[val_len] = _T('\0');

                unit = new Unit();
                unit->name_ = name_buf;
                unit->value_ = val_buf;
            } else {
                // 다음 토큰이 time 속성이 아니라면
                // 현재 시간을 병합한다.
                // 직전 토큰이 날짜 형태가 아니라면
                // 현재 날짜를 병합한다.

                TCHAR  current_timestamp[TIMESTAMP_LEN];
                FieldFilter::getCurrentTimestamp(current_timestamp);

                const size_t val_len = TIMESTAMP_LEN;

                TCHAR* name_buf = new TCHAR[name_len+1];
                TCHAR* val_buf = new TCHAR[val_len + 1];

                memcpy(name_buf, name, name_len*sizeof(TCHAR));
                name_buf[name_len] = _T('\0');

                // date 값 입력
                val_buf[0] = cur->token_buf_pos_[0];
                val_buf[1] = cur->token_buf_pos_[1];
                val_buf[2] = cur->token_buf_pos_[2];
                val_buf[3] = cur->token_buf_pos_[3];
                val_buf[4] = cur->token_buf_pos_[5];
                val_buf[5] = cur->token_buf_pos_[6];
                val_buf[6] = cur->token_buf_pos_[8];
                val_buf[7] = cur->token_buf_pos_[9];

                memcpy(val_buf+8, current_timestamp+8, (TIMESTAMP_LEN-8)*sizeof(TCHAR));
                val_buf[val_len] = _T('\0');

                unit = new Unit();
                unit->name_ = name_buf;
                unit->value_ = val_buf;
            }

            return unit;
        }

        Unit* time(FieldTokenizer::Token* prv, FieldTokenizer::Token* cur, FieldTokenizer::Token* nxt) {
            Unit* unit = nullptr;

            if(!cur->checkTime())
                return unit;

            const TCHAR* name = TIMESTAMP_NAME;
            const size_t name_len = _tcslen(name);

            // 직전 토큰이 날짜 형태가 아닐경우에만 현재 날짜를 병합하여 unit을 생성한다.
            // 직전 토큰이 날짜 형태라면 그 토큰을 처리할때 이미 unit을 생성 했을 것이다.
            if( prv && (prv->token_type_&0x1000000111000000)==BIT_ZERO && cur->depth_ == prv->depth_ ) {

                // 직전 토큰이 날짜 형태가 아니라면
                // 현재 날짜를 병합한다.

                TCHAR  current_timestamp[TIMESTAMP_LEN];
                FieldFilter::getCurrentTimestamp(current_timestamp);

                const size_t val_len = TIMESTAMP_LEN;

                TCHAR* name_buf = new TCHAR[name_len+1];
                TCHAR* val_buf = new TCHAR[val_len + 1];

                memcpy(name_buf, name, name_len*sizeof(TCHAR));
                name_buf[name_len] = _T('\0');

                memcpy(val_buf, current_timestamp, 8*sizeof(TCHAR));

                val_buf[ 8] = cur->token_buf_pos_[0];
                val_buf[ 9] = cur->token_buf_pos_[1];
                val_buf[10] = cur->token_buf_pos_[3];
                val_buf[11] = cur->token_buf_pos_[4];
                val_buf[12] = cur->token_buf_pos_[6];
                val_buf[13] = cur->token_buf_pos_[7];

                if(cur->token_size_ == 12) {
                    val_buf[14] = cur->token_buf_pos_[ 9];
                    val_buf[15] = cur->token_buf_pos_[10];
                    val_buf[16] = cur->token_buf_pos_[11];
                } else {
                    val_buf[14] = _T('0');
                    val_buf[15] = _T('0');
                    val_buf[16] = _T('0');
                }

                val_buf[val_len] = _T('\0');

                unit = new Unit();
                unit->name_ = name_buf;
                unit->value_ = val_buf;
            }

            return unit;
        }

        Unit* value_t2(FieldTokenizer::Token* prv, FieldTokenizer::Token* cur, FieldTokenizer::Token* nxt) {
            Unit* unit = nullptr;

            if( prv && (prv->token_type_&BIT_KEY) != BIT_ZERO && cur->depth_ == prv->depth_ ) {
                TCHAR* name_buf = new TCHAR[prv->token_size_+1];
                TCHAR* val_buf = new TCHAR[cur->token_size_+1];

                memcpy(name_buf, prv->token_buf_pos_, prv->token_size_*sizeof(TCHAR));
                name_buf[prv->token_size_] = _T('\0');

                // 앞뒤에 대괄호를 제거하여 값에 넣는다.
                memcpy(val_buf,  cur->token_buf_pos_+1, (cur->token_size_-2)*sizeof(TCHAR));
                val_buf[cur->token_size_-2] = _T('\0');

                unit = new Unit();
                unit->name_ = name_buf;
                unit->value_ = val_buf;
            }

            return unit;
        }

        Unit* equal(FieldTokenizer::Token* prv, FieldTokenizer::Token* cur, FieldTokenizer::Token* nxt) {
            Unit* unit = nullptr;

            if( prv && (prv->token_type_&BIT_KEY) != BIT_ZERO &&
                nxt && (nxt->token_type_&BIT_VALUE_T1) != BIT_ZERO && (prv->depth_ == cur->depth_) && (nxt->depth_ == cur->depth_) ) {

                    // Value 문자열에서 유효성을 검사하고 허용하지 않는 특수 문자에 대한 처리를 한다.
                    // "허용하지 않는 특수 문자에 대한 처리"
                    // 중간에 허용하지 않는 특수 문자가 나타나면 직접 문자까지만 값으로 사용한다.
                    int val_buf_pos;
                    for( val_buf_pos=0; val_buf_pos<nxt->token_size_; ++val_buf_pos ) {
                        if( !(nxt->token_buf_pos_[val_buf_pos] >=  _T('0') && nxt->token_buf_pos_[val_buf_pos] <= _T('9')) &&
                            !(nxt->token_buf_pos_[val_buf_pos] >=  _T('a') || nxt->token_buf_pos_[val_buf_pos] <= _T('z')) &&
                            !(nxt->token_buf_pos_[val_buf_pos] >=  _T('A') || nxt->token_buf_pos_[val_buf_pos] <= _T('Z')) &&
                            nxt->token_buf_pos_[val_buf_pos] != _T(':') &&
                            nxt->token_buf_pos_[val_buf_pos] != _T('?') &&
                            nxt->token_buf_pos_[val_buf_pos] != _T('{') &&
                            nxt->token_buf_pos_[val_buf_pos] != _T('}') &&
                            nxt->token_buf_pos_[val_buf_pos] != _T('.') &&
                            nxt->token_buf_pos_[val_buf_pos] != _T('-') &&
                            nxt->token_buf_pos_[val_buf_pos] != _T('_') ) {
                                break;
                        }
                    }

                    if( val_buf_pos == 0 ) {
                        return unit;
                    }

                    TCHAR* name_buf = new TCHAR[prv->token_size_+1];
                    TCHAR* val_buf = new TCHAR[val_buf_pos+1];

                    memcpy(name_buf, prv->token_buf_pos_, prv->token_size_*sizeof(TCHAR));
                    name_buf[prv->token_size_] = _T('\0');
                    memcpy(val_buf,  nxt->token_buf_pos_, val_buf_pos*sizeof(TCHAR));
                    val_buf[val_buf_pos] = _T('\0');

                    unit = new Unit();
                    unit->name_ = name_buf;
                    unit->value_ = val_buf;
            }

            return unit;
        }

        Unit* ip(FieldTokenizer::Token* prv, FieldTokenizer::Token* cur, FieldTokenizer::Token* nxt) {
            Unit* unit = nullptr;

            if(!cur->checkIp()) {
                return false;
            }

            const TCHAR* name = _T("ipaddr");
            const size_t name_len = _tcslen(name);

            TCHAR* name_buf = new TCHAR[name_len+1];
            TCHAR* val_buf = new TCHAR[cur->token_size_+1];

            memcpy(name_buf, name, name_len*sizeof(TCHAR));
            name_buf[name_len] = _T('\0');
            memcpy(val_buf, cur->token_buf_pos_, cur->token_size_*sizeof(TCHAR));
            val_buf[cur->token_size_] = _T('\0');

            unit = new Unit();
            unit->name_ = name_buf;
            unit->value_ = val_buf;

            return unit;
        }

        Unit* date_t2(FieldTokenizer::Token* prv, FieldTokenizer::Token* cur, FieldTokenizer::Token* nxt) {
            Unit* unit = nullptr;

            if(!cur->checkDateT2())
                return unit;

            const TCHAR* name = TIMESTAMP_NAME;
            const size_t name_len = _tcslen(name);

            if( nxt && nxt->checkTime() && cur->depth_ == nxt->depth_) {
                // 다음 토큰이 time 속성이라면 병합하여 unit을 만든다.   

                const size_t val_len = TIMESTAMP_LEN;

                TCHAR* name_buf = new TCHAR[name_len+1];
                TCHAR* val_buf = new TCHAR[val_len + 1];

                memcpy(name_buf, name, name_len*sizeof(TCHAR));
                name_buf[name_len] = _T('\0');

                // date 값 입력
                val_buf[ 0] = cur->token_buf_pos_[0];
                val_buf[ 1] = cur->token_buf_pos_[1];
                val_buf[ 2] = cur->token_buf_pos_[2];
                val_buf[ 3] = cur->token_buf_pos_[3];
                val_buf[ 4] = cur->token_buf_pos_[5];
                val_buf[ 5] = cur->token_buf_pos_[6];
                val_buf[ 6] = cur->token_buf_pos_[8];
                val_buf[ 7] = cur->token_buf_pos_[9];
                val_buf[ 8] = nxt->token_buf_pos_[0];
                val_buf[ 9] = nxt->token_buf_pos_[1];
                val_buf[10] = nxt->token_buf_pos_[3];
                val_buf[11] = nxt->token_buf_pos_[4];
                val_buf[12] = nxt->token_buf_pos_[6];
                val_buf[13] = nxt->token_buf_pos_[7];

                if(nxt->token_size_ == 12) {
                    val_buf[14] = nxt->token_buf_pos_[ 9];
                    val_buf[15] = nxt->token_buf_pos_[10];
                    val_buf[16] = nxt->token_buf_pos_[11];
                } else {
                    val_buf[14] = _T('0');
                    val_buf[15] = _T('0');
                    val_buf[16] = _T('0');
                }

                val_buf[val_len] = _T('\0');

                unit = new Unit();
                unit->name_ = name_buf;
                unit->value_ = val_buf;
            } else {
                // 다음 토큰이 time 속성이 아니라면
                // 현재 시간을 병합한다.
                // 직전 토큰이 날짜 형태가 아니라면
                // 현재 날짜를 병합한다.

                TCHAR  current_timestamp[TIMESTAMP_LEN];
                FieldFilter::getCurrentTimestamp(current_timestamp);

                const size_t val_len = TIMESTAMP_LEN;

                TCHAR* name_buf = new TCHAR[name_len+1];
                TCHAR* val_buf = new TCHAR[val_len + 1];

                memcpy(name_buf, name, name_len*sizeof(TCHAR));
                name_buf[name_len] = _T('\0');

                // date 값 입력
                val_buf[0] = cur->token_buf_pos_[0];
                val_buf[1] = cur->token_buf_pos_[1];
                val_buf[2] = cur->token_buf_pos_[2];
                val_buf[3] = cur->token_buf_pos_[3];
                val_buf[4] = cur->token_buf_pos_[5];
                val_buf[5] = cur->token_buf_pos_[6];
                val_buf[6] = cur->token_buf_pos_[8];
                val_buf[7] = cur->token_buf_pos_[9];

                memcpy(val_buf+8, current_timestamp+8, (TIMESTAMP_LEN-8)*sizeof(TCHAR));
                val_buf[val_len] = _T('\0');

                unit = new Unit();
                unit->name_ = name_buf;
                unit->value_ = val_buf;
            }

            return unit;
        }

        Unit* date_t3(FieldTokenizer::Token* prv, FieldTokenizer::Token* cur, FieldTokenizer::Token* nxt) {
            Unit* unit = nullptr;

            if(!cur->checkDateT3())
                return unit;

            const TCHAR* name = TIMESTAMP_NAME;
            const size_t name_len = _tcslen(name);

            if( nxt && nxt->checkTime() && cur->depth_ == nxt->depth_ ) {
                // 다음 토큰이 time 속성이라면 병합하여 unit을 만든다.   

                const size_t val_len = TIMESTAMP_LEN;

                TCHAR* name_buf = new TCHAR[name_len+1];
                TCHAR* val_buf = new TCHAR[val_len + 1];

                memcpy(name_buf, name, name_len*sizeof(TCHAR));
                name_buf[name_len] = _T('\0');

                // date 값 입력
                val_buf[ 0] = cur->token_buf_pos_[6];
                val_buf[ 1] = cur->token_buf_pos_[7];
                val_buf[ 2] = cur->token_buf_pos_[8];
                val_buf[ 3] = cur->token_buf_pos_[9];
                val_buf[ 4] = cur->token_buf_pos_[0];
                val_buf[ 5] = cur->token_buf_pos_[1];
                val_buf[ 6] = cur->token_buf_pos_[3];
                val_buf[ 7] = cur->token_buf_pos_[4];
                val_buf[ 8] = nxt->token_buf_pos_[0];
                val_buf[ 9] = nxt->token_buf_pos_[1];
                val_buf[10] = nxt->token_buf_pos_[3];
                val_buf[11] = nxt->token_buf_pos_[4];
                val_buf[12] = nxt->token_buf_pos_[6];
                val_buf[13] = nxt->token_buf_pos_[7];

                if(nxt->token_size_ == 12) {
                    val_buf[14] = nxt->token_buf_pos_[ 9];
                    val_buf[15] = nxt->token_buf_pos_[10];
                    val_buf[16] = nxt->token_buf_pos_[11];
                } else {
                    val_buf[14] = _T('0');
                    val_buf[15] = _T('0');
                    val_buf[16] = _T('0');
                }

                val_buf[val_len] = _T('\0');

                unit = new Unit();
                unit->name_ = name_buf;
                unit->value_ = val_buf;
            } else {
                // 다음 토큰이 time 속성이 아니라면
                // 현재 시간을 병합한다.
                // 직전 토큰이 날짜 형태가 아니라면
                // 현재 날짜를 병합한다.

                TCHAR  current_timestamp[TIMESTAMP_LEN];
                FieldFilter::getCurrentTimestamp(current_timestamp);

                const size_t val_len = TIMESTAMP_LEN;

                TCHAR* name_buf = new TCHAR[name_len+1];
                TCHAR* val_buf = new TCHAR[val_len + 1];

                memcpy(name_buf, name, name_len*sizeof(TCHAR));
                name_buf[name_len] = _T('\0');

                // date 값 입력
                val_buf[0] = cur->token_buf_pos_[6];
                val_buf[1] = cur->token_buf_pos_[7];
                val_buf[2] = cur->token_buf_pos_[8];
                val_buf[3] = cur->token_buf_pos_[9];
                val_buf[4] = cur->token_buf_pos_[0];
                val_buf[5] = cur->token_buf_pos_[1];
                val_buf[6] = cur->token_buf_pos_[3];
                val_buf[7] = cur->token_buf_pos_[4];

                memcpy(val_buf+8, current_timestamp+8, (TIMESTAMP_LEN-8)*sizeof(TCHAR));
                val_buf[val_len] = _T('\0');

                unit = new Unit();
                unit->name_ = name_buf;
                unit->value_ = val_buf;
            }

            return unit;

        }

        Unit* date_t4(FieldTokenizer::Token* prv, FieldTokenizer::Token* cur, FieldTokenizer::Token* nxt) {
            Unit* unit = nullptr;

            if(!cur->checkDateT4())
                return unit;

            const TCHAR* name = TIMESTAMP_NAME;
            const size_t name_len = _tcslen(name);

            if( nxt && nxt->checkTime() && cur->depth_ == nxt->depth_ ) {
                // 다음 토큰이 time 속성이라면 병합하여 unit을 만든다.   

                TCHAR  current_timestamp[TIMESTAMP_LEN];
                FieldFilter::getCurrentTimestamp(current_timestamp);

                const size_t val_len = TIMESTAMP_LEN;

                TCHAR* name_buf = new TCHAR[name_len+1];
                TCHAR* val_buf = new TCHAR[val_len + 1];

                memcpy(name_buf, name, name_len*sizeof(TCHAR));
                name_buf[name_len] = _T('\0');

                // date 값 입력
                val_buf[ 0] = current_timestamp[0];
                val_buf[ 1] = current_timestamp[1];
                val_buf[ 2] = current_timestamp[2];
                val_buf[ 3] = current_timestamp[3];
                val_buf[ 4] = cur->token_buf_pos_[0];
                val_buf[ 5] = cur->token_buf_pos_[1];
                val_buf[ 6] = cur->token_buf_pos_[3];
                val_buf[ 7] = cur->token_buf_pos_[4];
                val_buf[ 8] = nxt->token_buf_pos_[0];
                val_buf[ 9] = nxt->token_buf_pos_[1];
                val_buf[10] = nxt->token_buf_pos_[3];
                val_buf[11] = nxt->token_buf_pos_[4];
                val_buf[12] = nxt->token_buf_pos_[6];
                val_buf[13] = nxt->token_buf_pos_[7];

                if(nxt->token_size_ == 12) {
                    val_buf[14] = nxt->token_buf_pos_[ 9];
                    val_buf[15] = nxt->token_buf_pos_[10];
                    val_buf[16] = nxt->token_buf_pos_[11];
                } else {
                    val_buf[14] = _T('0');
                    val_buf[15] = _T('0');
                    val_buf[16] = _T('0');
                }

                val_buf[val_len] = _T('\0');

                unit = new Unit();
                unit->name_ = name_buf;
                unit->value_ = val_buf;
            } else {
                // 다음 토큰이 time 속성이 아니라면
                // 현재 시간을 병합한다.
                // 직전 토큰이 날짜 형태가 아니라면
                // 현재 날짜를 병합한다.

                TCHAR  current_timestamp[TIMESTAMP_LEN];
                FieldFilter::getCurrentTimestamp(current_timestamp);

                const size_t val_len = TIMESTAMP_LEN;

                TCHAR* name_buf = new TCHAR[name_len+1];
                TCHAR* val_buf = new TCHAR[val_len + 1];

                memcpy(name_buf, name, name_len*sizeof(TCHAR));
                name_buf[name_len] = _T('\0');

                // date 값 입력
                val_buf[0] = current_timestamp[0];
                val_buf[1] = current_timestamp[1];
                val_buf[2] = current_timestamp[2];
                val_buf[3] = current_timestamp[3];
                val_buf[4] = cur->token_buf_pos_[0];
                val_buf[5] = cur->token_buf_pos_[1];
                val_buf[6] = cur->token_buf_pos_[3];
                val_buf[7] = cur->token_buf_pos_[4];

                memcpy(val_buf+8, current_timestamp+8, (TIMESTAMP_LEN-8)*sizeof(TCHAR));
                val_buf[val_len] = _T('\0');

                unit = new Unit();
                unit->name_ = name_buf;
                unit->value_ = val_buf;
            }

            return unit;
        }
    }

    void getCurrentTimestamp(TCHAR* out) {
        auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
#ifdef _UNICODE
        char buf[TIMESTAMP_LEN];
        std::strftime(buf, TIMESTAMP_LEN, "%Y%m%d%I%M%S", std::localtime(&now));
        mbstowcs(out, buf, TIMESTAMP_LEN-3);
#else
        std::strftime(out, TIMESTAMP_LEN, "%Y%m%d%I%M%S", std::localtime(&now));
#endif
        
        out[14] = _T('0');
        out[15] = _T('0');
        out[16] = _T('0');        
    }

    void printTokenStream( FieldTokenizer::Token* token_stream ) {
        if(token_stream) {
            FieldTokenizer::Token* token = token_stream;

            while(token) {
                FieldTokenizer::print(token);

                token = token->next();
            }
        }
    }

    void printUnitStream( Unit* unit_stream ) {
        if(unit_stream) {
            Unit* unit = unit_stream;

            while(unit) {
                unit->print();

                unit = unit->next();
            }
        }
    }

    Unit* filter( TCHAR* raw ) {
        Unit* unit_first = nullptr;
        Unit* unit = nullptr;

        // 문자열 데이터를 토큰화한다.
        FieldTokenizer::Token* token_stream = FieldTokenizer::tokenize( raw, _tcslen(raw), 0 );

        if(token_stream) {
            FieldTokenizer::Token* prv;
            FieldTokenizer::Token* cur;
            FieldTokenizer::Token* nxt;

            prv = cur = nxt = nullptr;

            cur = token_stream;

            while(cur) {
                //Tokenizer::print(cur);

                nxt = cur->next();

                // 현재 토큰이 KEY나 VALUE_T1가 아닐때 처리.
                if( (cur->token_type_ & 0x1100111111000000) != BIT_ZERO ) {
                    Unit* new_unit = nullptr;

                    if( (cur->token_type_ & BIT_DATE) != BIT_ZERO ) {
                        new_unit = Property::date(prv, cur, nxt);
                    } else if( (cur->token_type_ & BIT_TIME) != BIT_ZERO ) {
                        new_unit = Property::time(prv, cur, nxt);
                    } else if( (cur->token_type_ & BIT_VALUE_T2) != BIT_ZERO ) {
                        new_unit = Property::value_t2(prv, cur, nxt);
                    } else if( (cur->token_type_ & BIT_EQUAL) != BIT_ZERO ) {
                        new_unit = Property::equal(prv, cur, nxt);
                    } else if( (cur->token_type_ & BIT_IP) != BIT_ZERO ) {
                        new_unit = Property::ip(prv, cur, nxt);
                    } else if( (cur->token_type_ & BIT_DATE_T2) != BIT_ZERO ) {
                        new_unit = Property::date_t2(prv, cur, nxt);
                    } else if( (cur->token_type_ & 0x0000000011000000) != BIT_ZERO ) {
                        if( (cur->token_type_ & BIT_DATE_T3) != BIT_ZERO ) {
                            new_unit = Property::date_t3(prv, cur, nxt);
                        }

                        if( new_unit == nullptr && (cur->token_type_ & BIT_DATE_T4) != BIT_ZERO ) {
                            new_unit = Property::date_t4(prv, cur, nxt);
                        }
                    }

                    if(new_unit) {
                        if(unit) {
                            unit->setNext( new_unit );
                        } else {
                            unit_first = new_unit;
                        }

                        unit = new_unit;
                    }
                }

                prv = cur;
                cur  = nxt;
            }

            // token stream 정리 
            cur = token_stream;
            while(cur) {
                nxt = cur->next();
                delete cur;
                cur = nxt;
            }
        }

        return unit_first;
    }
}
