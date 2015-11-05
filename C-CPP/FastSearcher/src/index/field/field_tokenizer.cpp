
#include "field_tokenizer.h"
#include <sstream>

namespace FieldTokenizer {
    bool Token::checkDate() {
        // 2013/11/11

        if( (token_type_&BIT_DATE) == BIT_ZERO )
            return false;

        if( token_size_ != 10 )
            return false;

        for(int i=0; i<token_size_; i++) {
            if(i == 0) {
                if(token_buf_pos_[i] != _T('1') && token_buf_pos_[i] != _T('2')) {
                    return false;
                }
            } else if( i == 5 ) {
                if(token_buf_pos_[i] != _T('0') && token_buf_pos_[i] != _T('1')) {
                    return false;
                }
            } else if( i == 8) {
                if(token_buf_pos_[i] != _T('0') && token_buf_pos_[i] != _T('1') && token_buf_pos_[i] != _T('2') && token_buf_pos_[i] != _T('3')) {
                    return false;
                }
            }
        }

        return true;
    }

    bool Token::checkTime() {
        // 11:11:11.111

        if( (token_type_&BIT_TIME) == BIT_ZERO )
            return false;

        if( token_size_ != 8 && token_size_ != 12)
            return false;

        for(int i=0; i<token_size_; i++) {
            if(i == 0) {
                if(token_buf_pos_[i] != _T('0') && token_buf_pos_[i] != _T('1') && token_buf_pos_[i] != _T('2')) {
                    return false;
                }
            } else if( i == 3 || i == 6 ) {
                if(token_buf_pos_[i] > _T('5')) {
                    return false;
                }
            }
        }

        return true;
    }

    bool Token::checkIp() {
        // 111.111.111.111

        if( (token_type_&BIT_IP) == BIT_ZERO )
            return false;

        if( token_size_ > 15 ) 
            return false;

        size_t comma_count = 0;
        size_t num_type_count = 0;

        for(int i=0; i<token_size_; i++) {

            if( token_buf_pos_[i] == _T('.') ) {
                comma_count++;
                num_type_count = 0;
            } else {
                if(++num_type_count > 3)
                    return false;
            }
        }

        if(comma_count != 3)
            return false;

        return true;
    }

    bool Token::checkDateT2() {
        // YYYY-MM-DD

        if( (token_type_&BIT_DATE_T2) == BIT_ZERO )
            return false;

        if( token_size_ != 10 )
            return false;

        for(int i=0; i<token_size_; i++) {
            if(i == 0) {
                if(token_buf_pos_[i] != _T('1') && token_buf_pos_[i] != _T('2')) {
                    return false;
                }
            } else if( i == 5 ) {
                if(token_buf_pos_[i] != _T('0') && token_buf_pos_[i] != _T('1')) {
                    return false;
                }
            } else if( i == 8) {
                if(token_buf_pos_[i] != _T('0') && token_buf_pos_[i] != _T('1') && token_buf_pos_[i] != _T('2') && token_buf_pos_[i] != _T('3')) {
                    return false;
                }
            }
        }

        return true;
    }

    bool Token::checkDateT3() {
        // MM/DD/YYYY

        if( (token_type_&BIT_DATE_T3) == BIT_ZERO )
            return false;

        if( token_size_ != 10 )
            return false;

        for(int i=0; i<token_size_; i++) {
            if( i == 0) {
                if(token_buf_pos_[i] != _T('0') && token_buf_pos_[i] != _T('1') && token_buf_pos_[i] != _T('2') && token_buf_pos_[i] != _T('3')) {
                    return false;
                }
            } else if( i == 3 )  {
                if(token_buf_pos_[i] != _T('0') && token_buf_pos_[i] != _T('1') && token_buf_pos_[i] != _T('2') && token_buf_pos_[i] != _T('3')) {
                    return false;
                }
            } else if(i == 6) {
                if(token_buf_pos_[i] != _T('1') && token_buf_pos_[i] != _T('2')) {
                    return false;
                }
            }
        }

        return true;
    }

    bool Token::checkDateT4() {
        // MM/DD

        if( (token_type_&BIT_DATE_T4) == BIT_ZERO )
            return false;

        if( token_size_ != 5 )
            return false;

        for(int i=0; i<token_size_; i++) {
            if( i == 0) {
                if(token_buf_pos_[i] != _T('0') && token_buf_pos_[i] != _T('1') && token_buf_pos_[i] != _T('2') && token_buf_pos_[i] != _T('3')) {
                    return false;
                }
            } else if( i == 3 )  {
                if(token_buf_pos_[i] != _T('0') && token_buf_pos_[i] != _T('1') && token_buf_pos_[i] != _T('2') && token_buf_pos_[i] != _T('3')) {
                    return false;
                }
            }
        }

        return true;
    }

    void print(Token* token) {
        TCHAR buf[1024];
        _tcsncpy(buf, token->token_buf_pos_, token->token_size_);
        buf[token->token_size_] = _T('\0');

#ifdef _UNICODE
        std::wstringstream ss;
#else
        std::stringstream ss;
#endif

        ss << buf << _T(" --> ");

        if( (token->token_type_ & BIT_DATE) != BIT_ZERO ) {
            ss << _T("DATE, ");
        }

        if( (token->token_type_ & BIT_TIME) != BIT_ZERO ) {
            ss << _T("TIME, ");
        }

        if( (token->token_type_ & BIT_KEY) != BIT_ZERO ) {
            ss << _T("KEY, ");
        }

        if( (token->token_type_ & BIT_VALUE_T1) != BIT_ZERO ) {
            ss << _T("VALUE_T1, ");
        }

        if( (token->token_type_ & BIT_VALUE_T2) != BIT_ZERO ) {
            ss << _T("VALUE_T2, ");
        }

        if( (token->token_type_ & BIT_EQUAL) != BIT_ZERO ) {
            ss << _T("EQUAL, ");
        }

        if( (token->token_type_ & BIT_IP) != BIT_ZERO ) {
            ss << _T("IP, ");
        }

        if( (token->token_type_ & BIT_DATE_T2) != BIT_ZERO ) {
            ss << _T("DATE_T2, ");
        }

        if( (token->token_type_ & BIT_DATE_T3) != BIT_ZERO ) {
            ss << _T("DATE_T3, ");
        }

        if( (token->token_type_ & BIT_DATE_T4) != BIT_ZERO ) {
            ss << _T("DATE_T4, ");
        }

        if( (token->token_type_ & BIT_VALUE_T3) != BIT_ZERO ) {
            ss << _T("VALUE_T3, ");
        }

        _tprintf(_T("%s\n"), ss.str().c_str());
    }

    Token* tokenize( TCHAR* raw, size_t raw_len, size_t depth ) {
        Token* token_first = nullptr;
        Token* token = nullptr;

        size_t len = raw_len;

        size_t bracket_enter_count = 0;
        size_t bracket_out_count = 0;
        bool   bracket_space = false;

        // TOKEN 버퍼 초기화
        TCHAR*      token_pos = nullptr;
        size_t      token_size = 0;
        long long   token_type = BIT_ALL_PROPERTY;

        for(int raw_idx=0; raw_idx<len; ) {
            if( raw[raw_idx] == _T(' ') || raw[raw_idx] == _T('\t') || raw[raw_idx] == _T('\r') ) {
                if( !bracket_space ) {
                    goto TOKEN_COMPLETE;
                }
            } else if( raw[raw_idx] == _T('=') ) {
                if( !bracket_space ) 
                {
                    if( token_size > 0 ) {
                        // = 문자가 확인되면 이전 Token을 완성하기 때문에 = 문자가 사용되지 않는다. 다시 = 문자 부터 시작해야 한다.
                        raw_idx--;

                        goto TOKEN_COMPLETE;
                    } else {
                        // equal 이외의 모든 속성 제거
                        token_type = token_type & 0x0000010000000000;

                        // Token 시작 포지션 등록
                        if(token_size == 0) {
                            token_pos = raw + raw_idx;
                            token_size++;
                        }

                        goto TOKEN_COMPLETE;
                    }
                }
            } else if( raw[raw_idx] == _T('[') ) {    
                if( !bracket_space ) 
                {
                    if(token_size > 0) {
                        // [ 문자가 확인되면 이전 Token을 완성하기 때문에 [ 문자가 사용되지 않는다. 다시 [ 문자 부터 시작해야 한다.
                        raw_idx--;

                        goto TOKEN_COMPLETE;
                    } else {
                        // value_t2(대괄호 값) 이외의 모든 속성 제거
                        token_type = token_type & 0x0000100000000000;

                        bracket_enter_count++;
                        bracket_space = true;
                    }
                } else {
                    bracket_enter_count++;
                }               
            } else if( raw[raw_idx] == _T(']') ) {
                if( bracket_space ) {
                    bracket_out_count++;

                    if(bracket_enter_count == bracket_out_count) {
                        bracket_enter_count = 0;
                        bracket_out_count = 0;
                        bracket_space = false;

                        // value_t2일 경우 내부 값에 대해서 재귀적으로 tokenize한다.
                        Token* sub_token_stream = FieldTokenizer::tokenize( token_pos+1, token_size-1, depth+1 );

                        // 내부 값으로 얻은 Token을 제일 처음에 붙여 넣는다.
                        if( sub_token_stream ) {
                            Token* sub_token_tail = sub_token_stream;
                            while( sub_token_tail ) {
                                if( sub_token_tail->next() == nullptr )
                                    break;

                                sub_token_tail = sub_token_tail->next();
                            }

                            if( token_first ) {
                                sub_token_tail->setNext(token_first);
                            } else {
                                token = sub_token_tail;
                            }

                            token_first = sub_token_stream;
                        }

                        // ] 문자까지 token에 포함시키다.
                        token_size++;

                        goto TOKEN_COMPLETE;
                    }
                }
            } 

            if( !bracket_space ) {

                // 유효하지 않은 속성 제거
                token_type = token_type & 0x1111001111111111;

                // 1~2 속성에 대한 처리
                if( (token_type & 0x1100000000000000) != BIT_ZERO ) {
                    // date에 대한 체크
                    if( (token_type & BIT_DATE) != BIT_ZERO ) {
                        // 2013/11/11

                        // 총 10자리 중에 이미 10자리가 있는데 추가적으로 문자가 추가되서는 않된다.
                        if(token_size > 10) {
                            token_type = token_type & RBIT_DATE;
                        } else {
                            if( token_size == 4 || token_size == 7 ) {
                                if( raw[raw_idx] != _T('/') )
                                    token_type = token_type & RBIT_DATE;
                            } else if( raw[raw_idx] < _T('0') || raw[raw_idx] > _T('9') ) {
                                token_type = token_type & RBIT_DATE;
                            }
                        }
                    }

                    // time에 대한 체크
                    if( (token_type & 0x0100000000000000) != BIT_ZERO ) {
                        // 11:11:11.000

                        // 총 12자리 중에 이미 12자리가 있는데 추가적으로 문자가 추가되서는 않된다.
                        if(token_size >= 12) {
                            token_type = token_type & 0x1011111111111111;
                        } else {
                            if( token_size == 2 || token_size == 5) {
                                if( raw[raw_idx] != _T(':') )
                                    token_type = token_type & 0x1011111111111111;
                            } else if( token_size == 8 ) {
                                if( raw[raw_idx] != _T('.') )
                                    token_type = token_type & 0x1011111111111111;
                            } else if( raw[raw_idx] < _T('0') && raw[raw_idx] > _T('9') ) {
                                token_type = token_type & 0x1011111111111111;
                            }
                        }
                    }
                }

                // 3~4 속성에 대한 처리
                if( (token_type & 0x0011000000000000) != BIT_ZERO ) {                    
                    if( (raw[raw_idx] >= _T('0') && raw[raw_idx] <= _T('9')) ||
                        (raw[raw_idx] >= _T('a') && raw[raw_idx] <= _T('z')) ||
                        (raw[raw_idx] >= _T('A') && raw[raw_idx] <= _T('Z')) ||
                        raw[raw_idx] == _T('.') ||
                        raw[raw_idx] == _T('-') ||
                        raw[raw_idx] == _T('_') ||
                        raw[raw_idx] == _T(':') ||
                        raw[raw_idx] == _T('?') ||
                        raw[raw_idx] == _T('{') ||
                        raw[raw_idx] == _T('}') ||
                        // 이후 값들은 value 형태의 값으로는 받아 들이지만 
                        // 필드화 과정에서 제외 된다
                        raw[raw_idx] == _T(',') ||
                        raw[raw_idx] == _T(';') ) {

                            // 위에 조건은 일반 값(value_t1)으로 만족하는 것이고
                            // :, ?, {, }는 Key로 사용될 수 없는 특수문자이므로 포함한다면 key 속성을 제거한다.
                            if( raw[raw_idx] == _T(':') ||
                                raw[raw_idx] == _T('?') ||
                                raw[raw_idx] == _T('{') ||
                                raw[raw_idx] == _T('}') ||
                                raw[raw_idx] == _T(',') ||
                                raw[raw_idx] == _T(';') ) {
                                    token_type = token_type & RBIT_KEY;                            
                            }
                    } else {
                        // 위에 조건을 만족하지 못한다면 key로도 사용 못하고 일반 값으로도 사용할 수 없으므로
                        // 두개의 속성을 제거한다.
                        token_type = token_type & 0x1100111111111111;
                    }
                }
                /* yungyikim 20131211
                if( (token_type & 0x0011000000000000) != BIT_ZERO ) {                    
                    if( (raw[raw_idx] >= _T('0') && raw[raw_idx] <= _T('9')) ||
                        (raw[raw_idx] >= _T('a') && raw[raw_idx] <= _T('z')) ||
                        (raw[raw_idx] >= _T('A') && raw[raw_idx] <= _T('Z')) ||
                        raw[raw_idx] == _T('.') ||
                        raw[raw_idx] == _T('-') ||
                        raw[raw_idx] == _T('_') ||
                        raw[raw_idx] == _T(':') ||
                        raw[raw_idx] == _T('?') ||
                        raw[raw_idx] == _T('{') ||
                        raw[raw_idx] == _T('}') ) {

                            // 위에 조건은 일반 값(value_t1)으로 만족하는 것이고
                            // :, ?, {, }는 Key로 사용될 수 없는 특수문자이므로 포함한다면 key 속성을 제거한다.
                            if( raw[raw_idx] == _T(':') ||
                                raw[raw_idx] == _T('?') ||
                                raw[raw_idx] == _T('{') ||
                                raw[raw_idx] == _T('}') ) {
                                    token_type = token_type & RBIT_KEY;                            
                            }
                    } else {
                        // 위에 조건을 만족하지 못한다면 key로도 사용 못하고 일반 값으로도 사용할 수 없으므로
                        // 두개의 속성을 제거한다.
                        token_type = token_type & 0x1100111111111111;
                    }
                }
                */

                // 7(ip) 속성에 대한 처리
                // 모든 Token이 완성된 후에 남아있는 속성에 대한 검증이 들어가기 때문에
                // Token을 생성할 때는 숫자와 .만이 포함되는지 여부로 판단한다.
                if ( (token_type & BIT_IP) != BIT_ZERO ) {
                    if( (raw[raw_idx] < _T('0') || raw[raw_idx] > _T('9')) && raw[raw_idx] != _T('.') ) {
                        // 7 속성 제거
                        token_type = token_type & RBIT_IP;
                    }
                }

                // 8 ~ 10 속성에 대한 처리
                if( (token_type & 0x0000000111000000) != BIT_ZERO ) {
                    if( (token_type & BIT_DATE_T2) != BIT_ZERO ) {
                        // YYYY-MM-DD

                        // 총 10자리 중에 이미 10자리가 있는데 추가적으로 문자가 추가되서는 않된다.
                        if(token_size > 10) {
                            token_type = token_type & RBIT_DATE_T2;
                        } else {
                            if( token_size == 4 || token_size == 7 ) {
                                if( raw[raw_idx] != _T('-') )
                                    token_type = token_type & RBIT_DATE_T2;
                            } else if( raw[raw_idx] < _T('0') || raw[raw_idx] > _T('9') ) {
                                token_type = token_type & RBIT_DATE_T2;
                            }
                        }
                    }

                    if( (token_type & BIT_DATE_T3) != BIT_ZERO ) {
                        // MM/DD/YYYY

                        // 총 10자리 중에 이미 10자리가 있는데 추가적으로 문자가 추가되서는 않된다.
                        if(token_size > 10) {
                            token_type = token_type & RBIT_DATE_T3;
                        } else {
                            if( token_size == 2 || token_size == 5 ) {
                                if( raw[raw_idx] != _T('/') )
                                    token_type = token_type & RBIT_DATE_T3;
                            } else if( raw[raw_idx] < _T('0') || raw[raw_idx] > _T('9') ) {
                                token_type = token_type & RBIT_DATE_T3;
                            }
                        }
                    }

                    if( (token_type & BIT_DATE_T4) != BIT_ZERO ) {
                        // MM/DD

                        // 총 5자리 중에 이미 5자리가 있는데 추가적으로 문자가 추가되서는 않된다.
                        if(token_size > 5) {
                            token_type = token_type & RBIT_DATE_T4;
                        } else {
                            if( token_size == 2 ) {
                                if( raw[raw_idx] != _T('/') )
                                    token_type = token_type & RBIT_DATE_T4;
                            } else if( raw[raw_idx] < _T('0') || raw[raw_idx] > _T('9') ) {
                                token_type = token_type & RBIT_DATE_T4;
                            }
                        }
                    }
                }
            }

            // Token 시작 포지션 등록
            if(token_size == 0)
                token_pos = raw + raw_idx;

            // token 크기 증가
            token_size++;

            // 다음 문자로 이동
            ++raw_idx;

            continue;
TOKEN_COMPLETE:
            if(token_size > 0) {
                // 예외처리
                // 대괄호가 열린상태에서 끝나는경우 그냥 버린다.
                if(!bracket_space) {
                    Token* new_token = new Token(token_pos, token_size, token_type, depth);

                    if(token) {                    
                        token->setNext(new_token);                
                    } else {
                        token_first = new_token;
                    }

                    token = new_token;
                }
            }

TOKEN_BUF_INIT:
            token_pos = nullptr;
            token_size = 0;
            token_type = BIT_ALL_PROPERTY;

            // 다음 문자로 이동
            ++raw_idx;
        }

        // 나머지 처리
        if(token_size > 0) {
            // 예외처리
            // 대괄호가 열린상태에서 끝나는경우 그냥 버린다.
            if(!bracket_space) {
                Token* new_token = new Token(token_pos, token_size, token_type, depth);

                if(token) {                    
                    token->setNext(new_token);                
                } else {
                    token_first = new_token;
                }

                token = new_token;
            }
        }

        return token_first;
    }
}
