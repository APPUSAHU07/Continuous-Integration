//@<COPYRIGHT>@
//==================================================
//Copyright $2026.
//Siemens Product Lifecycle Management Software Inc.
//All Rights Reserved.
//==================================================
//@<COPYRIGHT>@

/* 
 * @file 
 *
 *   This file contains the implementation for the Extension C9testregisterhandler
 *
 */
#include <C9testlib/C9testregisterhandler.hxx>
#include <epm/epm.h>
#include <tc/emh.h>
#include <tccore/item.h>
#include <tccore/aom.h>
#include <tccore/aom_prop.h>
#include <tccore/WorkspaceObject.h>
#include <sa/tcfile.h>
#include <fclasses/tc_date.h>
#include <fclasses/tc_string.h>

#define ERROR_INVALID_DATE  919001

EPM_decision_t RuleHandlerFunc(EPM_rule_message_t msg);
EPM_decision_t ValidateCreationDate(EPM_rule_message_t msg);

int C9testregisterhandler( METHOD_message_t * /*msg*/, va_list /*args*/ )
{
	TC_write_syslog("Entering C9testregisterhandler function");
 
	EPM_register_rule_handler("MyRuleHandler", "Custom Rule Handler", RuleHandlerFunc);

	EPM_register_rule_handler("Validate Date", "Validate the creation date of the object",ValidateCreationDate);

	TC_write_syslog("Exiting C9testregisterhandler function");
	return 0;
}

EPM_decision_t RuleHandlerFunc(EPM_rule_message_t msg)
{
	TC_write_syslog("Entering RuleHandlerFunc function");

	EPM_decision_t status = EPM_go;

	// My functionality

	tag_t tTask = msg.task;

		status = EPM_nogo;


	TC_write_syslog("Exiting RuleHandlerFunc function %d", tTask);
	return status;
}

EPM_decision_t ValidateCreationDate(EPM_rule_message_t msg){

		TC_write_syslog("Entering ValidateCreationDate function");
					int ifail = ITK_ok;
					EPM_decision_t decision = EPM_go;
				    int targetCount = 0;
				    tag_t *targets = NULL;

				    date_t creationDate;
				    char* dateString = NULL;

				    /* Get root task */
				    tag_t rootTask = NULLTAG;
				    ifail = EPM_ask_root_task(msg.task, &rootTask);
				    if (ifail != ITK_ok){
				    	 TC_write_syslog("EPM_ask_root_task failed");
				    	 return EPM_nogo;
				    }

				    /* Get target attachments */
				    ifail = EPM_ask_attachments(rootTask, EPM_target_attachment,
				                                 &targetCount, &targets);
				    if (ifail != ITK_ok){
				    	 TC_write_syslog("EPM_ask_attachments failed");
				    	 return EPM_nogo;
				    }


				        for (int i = 0; i < targetCount; i++)
				        {
				            char *typeName = NULL;

				    ifail = WSOM_ask_object_type2(targets[i], &typeName);
				    if (ifail != ITK_ok){
				    	TC_write_syslog("WSOM_ask_object_type2 failed");
				    	return EPM_nogo;
				    }

				            TC_write_syslog("Checking for object type");


				    if (tc_strstr(typeName, "Revision") != NULL)
				            {
				            	ifail = AOM_ask_value_date(targets[i], "creation_date", &creationDate);
				            	if (ifail != ITK_ok){
				            		TC_write_syslog("AOM_ask_value_date failed");
				            		return EPM_nogo;
				            	}

				            	ifail = DATE_date_to_string(creationDate,"%d-%b-%Y %H:%M:%S", &dateString);
				            	if (ifail != ITK_ok){
				            	TC_write_syslog("DATE_date_to_string failed");
				            	return EPM_nogo;
				            	}

				                TC_write_syslog("%s",dateString);
				                TC_write_syslog("Validating the date");

				                if (tc_strstr(dateString, "Jan-2026") == NULL)
				                {
				                	TC_write_syslog("The object was not created within January 2026.");
				                    EMH_store_error_s1(
				                        EMH_severity_error,
				                        ERROR_INVALID_DATE,
				                        "The object was not created within January 2026."
				                    );

				                    decision = EPM_nogo;
				                    MEM_free(typeName);
				                    break;
				                }
				            }

				            MEM_free(typeName);
				        }

				    if (targets) MEM_free(targets);
				    MEM_free(dateString);
		TC_write_syslog("Exiting ValidateCreationDate function");
		return decision;
}
