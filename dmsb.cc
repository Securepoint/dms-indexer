#include "xapian-search.h"
#include <string>   // string, stoi
#include <iostream> // cout
#include <algorithm> // vector

std::vector < SP_SearchResult > search_document(SP_Search Search,
  unsigned int startset, unsigned int *totalresults, string * corrected);

static void help(){
	std::cout << "Usage: dms-cmdl XAPIAN-DATABASE SERACH-QUERY [...] \n"
	<< " Arguments:\n"
	<< " 1. XAPIAN-DATABASE path to xapian dir (string)\n"
	<< " 2. SERACH-QUERY (string)\n"
	<< " Optional:\n"
	<< " 3. Offset (int)>\n"
	<< " 4. Number of results (int)>\n"
	<< " 5. Collapse by x (string)  hash, uid, msgid \n"
	<< " 6. Relevant document id's  (int[] terminated by ';'   e.g.: '2' '23' ... ';' )\n"
	<< " 7. Sort results by (<sp_xapian_slot_id(int) bool>[]   e.g.: '10' '0' '12' '1'... )\n\n"
	<< "    sp_xapian_slot: DATE(10), UID(11), DOCNAME(12), USER(13),\n"
	<< "                    FOLDER(15), MIMEID(16), FILETYPE(17),\n"
	<< "                    MSGID(18), REFERENCES(19), REPLYTO(20),\n"
	<< "                    LTA(21), MSG_SID(22), SIZE(23), IMAPID(24),\n"
	<< "                    MD5HASH(25), UNIQUEID(26), DATE_FROM_SID(27),\n"
	<< "                    LTA_UID(28), KEYWORDS(29), DATE_SEC(30),\n"
	<< "                    EMAIL_FROM(31), EMAIL_TO(32), EMAIL_CC(33),\n"
	<< "                    EMAIL_BCC(34), EMAIL_SUBJECT(35), MIMETYPE(36),\n";
}

static void errn(){
	std::cerr<<"search error\n";
}

int main(int argc, char* argv[]){
	int i=0;
	void (*err)() = help;
	std::string progn;
	if(i<argc) { progn = argv[i++];}
	if(progn == "dmsb") { err = errn; }
	if (argc < 3) { return (err(),1);};
	try{
		SP_Search search((i<argc)? argv[i++]:"");
		if(i<argc) { search.add_query( argv[i++]);}

		int start = 0;
		if(i<argc) { start = std::stoi(argv[i++]); }
		if(i<argc) { search.set_howmanyresults(std::stoi(argv[i++])); }
		if(i<argc) { std::string col = argv[i++]; search.set_collapse(col); }
		while(i<argc && argv[i][0] != ';'){ search.push_relevance_doc(std::stoi(argv[i++])); }
		std::vector < std::pair < sp_xapian_slot, bool > >sort_by_slot_reverse;
		++i;
		while(i < argc-1){
			sp_xapian_slot eslot = (sp_xapian_slot)std::stoi(argv[i++]);
			if(eslot < sp_xapian_slot::FIRST || eslot > sp_xapian_slot::LAST){ return (err(),3); }
			bool reverse = (bool)std::stoi(argv[i++]);
			sort_by_slot_reverse.push_back(std::make_pair(eslot,reverse));
		}
		search.setSortbySlotReverse(sort_by_slot_reverse);

		unsigned int totalresults;
		string corrected;
		std::vector <SP_SearchResult> Results = search_document(search,start,&totalresults,&corrected);
		std::cerr<<"total:"<<totalresults<<"\n";
		std::cerr<<corrected<<"\n";

		while (!Results.empty()) {
			SP_SearchResult result = Results.back();
			Results.pop_back();
			std::cout
			<< result.get_docid()        << "\n"
			<< result.get_uid()          << "\n"
			<< result.get_mimeid()       << "\n"
			<< result.get_percent()      << "\n"
			<< result.get_date()         << "\n"
			<< result.get_docname()      << "\n"
			<< result.get_filetype()     << "\n"
			<< result.get_folder()       << "\n"
			<< result.get_category()     << "\n"
			<< result.get_size()         << "\n"
			<< result.get_username()     << "\n"
			<< result.get_received_date()<< "\n"
			<< result.get_date_secs()    << "\n"
			<< result.get_keywords()     << "\n"
			<< result.get_mimetype()     << "\n"
			<< result.get_email_from()   << "\n"
			<< result.get_email_to()     << "\n"
			<< result.get_email_cc()     << "\n"
			<< result.get_email_bcc()    << "\n"
			<< result.get_email_subject()<< "\n";

			for(auto it= result.get_matched_words().begin(); it !=  result.get_matched_words().end();it++){
				std::cout<<*it<<",";
			}
			std::cout<<"\n";
			try{ std::cout<<search.db.get_document(result.get_docid()).get_data(); }catch(...){}
			std::cout<<"\n---\n";
		}
	}
	catch(...){ return (err(),2);}
}
