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
 *   This file contains the implementation for the Extension C9testExportBom
 *
 */
#include <C9testlib/C9testExportBom.hxx>
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

int ExportBom(EPM_action_message_t msg);
void traverse_bom_line(tag_t line, FILE *fp);

int C9testExportBom( METHOD_message_t * /*msg*/, va_list /*args*/ )
{
	EPM_register_action_handler("Export BOM Structure","Export Item Revision structure to text file",ExportBom);
	return 0;

}

int ExportBom(EPM_action_message_t msg)
{
    TC_write_syslog("[ExportStructure] Handler started\n");

    tag_t root_task = NULLTAG;
    tag_t *targets = NULL;
    int target_count = 0;

    EPM_ask_root_task(msg.task, &root_task);

    TC_write_syslog("[ExportStructure] Root task obtained\n");

    EPM_ask_attachments(
        root_task,
        EPM_target_attachment,
&target_count,
&targets
    );

    TC_write_syslog("[ExportStructure] Target count: %d\n", target_count);

    for (int i = 0; i < target_count; i++)
    {
        char *type = NULL;

        AOM_ask_value_string(targets[i], "object_type", &type);

        TC_write_syslog("[ExportStructure] Target object type: %s\n", type);

        if (tc_strcmp(type, "ItemRevision") != 0)
        {
            MEM_free(type);
            TC_write_syslog("[ExportStructure] Skipping non ItemRevision\n");
            continue;
        }

        MEM_free(type);

        TC_write_syslog("[ExportStructure] Processing ItemRevision\n");

        /* Create BOM Window */

        tag_t window = NULLTAG;
        tag_t rule = NULLTAG;
        tag_t top_line = NULLTAG;

        BOM_create_window(&window);

        TC_write_syslog("[ExportStructure] BOM window created\n");

        CFM_find("Latest Working", &rule);

        BOM_set_window_config_rule(window, rule);

        TC_write_syslog("[ExportStructure] Config rule set\n");

        BOM_set_window_top_line(
            window,
            NULLTAG,
            targets[i],
            NULLTAG,
&top_line
        );

        TC_write_syslog("[ExportStructure] Top BOM line created\n");

        char filepath[256];
        sprintf(filepath, "C:\\Temp\\bom_structure_%d.txt", i);

        FILE *fp = TC_fopen(filepath, "w");

        if (!fp)
        {
            TC_write_syslog("[ExportStructure] Failed to open file\n");
            continue;
        }

        fprintf(fp,
        "Object String | Find No | Quantity | Object Type\n");

        TC_write_syslog("[ExportStructure] Starting BOM traversal\n");

        traverse_bom_line(top_line, fp);

        fclose(fp);

        TC_write_syslog("[ExportStructure] Structure written to file\n");

        BOM_close_window(window);

        TC_write_syslog("[ExportStructure] BOM window closed\n");

        /* Create dataset */

        tag_t dataset_type = NULLTAG;
        tag_t dataset = NULLTAG;

        AE_find_datasettype2("Text", &dataset_type);

        TC_write_syslog("[ExportStructure] Dataset type found\n");

        AE_create_dataset_with_id(
            dataset_type,
            "BOMStructureReport",
            "BOMStructureReport",
            "BOMStructureReport",
            "A",
&dataset
        );

        TC_write_syslog("[ExportStructure] Dataset created\n");

        AE_import_named_ref(
            dataset,
            "TEXT",
            filepath,
            "structure.txt",
            SS_TEXT
        );

        TC_write_syslog("[ExportStructure] Named reference imported\n");

        tag_t relation_type = NULLTAG;
        tag_t relation = NULLTAG;

        GRM_find_relation_type(
            "IMAN_specification",
&relation_type
        );

        GRM_create_relation(
            targets[i],
            dataset,
            relation_type,
            NULLTAG,
&relation
        );

        TC_write_syslog("[ExportStructure] Dataset attached to ItemRevision\n");

        AOM_save_without_extensions(dataset);
        AOM_save_without_extensions(targets[i]);
    }

    if (targets)
        MEM_free(targets);

    TC_write_syslog("[ExportStructure] Handler completed\n");

    return EPM_go;
}

void traverse_bom_line(tag_t line, FILE *fp)
{
    char *obj_string = NULL;
    char *find_no = NULL;
    char *qty = NULL;
    char *type = NULL;

    AOM_ask_value_string(line, "bl_indented_title", &obj_string);
    AOM_ask_value_string(line, "bl_find_no", &find_no);
    AOM_ask_value_string(line, "bl_quantity", &qty);
    AOM_ask_value_string(line, "object_type", &type);

    fprintf(fp,
            "%s | %s | %s | %s\n",
            obj_string,
            find_no,
            qty,
            type);

    int count = 0;
    tag_t *children = NULL;

    BOM_line_ask_child_lines(
        line,
&count,
&children
    );

    for (int i = 0; i < count; i++)
        traverse_bom_line(children[i], fp);

    if (children)
        MEM_free(children);

    if (obj_string) MEM_free(obj_string);
    if (find_no) MEM_free(find_no);
    if (qty) MEM_free(qty);
    if (type) MEM_free(type);
}
