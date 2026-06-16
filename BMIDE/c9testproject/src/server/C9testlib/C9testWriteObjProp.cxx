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
 *   This file contains the implementation for the Extension C9testWriteObjProp
 *
 */
#include <C9testlib/C9testWriteObjProp.hxx>
#include <epm/epm.h>
#include <tccore/aom.h>
#include <tccore/aom_prop.h>
#include <tccore/grm.h>
#include <ae/dataset.h>
#include <ae/ae.h>
#include <user_exits/user_exits.h>
#include <res/res_itk.h>
#include <base_utils/mem.h>

/* Error stack printer */
static int print_error_stack(void)
{
    int nErr = 0;
    const int *sev = NULL, *codes = NULL;
    const char **msgs = NULL;

    EMH_ask_errors(&nErr, &sev, &codes, &msgs);
    if (nErr > 0) {
        TC_write_syslog("\n--- ITK Error Stack Trace Begin ---\n");
        for (int i = 0; i < nErr; i++) {
            TC_write_syslog("  [Error %d] %s\n", codes[i], msgs[i]);
        }
        TC_write_syslog("--- ITK Error Stack Trace End ---\n\n");
    }
    return ITK_ok;
}

/* Macro for standardized error handling */
#define IFERR_ABORT(call)                      \
    do {                                       \
        ifail = (call);                        \
        if (ifail != ITK_ok) {                 \
            print_error_stack();               \
            goto CLEANUP;                      \
        }                                      \
    } while (0)

int WriteObjProp(EPM_action_message_t msg);

int C9testWriteObjProp( METHOD_message_t * /*msg*/, va_list /*args*/ )
{
	EPM_register_action_handler("Write Prop","Writes the object properties on a CSV file",WriteObjProp);
    return 0;

}

int WriteObjProp(EPM_action_message_t msg)
{
    int ifail = ITK_ok;

    int count = 0;
    int secondary_count = 0;
    tag_t root_task = NULL_TAG;
    tag_t target_object = NULL_TAG;
    tag_t* attachments = NULL;
    tag_t* secondary_objects = NULL;
    tag_t dataset = NULL_TAG;
    tag_t relation_type = NULL_TAG;
    tag_t dataset_type = NULL_TAG;
    tag_t rel = NULL_TAG;
    tag_t target_rel = NULL_TAG;
    FILE* csv_file = NULL;
    char* file_path = "workflow_cumulative_report.csv";
    char* id = NULL;
    char* name = NULL;
    char* type = NULL;
    char* obj_str = NULL;
    char* desc = NULL;
    char* date = NULL;
    logical exists = FALSE;

    // 1. Get Root Task and targets
    IFERR_ABORT(EPM_ask_root_task(msg.task, &root_task));
    IFERR_ABORT(EPM_ask_attachments(msg.task, EPM_target_attachment, &count, &attachments));

    if (count == 0)
    {
        ifail = EPM_nogo;
        goto CLEANUP;
    }
    target_object = attachments[0];

    // 2. Locate existing Dataset on Root Task
    IFERR_ABORT(GRM_find_relation_type("IMAN_specification", &relation_type));
    IFERR_ABORT(GRM_list_secondary_objects_only(root_task, relation_type, &secondary_count, &secondary_objects));

    for (int i = 0; i < secondary_count; i++) {
        char* ds_name = NULL;
        IFERR_ABORT(AOM_ask_value_string(secondary_objects[i], "object_name", &ds_name));
        if (ds_name && strcmp(ds_name, "Workflow_Cumulative_Report") == 0) {
            dataset = secondary_objects[i];
            exists = TRUE;
            IFERR_ABORT(AE_export_named_ref(dataset, "Text", file_path));
        }
        if (ds_name) MEM_free(ds_name);
        if (exists) break;
    }

    // 3. File IO
    csv_file = fopen(file_path, exists ? "a" : "w");
    if (csv_file == NULL) {
        ifail = 1;
        goto CLEANUP;
    }
 
    if (!exists) {
        fprintf(csv_file, "ID,Name,Type,Object String,Description,Creation Date\n");
    }

    // 4. Retrieve Properties
    IFERR_ABORT(AOM_ask_value_string(target_object, "item_id", &id));
    IFERR_ABORT(AOM_ask_value_string(target_object, "object_name", &name));
    IFERR_ABORT(AOM_ask_value_string(target_object, "object_type", &type));
    IFERR_ABORT(AOM_ask_value_string(target_object, "object_string", &obj_str));
    IFERR_ABORT(AOM_ask_value_string(target_object, "object_desc", &desc));
    IFERR_ABORT(AOM_ask_value_string(target_object, "creation_date", &date));

    fprintf(csv_file, "%s,%s,%s,%s,%s,%s\n",
            id ? id : "", name ? name : "", type ? type : "",
            obj_str ? obj_str : "", desc ? desc : "", date ? date : "");

    // 5. Manage Dataset
    fclose(csv_file);
    csv_file = NULL;

    if (!exists) {
        IFERR_ABORT(AE_find_dataset2("Text", &dataset_type));
        IFERR_ABORT(AE_create_dataset_with_id(dataset_type, "Workflow_Cumulative_Report", "CSV Report", NULL, NULL, &dataset));
        IFERR_ABORT(GRM_create_relation(root_task, dataset, relation_type, NULL_TAG, &rel));
        IFERR_ABORT(GRM_save_relation(rel));
    }

    IFERR_ABORT(AE_import_named_ref(dataset, "Text", file_path, "report.csv", NULL));
    IFERR_ABORT(AE_save_myself(dataset));

    // 6. Link to Target
    IFERR_ABORT(GRM_create_relation(target_object, dataset, relation_type, NULL_TAG, &target_rel));
    IFERR_ABORT(GRM_save_relation(target_rel));

CLEANUP:
    if (csv_file) fclose(csv_file);
    if (secondary_objects) MEM_free(secondary_objects);
    if (attachments) MEM_free(attachments);
    if (id) MEM_free(id);
    if (name) MEM_free(name);
    if (type) MEM_free(type);
    if (obj_str) MEM_free(obj_str);
    if (desc) MEM_free(desc);
    if (date) MEM_free(date);

    return (ifail == ITK_ok) ? ITK_ok : EPM_nogo;
}
