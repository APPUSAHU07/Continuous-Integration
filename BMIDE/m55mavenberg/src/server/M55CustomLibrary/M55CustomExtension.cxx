#include <M55CustomLibrary/M55CustomExtension.hxx>
#include <ug_va_copy.h>
#include <tccore/item.h>
#include <tccore/aom.h>
#include <tccore/grm.h>
#include <tccore/method.h>

int M55CustomExtension( METHOD_message_t * /*msg*/, va_list args )
{
		TC_write_syslog("Entering M55CustomExtension");
	    va_list largs;
	    va_copy(largs, args);
	    tag_t item_tag = va_arg(largs, tag_t);

	    tag_t item_rev = NULLTAG;
	    va_end(largs);
	    ITEM_ask_latest_rev(item_tag, &item_rev);
	    tag_t doc_item = NULLTAG;
	    tag_t doc_rev  = NULLTAG;
	    TC_write_syslog("Creating a new Document");
	    ITEM_create_item(
	        "Devops",
	        "New Doc",
	        "Document",
	        "A",
&doc_item,
&doc_rev
	    );
	    AOM_save_without_extensions(doc_item);
	    AOM_save_without_extensions(doc_rev);
	    tag_t relation_type = NULLTAG;
	    tag_t relation = NULLTAG;

	    TC_write_syslog("Finding Relation type");

	    GRM_find_relation_type("IMAN_specification", &relation_type);

	    TC_write_syslog("Creating a new Relation");
	    GRM_create_relation(
	        item_rev,
	        doc_rev,
			relation_type,
	        NULLTAG,
&relation
	    );
	    TC_write_syslog("Saving the relation");
	    GRM_save_relation(relation);

	    return ITK_ok;
}
