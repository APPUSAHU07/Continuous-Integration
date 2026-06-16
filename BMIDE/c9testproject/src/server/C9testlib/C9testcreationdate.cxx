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
 *   This file contains the implementation for the Extension C9testcreationdate
 *
 */
#include <C9testlib/C9testcreationdate.hxx>
#include <epm/epm.h>
#include <tc/emh.h>
#include <tccore/item.h>
#include <tccore/aom.h>
#include <tccore/grm.h>
#include <tccore/aom_prop.h>
#include <tccore/WorkspaceObject.h>
#include <sa/tcfile.h>
#include <ae/ae.h>
#include <fclasses/tc_date.h>
#include <fclasses/tc_string.h>

#define ERROR_INVALID_DATASET  100001

EPM_decision_t CheckNamedReference(EPM_rule_message_t msg);

int C9testcreationdate( METHOD_message_t * /*msg*/, va_list /*args*/ )
{
	TC_write_syslog("Entering C9testcreationdate function");
	EPM_register_rule_handler("CheckNamedReference", "Released if the attached dataset has named reference", CheckNamedReference);
	TC_write_syslog("Exiting C9testcreationdate function");
 return 0;

}

EPM_decision_t CheckNamedReference(EPM_rule_message_t msg)
{
	TC_write_syslog("Entering CheckNamedReference function");
	int ifail = ITK_ok;
	    EPM_decision_t decision = EPM_go;

	    tag_t trootTask = NULLTAG;
	    tag_t *targets = NULL;
	    int targetCount = 0;



	    ifail = EPM_ask_root_task(msg.task, &trootTask);
	    if (ifail != ITK_ok) return EPM_nogo;


	    ifail = EPM_ask_attachments(trootTask,
	                                EPM_target_attachment,
	                                &targetCount,
	                                &targets);
	    if (ifail != ITK_ok) return EPM_nogo;

	    for (int i = 0; i < targetCount; i++)
	    {

	        tag_t *secondaryObjs = NULL;
	        tag_t ttypeTag = NULLTAG;
	        int secCount = 0;

	        /* Get attached datasets */
	        TC_write_syslog("Getting list of Secondary objects");
	        ifail = GRM_list_secondary_objects_only(
	                    targets[i],
	                    NULLTAG,
	                    &secCount,
	                    &secondaryObjs);

	        if (ifail != ITK_ok)
	        {
	            decision = EPM_nogo;
	            break;
	        }

	        logical namedRefFound = FALSE;

	        TC_write_syslog("Checking the type of Secondary object");

	        for (int j = 0; j < secCount; j++)
	        {
	            logical isDataset = FALSE;
	            TCTYPE_ask_object_type(secondaryObjs[j],&ttypeTag);
	            ifail = TCTYPE_is_type_of_as_str(ttypeTag,"Dataset",&isDataset);
	            if (isDataset)
	            {
	            	TC_write_syslog("Finding named reference");
	                int refCount = 0;
	                AE_ask_dataset_ref_count(secondaryObjs[j], &refCount);

	                if (refCount > 0)
	                {
	                    namedRefFound = TRUE;
	                    break;
	                }
	            }
	        }

	        if (secondaryObjs) MEM_free(secondaryObjs);

	        if (!namedRefFound)
	        {
	        	TC_write_syslog("No named reference found");
	            EMH_store_error_s1(
	                EMH_severity_error,
	                ERROR_INVALID_DATASET,
	                "Release blocked: No named reference found in attached dataset."
	            );

	            decision = EPM_nogo;
	            break;
	        }
	    }

	    if (targets) MEM_free(targets);

	    TC_write_syslog("Exiting CheckNamedReference");
	    return decision;
}
