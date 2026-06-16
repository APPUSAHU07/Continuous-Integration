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
 *   This file contains the implementation for the Extension C9testattachdataset
 *
 */
#include <C9testlib/C9testattachdataset.hxx>
#include <ug_va_copy.h>
#include <tccore/item.h>
#include <tccore/aom.h>
#include <tccore/grm.h>
#include <tccore/method.h>
#include <tc/emh.h>
#include <tccore/tctype.h>
#include <tccore/aom_prop.h>
#include <ae/dataset.h>
#include <ae/ae.h>
#include <fclasses/tc_stdarg.h>

int C9testattachdataset( METHOD_message_t * /*msg*/, va_list args)
{
		TC_write_syslog("\n Entering C9testattachdataset");
	    va_list largs;
	    va_copy(largs, args);

	    const char* item_id = va_arg(largs, const char*);  // item_id

	    // consume but ignore (must preserve order)
	    (void)va_arg(largs, const char*); // item_name
	    (void)va_arg(largs, const char*); // type_name
	    (void)va_arg(largs, const char*); // rev_id

	    tag_t* new_item_tag_p = va_arg(largs, tag_t*);
	    tag_t* new_rev_tag_p  = va_arg(largs, tag_t*);

	    (void)va_arg(largs, tag_t); // item_master_form_tag
	    (void)va_arg(largs, tag_t); // item_rev_master_form_tag

	    va_end(largs);

	    const tag_t item_tag = (new_item_tag_p ? *new_item_tag_p : NULLTAG);
	    const tag_t item_rev = (new_rev_tag_p  ? *new_rev_tag_p  : NULLTAG);
	    TC_write_syslog("\n C9testattachdataset Checking for NULLTAG");
	    if (item_tag == NULLTAG || item_rev == NULLTAG)
	    {
	        TC_write_syslog("\n C9testattachdataset Missing new item/revision tag. Skipping.\n");
	        return ITK_ok;
	    }

	    TC_write_syslog("\n C9testattachdataset Initializing values for dataset");
	    tag_t datasetType  = NULLTAG;
	    tag_t createInput  = NULLTAG;
	    tag_t newDataset   = NULLTAG;
	    tag_t relationType = NULLTAG;
	    tag_t relation     = NULLTAG;

	    int ifail = ITK_ok;
	    TC_write_syslog("\n C9testattachdataset finding MSWord dataset type");
	    ifail = TCTYPE_find_type("MSWord", "", &datasetType);
	    if (ifail != ITK_ok || datasetType == NULLTAG)
	    {
	        TC_write_syslog("\n C9testattachdataset MSWord type not found. Skipping.\n");
	        return ITK_ok;
	    }

	    if (TCTYPE_construct_create_input(datasetType, &createInput) != ITK_ok || createInput == NULLTAG)
	        return ITK_ok;

	    // Optional: name using item_id
	    char ds_name[256] = {0};
	    snprintf(ds_name, sizeof(ds_name), "%s-Doc", (item_id && *item_id) ? item_id : "NEW");

	    TC_write_syslog("\n C9testattachdataset Setting values for dataset");

	    AOM_set_value_string(createInput, "object_name", ds_name);
	    AOM_set_value_string(createInput, "object_desc", "Auto created Word document");

	    if (TCTYPE_create_object(createInput, &newDataset) != ITK_ok || newDataset == NULLTAG)
	        return ITK_ok;

	    AOM_save_without_extensions(newDataset);
	    AOM_unlock(newDataset);
	    TC_write_syslog("\n C9testattachdataset Attaching dataset via IMAN_specification");
	    // --- Attach via IMAN_specification ---
	    if (GRM_find_relation_type("IMAN_specification", &relationType) == ITK_ok && relationType != NULLTAG)
	    {
	        if (GRM_create_relation(item_rev, newDataset, relationType, NULLTAG, &relation) == ITK_ok)
	        {
	            (void)GRM_save_relation(relation);
	        }
	    }

	    TC_write_syslog("\n C9testattachdataset Word dataset '%s' attached to new revision.\n", ds_name);
	    return ITK_ok;
	}
