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
// $ ./gen_config > config.xml

// validate your edited config file
// $ xmllint --noout --dtdvalid config.dtd config.xml

#if defined(LIBXML_TREE_ENABLED) && defined(LIBXML_OUTPUT_ENABLED)

#define PROPS_POINTER_MODE   11
#define PROPS_SCROLLING_MODE  8

static char *props_name_pointer_mode[PROPS_POINTER_MODE]
                  = {"POINTER_MODE_KEY",
                     "POINTER_UP_KEY",
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
                  = {"KEY_RIGHTCTRL",
                     "KEY_W",
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
                  = {"SCROLLING_MODE_KEY",
                     "SCROLLING_UP_KEY",
                     "SCROLLING_DOWN_KEY",
                     "SCROLLING_RIGHT_KEY",
                     "SCROLLING_LEFT_KEY",
                     "SCROLLING_SPEED",
                     "SCROLLING_SPEEDUP_KEY",
                     "SCROLLING_SPEEDUP_FACTOR"};

static char *props_value_scrolling_mode[PROPS_SCROLLING_MODE]
                  = {"KEY_RIGHTSHIFT",
                     "KEY_W",
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

   // specify .dtd file
   xmlCreateIntSubset(doc, BAD_CAST "config", NULL,
                           BAD_CAST "config/config.dtd");

   // device
   xmlNodePtr device_node = xmlNewChild(root_node, NULL,
                                        BAD_CAST "device", NULL);
   xmlNewChild(device_node, NULL, BAD_CAST "DEVICE",
                                  BAD_CAST "/path/to/your/keyboard");

   // pointer mode
   xmlNodePtr pointer_node = xmlNewChild(root_node, NULL,
                                         BAD_CAST "pointer_mode", NULL);
   for (int i = 0; i < PROPS_POINTER_MODE; i++)
   {
      xmlNewChild(pointer_node, NULL, BAD_CAST props_name_pointer_mode[i],
                                      BAD_CAST props_value_pointer_mode[i]);
   }

   // scrolling mode
   xmlNodePtr scrolling_node = xmlNewChild(root_node, NULL,
                                           BAD_CAST "scrolling_mode", NULL);
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
