BEGIN			{
			print "//patched by awk 1";
			print "#include \"raslib/error.hh\""
			print "#include \"servercomm/servercomm.hh\""
			print "#include <new>"
			print "char *secureResultBufferForRPC;"
			print "bool bMemFailed = false;"
			print "//end patched by awk 1";
			}
/result\ =\ .*local/	{
			print "//patched by awk 2";
			print "\ttry\t{";
			print "//end patched by awk 2";
			print $0;
			print "//patched by awk 2.1";
			print "\t\t}";
			print "\tcatch (r_Error& e)";
			print "\t\t{";
			print "\t\tretvalTxt = e.serialiseError();";
			print "\t\tu_short temp = 42;";
			print "\t\tresult = secureResultBufferForRPC;";
			print "\t\tmemcpy(result, (char*)&temp, sizeof(u_short));"
			print "\t\t}";
			print "\tcatch (std::bad_alloc)";
			print "\t\t{";
			print "\t\tbMemFailed = true;"
			print "\t\tServerComm* sc = ServerComm::actual_servercomm;"			
			print "\t\tr_Ememory_allocation e;"
			print "\t\tretvalTxt = e.serialiseError();"
			print "\t\tu_short temp = 42;";
			print "\t\tresult = secureResultBufferForRPC;"
			print "\t\tmemcpy(result, (char*)&temp, sizeof(u_short));"
			print "\t\t"
			print "\t\t}";
			print "//end patched by awk 2.1";
			next;
			}
/^void$/		{
			print "//patched by awk 3";
			print "char\*";
			print "//end patched by awk 3";
			next;
			}
/^{$/			{
			print $0;
			print "//patched by awk 4";
			print "char\* retvalTxt = 0;";
			print "//end patched by awk 4";
			next;
			}
/return;/		{
			print "//patched by awk 5";
			print "return retvalTxt;";
			print "//end patched by awk 5";
			next;
			}
			{
			print $0;
			}
