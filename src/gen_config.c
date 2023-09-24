#include <stdlib.h>

#include <libxml/tree.h>
#include <libxml/parser.h>

//        s
//      (o5o)
//     /7(=)\_
// VermouthMoth

// compile
// $ gcc `xml2-config --cflags --libs` -o gen_config gen_config.c

// generate and save config file template
// if /path/to/config.dtd is not specified,
// src/config.dtd is set as default
// $ ./gen_config /path/to/config.dtd > config.xml

// validate config file
// $ xmllint --noout --valid config.xml

#if defined(LIBXML_TREE_ENABLED) && defined(LIBXML_OUTPUT_ENABLED)

#define PROPS_POINTER_MODE   10
#define PROPS_SCROLLING_MODE  7

static char *props_name_pointer_mode[PROPS_POINTER_MODE]
                  = {"POINTER_UP_KEY",
                     "POINTER_DOWN_KEY",
                     "POINTER_RIGHT_KEY",
                     "POINTER_LEFT_KEY",
                     "POINTER_MOVEMENT_SPEED",
                     "POINTER_SPEEDUP_KEY",
                     "POINTER_SPEEDUP_FACTOR",
                     "MOUSE_LEFT_BUTTON",
                     "MOUSE_RIGHT_BUTTON",
                     "MOUSE_MIDDLE_BUTTON"}; 

static char *props_value_pointer_mode[PROPS_POINTER_MODE]
                  = {"KEY_W",
                     "KEY_S",
                     "KEY_D",
                     "KEY_A",
                     "2",
                     "KEY_SLASH",
                     "5",
                     "KEY_COMMA",
                     "KEY_DOT",
                     "KEY_M"};

static char *props_name_scrolling_mode[PROPS_SCROLLING_MODE]
                  = {"SCROLLING_UP_KEY",
                     "SCROLLING_DOWN_KEY",
                     "SCROLLING_RIGHT_KEY",
                     "SCROLLING_LEFT_KEY",
                     "SCROLLING_SPEED",
                     "SCROLLING_SPEEDUP_KEY",
                     "SCROLLING_SPEEDUP_FACTOR"};

static char *props_value_scrolling_mode[PROPS_SCROLLING_MODE]
                  = {"KEY_W",
                     "KEY_S",
                     "KEY_D",
                     "KEY_A",
                     "25",
                     "KEY_SLASH",
                     "2"};

int main(int argc, char *argv[])
{
   xmlDocPtr doc = xmlNewDoc(BAD_CAST "1.0");
   xmlNodePtr root_node = xmlNewDocRawNode(doc, NULL,
                                           BAD_CAST "config", NULL);
   xmlDocSetRootElement(doc, root_node);

   // laod config.dtd and add as internal dtd subset
   char *dtd_file;
   if (argc < 2)
      dtd_file = "src/config.dtd";
   else
      dtd_file = argv[1];

   xmlDtdPtr dtd = xmlParseDTD(NULL, BAD_CAST dtd_file);
   dtd->name = BAD_CAST "config";
   dtd->SystemID = NULL;
   xmlAddPrevSibling(doc->children, (xmlNodePtr)dtd);

   // device
   xmlNewProp(root_node, BAD_CAST "DEVICE",
                           BAD_CAST "/path/to/your/keyboard");

   // pointer mode
   xmlNodePtr pointer_node = xmlNewChild(root_node, NULL,
                                         BAD_CAST "pointer_mode", NULL);
   xmlNewProp(pointer_node, BAD_CAST "POINTER_MODE_KEY",
                            BAD_CAST "KEY_RIGHTCTRL");
   for (int i = 0; i < PROPS_POINTER_MODE; i++)
   {
      xmlNewChild(pointer_node, NULL, BAD_CAST props_name_pointer_mode[i],
                                      BAD_CAST props_value_pointer_mode[i]);
   }

   // scrolling mode
   xmlNodePtr scrolling_node = xmlNewChild(root_node, NULL,
                                           BAD_CAST "scrolling_mode", NULL);
   xmlNewProp(scrolling_node, BAD_CAST "SCROLLING_MODE_KEY",
                              BAD_CAST "KEY_RIGHTSHIFT");
   for (int i = 0; i < PROPS_SCROLLING_MODE; i++)
   {
      xmlNewChild(scrolling_node, NULL, BAD_CAST props_name_scrolling_mode[i],
                                        BAD_CAST props_value_scrolling_mode[i]);
   }

   xmlSaveFormatFileEnc("-", doc, "ASCII", 1);

   xmlFreeDoc(doc);
   xmlCleanupParser();

   exit(EXIT_SUCCESS);
}

#else

int main(int argc, char *argv[])
{
   fprintf(stderr, "[E] tree or output supports not available\n");
   exit(EXIT_FAILURE);
}

#endif
