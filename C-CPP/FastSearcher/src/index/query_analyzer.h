
#ifndef __fast_searcher_index_query_analyzer_h_included__
#define __fast_searcher_index_query_analyzer_h_included__

#include "boolean_query.h"
#include "range_query.h"
#include "term_query.h"
#include <string>
#include <regex>

namespace QueryAnalyzer {
    
    class _internal_QueryAnalyzer {
        BooleanQuery* extractOperators(std::string& query_string) {
            return nullptr;
        }

        // 부등식 키워드 추출
        BooleanQuery* extractInequalities(std::string& query_string) {
            // | AA and BB or Field > 100 and Field <200 |
            // | AA and BB or Field > 100 |
            // Field > 100 |
            // | Field > 100
            // AA
            // AA and BB

            BooleanQuery* b_query = new BooleanQuery();
                        
            std::string src_str;
            std::smatch m;
            src_str = query_string;
            while(std::regex_search(src_str, m, std::regex("([a-zA-Z][a-zA-Z0-9.-_]+)\\s\*((>|>=|<|<=|=)\\s\*(([0-9]+)|[a-zA-Z0-9.-_:{}]+)\\s\*)((and|And|AND|or|Or|OR)\\s\*|(\\||$))"))) {
                std::string str = m[0];

                std::string rep_str = "";
                if(str[0] == '|' || str.back() == '|') {
                    rep_str = "|";
                }

                size_t pos = query_string.find(str);
                query_string.replace(pos, str.size(), rep_str);

                if(!m[4].str().empty()) {
                    // 부등호 값을 포함.
                        
                    if(!m[5].str().empty()) {
                        int64_t from_value, to_value;

                        // 값이 숫자 형태.
                        if(m[3] == ">") {
                            from_value = _atoi64(m[5].str().c_str())+1;
                            to_value = INT64_MAX;
                        } else if(m[3] == ">=") {
                            from_value = _atoi64(m[5].str().c_str());
                            to_value = INT64_MAX;
                        } else if(m[3] == "<") {
                            from_value = INT64_MIN;
                            to_value = _atoi64(m[5].str().c_str())-1;
                        } else if(m[3] == "<=") {
                            from_value = INT64_MIN;
                            to_value = _atoi64(m[5].str().c_str());
                        }

                        RangeQuery* rq = new RangeQuery(m[1].str().c_str(), from_value, to_value);

                        if(m[7] == "and" || m[8] == "|") {
                            b_query->add(rq, OperationType::And);
                        } else if(m[7] == "or") {
                            b_query->add(rq, OperationType::Or);
                        } else {
                            b_query->add(rq, OperationType::Null);
                        }
                    } else {
                        int a = 0;
                    }
                }

                src_str = m.suffix().str();

                // 여러개의 복합 쿼리는 And 연산으로 묶으면 된다.
            }

            return b_query;
        }

        // 복합 키워드 추출(And나 Or와 같은 논리 연산자로 묶인 키워드)
        BooleanQuery* extractCompound(std::string& query_string) {
            // CC | AA and BB or Field > 100 | DD
            // CC | AA and BB or Field > 100
            // AA and BB or Field > 100 | DD
            // AA and BB
            // CC | BB and Field = 10 | DD
            // CC | BB and Field = 10
            // BB and Field = 10 | DD
            // Field > 100 | DD
            // CC | BB and Field=10 | DD
            // CC | Field > 100

            BooleanQuery* b_query = new BooleanQuery();
                        
            std::string src_str;
            std::smatch m;
            src_str = query_string;
            while(std::regex_search(src_str, m, std::regex("(^\\s?|\\|\\s)([a-zA-Z][a-zA-Z0-9.-_]+(\\s?(>|>=|<|<=|=)\\s?[a-zA-Z0-9.-_:{}]+)?)(\\s(and|And|AND|or|Or|OR)\\s([a-zA-Z][a-zA-Z0-9.-_]+(\\s?(>|>=|<|<=|=)\\s?[a-zA-Z0-9.-_:{}]+)?))+(\\s\\||\\s?$)"))) {
                std::string compound_query = m[0];

                std::string rep_str = "";
                if(compound_query[0] == '|' || compound_query.back() == '|') {
                    rep_str = "|";
                }

                size_t pos = query_string.find(compound_query);
                query_string.replace(pos, compound_query.size(), rep_str);

                BooleanQuery* bq = new BooleanQuery();

                std::string sub_src_str = compound_query;
                std::smatch sub_m;
                while(std::regex_search(sub_src_str, sub_m, std::regex("([a-zA-Z][a-zA-Z0-9.-_]+)\\s\*((>|>=|<|<=|=)\\s\*(([0-9]+)|[a-zA-Z0-9.-_:{}]+)\\s\*)?((and|And|AND|or|Or|OR)\\s\*|(\\||$))"))) {
                    std::string str = sub_m[0];

                    std::string sub_rep_str = "";
                    size_t pos = compound_query.find(str);
                    compound_query.replace(pos, str.size(), sub_rep_str);

                    if(!sub_m[4].str().empty()) {
                        // 부등호 값을 포함.
                        
                        if(!sub_m[5].str().empty()) {
                            int64_t from_value, to_value;

                            // 값이 숫자 형태.
                            if(sub_m[3] == ">") {
                                from_value = _atoi64(sub_m[5].str().c_str())+1;
                                to_value = INT64_MAX;
                            } else if(sub_m[3] == ">=") {
                                from_value = _atoi64(sub_m[5].str().c_str());
                                to_value = INT64_MAX;
                            } else if(sub_m[3] == "<") {
                                from_value = INT64_MIN;
                                to_value = _atoi64(sub_m[5].str().c_str())-1;
                            } else if(sub_m[3] == "<=") {
                                from_value = INT64_MIN;
                                to_value = _atoi64(sub_m[5].str().c_str());
                            }

                            RangeQuery* rq = new RangeQuery(sub_m[1].str().c_str(), from_value, to_value);

                            if(sub_m[7] == "and" || sub_m[8] == "|") {
                                bq->add(rq, OperationType::And);
                            } else if(sub_m[7] == "or") {
                                bq->add(rq, OperationType::Or);
                            } else {
                                bq->add(rq, OperationType::Null);
                            }
                        } else {
                            int a = 0;
                        }
                    } else {
                        TermQuery* tq = new TermQuery(sub_m[1].str().c_str());

                        if(sub_m[7] == "and" || sub_m[8] == "|") {
                            bq->add(tq, OperationType::And);
                        } else if(sub_m[7] == "or") {
                            bq->add(tq, OperationType::Or);
                        } else {
                            bq->add(tq, OperationType::Null);
                        }
                    }

                    sub_src_str = sub_m.suffix().str();
                }

                src_str = m.suffix().str();

                b_query->add(bq, OperationType::And);

                // 여러개의 복합 쿼리는 And 연산으로 묶으면 된다.
            }

            return b_query;
        }

        // 일반 키워드 추출
        BooleanQuery* extractGeneral(std::string& query_string) {
            BooleanQuery* b_query = new BooleanQuery();
                        
            std::string src_str;
            std::smatch m;
            src_str = query_string;
            while(std::regex_search(src_str, m, std::regex("([a-zA-Z][a-zA-Z0-9.-_]+)\\s\*((and|And|AND|or|Or|OR)\\s\*|(\\||$))"))) {
                std::string str = m[0];

                std::string rep_str = "";
                if(str[0] == '|' || str.back() == '|') {
                    rep_str = "|";
                }

                size_t pos = query_string.find(str);
                query_string.replace(pos, str.size(), rep_str);

                TermQuery* tq = new TermQuery(m[1].str().c_str());

                if(m[7] == "and" || m[8] == "|") {
                    b_query->add(tq, OperationType::And);
                } else if(m[7] == "or") {
                    b_query->add(tq, OperationType::Or);
                } else {
                    b_query->add(tq, OperationType::Null);
                }

                src_str = m.suffix().str();

                // 여러개의 복합 쿼리는 And 연산으로 묶으면 된다.
            }

            return b_query;
        }

        // 키워드 추출
        BooleanQuery* extractKeywords(std::string& query_string) {
            BooleanQuery* result_query = new BooleanQuery();

            BooleanQuery* compound_query = extractCompound(query_string);
            if(compound_query) {
                QueryListType::iterator iter = compound_query->getQueryList().begin();

                for(;iter!=compound_query->getQueryList().end(); iter++) {
                    result_query->add(*iter);
                }

                delete compound_query;
            }

            BooleanQuery* inequalities_query = extractInequalities(query_string);
            if(inequalities_query) {
                QueryListType::iterator iter = inequalities_query->getQueryList().begin();

                for(;iter!=inequalities_query->getQueryList().end(); iter++) {
                    result_query->add(*iter);
                }

                delete inequalities_query;
            }

            BooleanQuery* general_query = extractGeneral(query_string);
            if(general_query) {
                QueryListType::iterator iter = general_query->getQueryList().begin();

                for(;iter!=general_query->getQueryList().end(); iter++) {
                    result_query->add(*iter);
                }

                delete general_query;
            }

            return result_query;
        }

    public:
        BooleanQuery* analyze(const char* query_str) {
            BooleanQuery* result_query = new BooleanQuery();

            if(query_str == nullptr)
                return nullptr;

            std::string query_string(query_str);
            
            BooleanQuery* operators = extractOperators(query_string);
            if(operators) {

            }

            BooleanQuery* keywords = extractKeywords(query_string);
            if(keywords) {
                QueryListType::iterator iter = keywords->getQueryList().begin();

                for(;iter!=keywords->getQueryList().end(); iter++) {
                    result_query->add(*iter);
                }

                delete keywords;
            }

            return result_query;
        }
    };

    BooleanQuery* analyze(const char* query_str) {
        BooleanQuery* result_querys = new BooleanQuery();
        BooleanQuery* tmp_querys = new BooleanQuery();

        // 쿼리들을 모두 체크하여 2차 검색용이 포함된 쿼리를
        // 뒤쪽에 나오도록 정렬한다.
        BooleanQuery* bq = _internal_QueryAnalyzer().analyze(query_str);

        QueryListType::iterator iter = bq->getQueryList().begin();
        for(;iter!=bq->getQueryList().end(); iter++) {
            Query* query = (*iter).query_;
            if(strcmp(query->getQueryName(), "TermQuery" ) == 0) {
                result_querys->add(*iter);
            } else if(strcmp(query->getQueryName(), "BooleanQuery") == 0) {
                BooleanQuery* sub_bq = (BooleanQuery*)query;

                QueryListType::iterator sub_iter = sub_bq->getQueryList().begin();
                for(;sub_iter!=sub_bq->getQueryList().end(); sub_iter++) {
                    if(strcmp((*sub_iter).query_->getQueryName(), "RangeQuery") == 0) {
                        tmp_querys->add(*iter);
                        break;
                    }
                }
                if(sub_iter==sub_bq->getQueryList().end()) {
                    result_querys->add(*iter);
                }
            } else {
                tmp_querys->add(*iter);
            }
        }

        QueryListType::iterator tmp_iter = tmp_querys->getQueryList().begin();
        for(;tmp_iter!=tmp_querys->getQueryList().end(); tmp_iter++) {
            result_querys->add(*tmp_iter);
        }

        return result_querys;
    }
};

#endif /* __fast_searcher_index_query_analyzer_h_included__ */
