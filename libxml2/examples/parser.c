#include <libxml/parser.h>
#include <libxml/xmlmemory.h>
#include <libxml/xpath.h>
#include <libxml/xmlschemas.h>
#include <string.h>

static void traverseNodes(xmlNodePtr node, int height)
{
	xmlNodePtr cur = NULL;
	int i = height;

	for (cur = node; cur; cur = cur->next) {
		if (!strncmp((char *)cur->name, "group_id", 9)) {
			while (i--) printf("\t");
			printf("%s\n", cur->children->content);
		}

		traverseNodes(cur->children, height + 1);
	}
}

int main(int argc, char *argv[])
{
	xmlDocPtr doc;
	xmlXPathContextPtr xpath;
	xmlXPathObjectPtr xpathObj;
	xmlChar *ex;

	if (argc != 2) {
		printf("usage: %s file\n", argv[0]);
		return -1;
	}

	LIBXML_TEST_VERSION;

	/* Init */
	xmlInitParser();
	doc = xmlParseFile(argv[1]);
	xpath = xmlXPathNewContext(doc);

	/* XPath */
	ex = (xmlChar *)"/group_info/group_info";
	xpathObj = xmlXPathEvalExpression(ex, xpath);

	/* Parse */
	traverseNodes(xpathObj->nodesetval->nodeTab[0], -1);

	/* Free */
	xmlXPathFreeObject(xpathObj);
	xmlFreeDoc(doc);
	xmlXPathFreeContext(xpath);
	xmlCleanupParser();

	/* Debugging */
	xmlMemoryDump();

	return 0;
}
