#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <libevdev/libevdev.h>

#include <libxml/xmlreader.h>

#include "../include/setup.h"

//        s
//      (o5o)
//     /7(=)\_
// VermouthMoth

#ifdef LIBXML_READER_ENABLED

char *DEVICE;

// for keycode, corresponds to KEY_RESERVED at the initial value
unsigned int POINTER_MODE_KEY;
unsigned int POINTER_UP_KEY;
unsigned int POINTER_DOWN_KEY;
unsigned int POINTER_RIGHT_KEY;
unsigned int POINTER_LEFT_KEY;
int POINTER_MOVEMENT_SPEED = 1;
unsigned int POINTER_SPEEDUP_KEY;
int POINTER_SPEEDUP_FACTOR = 1;

unsigned int MOUSE_LEFT_BUTTON;
unsigned int MOUSE_RIGHT_BUTTON;
unsigned int MOUSE_MIDDLE_BUTTON;

unsigned int SCROLLING_MODE_KEY;
unsigned int SCROLLING_UP_KEY;
unsigned int SCROLLING_DOWN_KEY;
unsigned int SCROLLING_RIGHT_KEY;
unsigned int SCROLLING_LEFT_KEY;
int SCROLLING_SPEED = 1;
unsigned int SCROLLING_SPEEDUP_KEY;
int SCROLLING_SPEEDUP_FACTOR = 1;

PassThroughKey pass_through_keys[PASS_THROUGH_KEY_MAX];
int pass_through_keys_count;

static int keyname_to_keycode(char const *keyname)
{
   return libevdev_event_code_from_name(EV_KEY, keyname);
}

static void set_keycode(unsigned int *setting_item,
                        char const *setting_value)
{
   int ret;
   ret = keyname_to_keycode(setting_value);
   if (ret != -1)
      *setting_item = (unsigned int)ret;
   else
      printf("[E] %-25s  failed\n", "");
}

static void set_integer(int *setting_item, char const *setting_value)
{
   int ret;
   ret = atoi(setting_value);
   if (ret != 0)
      *setting_item = ret;
   else
      printf("[E] %-25s  failed\n", "");
}

static void set_value(char const *name, char const *value)
{
   printf("[I] %-25s: %s\n", name, value);

   if (strcmp("DEVICE", name) == 0)
      DEVICE = strdup(value);
   else if (strcmp("POINTER_MODE_KEY", name) == 0)
      set_keycode(&POINTER_MODE_KEY, value);
   else if (strcmp("POINTER_UP_KEY", name) == 0)
      set_keycode(&POINTER_UP_KEY, value);
   else if (strcmp("POINTER_DOWN_KEY", name) == 0)
      set_keycode(&POINTER_DOWN_KEY, value);
   else if (strcmp("POINTER_RIGHT_KEY", name) == 0)
      set_keycode(&POINTER_RIGHT_KEY, value);
   else if (strcmp("POINTER_LEFT_KEY", name) == 0)
      set_keycode(&POINTER_LEFT_KEY, value);
   else if (strcmp("POINTER_SPEEDUP_KEY", name) == 0)
      set_keycode(&POINTER_SPEEDUP_KEY, value);
   else if (strcmp("MOUSE_LEFT_BUTTON", name) == 0)
      set_keycode(&MOUSE_LEFT_BUTTON, value);
   else if (strcmp("MOUSE_RIGHT_BUTTON", name) == 0)
      set_keycode(&MOUSE_RIGHT_BUTTON, value);
   else if (strcmp("MOUSE_MIDDLE_BUTTON", name) == 0)
      set_keycode(&MOUSE_MIDDLE_BUTTON, value);
   else if (strcmp("SCROLLING_MODE_KEY", name) == 0)
      set_keycode(&SCROLLING_MODE_KEY, value);
   else if (strcmp("SCROLLING_UP_KEY", name) == 0)
      set_keycode(&SCROLLING_UP_KEY, value);
   else if (strcmp("SCROLLING_DOWN_KEY", name) == 0)
      set_keycode(&SCROLLING_DOWN_KEY, value);
   else if (strcmp("SCROLLING_RIGHT_KEY", name) == 0)
      set_keycode(&SCROLLING_RIGHT_KEY, value);
   else if (strcmp("SCROLLING_LEFT_KEY", name) == 0)
      set_keycode(&SCROLLING_LEFT_KEY, value);
   else if (strcmp("SCROLLING_SPEEDUP_KEY", name) == 0)
      set_keycode(&SCROLLING_SPEEDUP_KEY, value);
   else if (strcmp("POINTER_MOVEMENT_SPEED", name) == 0)
      set_integer(&POINTER_MOVEMENT_SPEED, value);
   else if (strcmp("SCROLLING_SPEED", name) == 0)
      set_integer(&SCROLLING_SPEED, value);
   else if (strcmp("POINTER_SPEEDUP_FACTOR", name) == 0)
      set_integer(&POINTER_SPEEDUP_FACTOR, value);
   else if (strcmp("SCROLLING_SPEEDUP_FACTOR", name) == 0)
      set_integer(&SCROLLING_SPEEDUP_FACTOR, value);
}

static void set_pass_through_key(char const *keyname,
                                 int mode)
{
   printf("[I] %-25s: %s\n", "PASS_THROUGH_KEY", keyname);

   int ret;
   ret = keyname_to_keycode(keyname);
   if (ret != -1)
   {
      pass_through_keys[pass_through_keys_count].keycode
         = (unsigned int)ret;
      pass_through_keys[pass_through_keys_count].mode
         = mode;
      pass_through_keys_count += 1;
   }
   else
      printf("[E] %-25s  failed\n", "");
}

static void parse_and_load(xmlTextReaderPtr reader)
{
   printf("[I] start loading settings...\n");

   // name -> value -> name -> value -> ... loop
   // should be guaranteed by .dtd
   xmlChar const *name;
   unsigned int mode; // for pass through setting

   do
   {
      if (xmlTextReaderNodeType(reader) == XML_READER_TYPE_ELEMENT)
      {
         name = xmlTextReaderConstName(reader);

         if (strcmp((char const *)name, "config") == 0)
         {
            xmlChar const *value
               = xmlTextReaderGetAttribute(reader,
                                           BAD_CAST "DEVICE");
            if (value != NULL)
               set_value("DEVICE", (char const *)value);
         }
         else if (strcmp((char const *)name, "pointer_mode") == 0)
         {
            xmlChar const *value
               = xmlTextReaderGetAttribute(reader,
                                           BAD_CAST "POINTER_MODE_KEY");
            mode = POINTER_MODE;
            if (value != NULL)
               set_value("POINTER_MODE_KEY", (char const *)value);
         }
         else if (strcmp((char const *)name, "scrolling_mode") == 0)
         {
            xmlChar const *value
               = xmlTextReaderGetAttribute(reader,
                                           BAD_CAST "SCROLLING_MODE_KEY");
            mode = SCROLLING_MODE;
            if (value != NULL)
               set_value("SCROLLING_MODE_KEY", (char const *)value);
         }
      }
      else if (xmlTextReaderNodeType(reader) == XML_READER_TYPE_TEXT)
      {
         xmlChar const *value = xmlTextReaderConstValue(reader);
         if (value != NULL)
         {
            if (strcmp((char const *)name, "PASS_THROUGH_KEY") == 0)
               set_pass_through_key((char const *)value,
                                    mode);
            else
               set_value((char const *)name, (char const *)value);
         }
      }
   } while (xmlTextReaderRead(reader) == 1);
}


static void validate_xml(xmlTextReaderPtr reader)
{
   if (xmlTextReaderIsValid(reader) != 1)
   {
      fprintf(stderr, "[E] failed to load config file format\n");
      exit(EXIT_FAILURE);
   }
}

void setup(char const *file_name)
{
   xmlTextReaderPtr reader = xmlReaderForFile(file_name, "ASCII",
                                              XML_PARSE_NOENT
                                            | XML_PARSE_DTDATTR
                                            | XML_PARSE_DTDVALID);
   if (reader == NULL)
   {
      fprintf(stderr, "[E] failed to open config file: %s\n", file_name);
      exit(EXIT_FAILURE);
   }

   validate_xml(reader);

   parse_and_load(reader);

   xmlFreeTextReader(reader);
   xmlCleanupParser();
}

#else

void setup(char const *file_name)
{
   fprintf(stderr, "[E] xmlReader support not available\n");
   exit(EXIT_FAILURE);
}

#endif
