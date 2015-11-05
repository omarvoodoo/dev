
#ifndef __fast_searcher_index_document_test_h_included__
#define __fast_searcher_index_document_test_h_included__

#include "token.h"
#include "field/field.h"
#include "document.h"
#include "source_list.h"
#include "index\query_analyzer.h"
#include <vector>

class DocumentTest {
public:
	static void foo() {
        SourceList source_list;

        const int raw_data_count = 10;
        char* raw_data_list[raw_data_count] = {
            "2014/03/05 20:57:40.160	222.238.66.46 	 Facility[0]	SeverityLevel[0]	Message[GPON[212]: DISCOVERED serial-number DGNT11370B9C (olt 2/2)]",
            "2014/03/05 20:57:40.161	114.202.155.100 	 Facility[1]	SeverityLevel[111]	Message[SYS-1-TIMEOUT: DEV_BCM_LOCK : it seems stuck! starting at _bcm_l2_addr_callback 613 in bcmARL.0(c384d090). elapsed 11] ",
            "2014/03/05 20:57:40.161	180.66.30.13 	 Facility[2]	SeverityLevel[112]	Message[SWCH[105]: SWCH: Temperature load(65) under-flowed than threshold 75(high)] ",
            "2014/03/05 20:57:39.977	175.113.181.243 	 Facility[3]	SeverityLevel[113]	Message[system: port 17 olr event : SRA updated rate(19134kbps) in upstream] strkey[hehehe] ",
            "2014/03/05 20:57:39.976	211.201.128.239 	 Facility[4]	SeverityLevel[114]	Message[system: erasing and writing flash(configs) : start] ",
            "2014/03/05 20:57:39.966	58.123.159.141 	 Facility[5]	SeverityLevel[115]	Message[telnetd[11003]: ttloop: peer died: EOF ] ",
            "2014/03/05 20:57:39.946	1.226.225.97 	 Facility[6]	SeverityLevel[116]	Message[last message repeated 56 times] ",
            "2014/03/05 20:57:39.939	58.123.72.118 	 Facility[7]	SeverityLevel[117]	Message[vtysh: erasing and writing flash(configs) : request] ",
            "2014/03/05 20:57:38.977	175.112.94.213 	 Facility[8]	SeverityLevel[118]	Message[portd: Port 2: Link Down] ",
            "2014/03/05 20:57:37.939	175.117.89.34 	 Facility[9]	SeverityLevel[119]	Message[system: port 1 link opern(oational)] strkey[hohoho]"
        };

        Source* source = source_list.getSource("default_source");
        for(int i=0; i<raw_data_count; ++i) {
            source->addDocument(raw_data_list[i]);
        }

        //BooleanQuery* result_query = QueryAnalyzer::analyze("CC | AA and BB or Field > 100 | DD | zz and yy or xield > BAA | FFF > 1000");

        BooleanQuery* result_query = QueryAnalyzer::analyze("Facility | system or port");
        source->search(result_query);

		int a = 0;
	}
};

#endif /* __fast_searcher_index_document_test_h_included__ */