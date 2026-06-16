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
 *   This file contains the implementation for the Extension C9testValidateBool
 *
 */
#include <C9testlib/C9testValidateBool.hxx>
#include <epm/epm.h>
#include <tc/emh.h>
#include <tccore/item.h>
#include <tccore/aom.h>
#include <tccore/grm.h>
#include <tccore/aom_prop.h>
#include <tccore/WorkspaceObject.h>
#include <sa/tcfile.h>
#include <sa/sa.h>
#include <sa/sa_errors.h>
#include <sa/role.h>
#include <sa/group.h>
#include <sa/groupmember.h>
#include <sa/user.h>
#include <ae/ae.h>
#include <fclasses/tc_date.h>
#include <fclasses/tc_string.h>
#include <bom/bom.h>
#include <cfm/cfm.h>
#include <fclasses/tc_date.h>
#include <fclasses/tc_stdlib.h>

#define ITK_CALL(X) 										  \
if((ifail = (X)) != ITK_ok) { 								  \
    printf("Error %d at %s:%d\n", ifail, __FILE__, __LINE__); \
    return ifail; 											  \
}

int ValidateBool(EPM_action_message_t msg);

int C9testValidateBool( METHOD_message_t * /*msg*/, va_list /*args*/ )
{
	EPM_register_action_handler("Validate Bool", "Workflow assigned to different roles based on boolean value", ValidateBool);

	return 0;

}

int ValidateBool(EPM_action_message_t msg)
{
	int ifail = ITK_ok;

	    tag_t rootTask = NULLTAG;
	    tag_t *targets = NULL;
	    int targetCount = 0;

	    logical boolValue = FALSE;

	    tag_t groupTag = NULLTAG;
	    tag_t roleTag = NULLTAG;
	    tag_t *groupMembers = NULL;
	    int groupCount = 0;

	    char signoffRequired[] = "RequiredModifiable";


	    ITK_CALL(EPM_ask_root_task(msg.task, &rootTask));


	    ITK_CALL(EPM_ask_attachments(rootTask,
	                                 EPM_target_attachment,
	                                 &targetCount,
	                                 &targets));

	    if(targetCount == 0)
	        return ITK_ok;


	    ITK_CALL(AOM_ask_value_logical(targets[0],
	                                   "c9Boolean_Property",
	                                   &boolValue));
	    TC_write_syslog("This is the Boolean Value %s",boolValue);

	    ITK_CALL(SA_find_group("Engineering", &groupTag));

	    TC_write_syslog("Finding group and role");
	    if(boolValue == FALSE){
	        ITK_CALL(SA_find_role2("Designer", &roleTag));
	    }
	    else
	    	ITK_CALL(SA_find_role2("Sam", &roleTag));

	    TC_write_syslog("Finding groupmember with those roles");
	    ITK_CALL(SA_find_groupmember_by_role(
	                groupTag,
	                roleTag,
	                &groupCount,
	                &groupMembers));

	    if(groupCount > 0)
	    {
	    	TC_write_syslog("Assign the task to the roles");
	        ITK_CALL(EPM_add_reviewers_on_task_with_signoff(
	                    rootTask,
	                    TRUE,
	                    groupCount,
	                    groupMembers,
	                    signoffRequired));

	        ITK_CALL(EPM_set_adhoc_signoff_selection_done(rootTask, TRUE));
	    }


	    if(targets) MEM_free(targets);
	    if(groupMembers) MEM_free(groupMembers);

	    return ITK_ok;
}
