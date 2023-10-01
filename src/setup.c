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

// determined from .dtd
#define SETTING_ROOT_DEPTH  0
#define SETTING_MODE_DEPTH  1
#define SETTING_NAME_DEPTH  2
#define SETTING_VALUE_DEPTH 3
#define SETTING_ATTRIBUTES  3

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

unsigned int PASS_THROUGH_KEY[PASS_THROUGH_KEY_MAX];
int pass_through_key_count;

static void set_value(char const *name, char const *value)
{
   int ret;
   if (strcmp("DEVICE", name) == 0)
   {
      DEVICE = strdup(value);
   }
   else if (strcmp("POINTER_MODE_KEY", name) == 0)
   {
      ret = libevdev_event_code_from_name(EV_KEY, value);
      if (ret != -1)
         POINTER_MODE_KEY = (unsigned int)ret;
      else
         printf("[E] %-25s  failed\n", "");
   }
   else if (strcmp("POINTER_UP_KEY", name) == 0)
   {
      ret = libevdev_event_code_from_name(EV_KEY, value);
      if (ret != -1)
         POINTER_UP_KEY = (unsigned int)ret;
      else
         printf("[E] %-25s  failed\n", "");
   }
   else if (strcmp("POINTER_DOWN_KEY", name) == 0)
   {
      ret = libevdev_event_code_from_name(EV_KEY, value);
      if (ret != -1)
         POINTER_DOWN_KEY = (unsigned int)ret;
      else
         printf("[E] %-25s  failed\n", "");
   }
   else if (strcmp("POINTER_RIGHT_KEY", name) == 0)
   {
      ret = libevdev_event_code_from_name(EV_KEY, value);
      if (ret != -1)
         POINTER_RIGHT_KEY = (unsigned int)ret;
      else
         printf("[E] %-25s  failed\n", "");
   }
   else if (strcmp("POINTER_LEFT_KEY", name) == 0)
   {
      ret = libevdev_event_code_from_name(EV_KEY, value);
      if (ret != -1)
         POINTER_LEFT_KEY = (unsigned int)ret;
      else
         printf("[E] %-25s  failed\n", "");
   }
   else if (strcmp("POINTER_MOVEMENT_SPEED", name) == 0)
   {
      ret = atoi(value);
      if (ret != 0)
         POINTER_MOVEMENT_SPEED = ret;
      else
         printf("[E] %-25s  failed\n", "");
   }
   else if (strcmp("POINTER_SPEEDUP_KEY", name) == 0)
   {
      ret = libevdev_event_code_from_name(EV_KEY, value);
      if (ret != -1)
         POINTER_SPEEDUP_KEY = (unsigned int)ret;
      else
         printf("[E] %-25s  failed\n", "");
   }
   else if (strcmp("POINTER_SPEEDUP_FACTOR", name) == 0)
   {
      ret = atoi(value);
      if (ret != 0)
         POINTER_SPEEDUP_FACTOR = ret;
      else
         printf("[E] %-25s  failed\n", "");
   }
   else if (strcmp("MOUSE_LEFT_BUTTON", name) == 0)
   {
      ret = libevdev_event_code_from_name(EV_KEY, value);
      if (ret != -1)
         MOUSE_LEFT_BUTTON = (unsigned int)ret;
      else
         printf("[E] %-25s  failed\n", "");
   }
   else if (strcmp("MOUSE_RIGHT_BUTTON", name) == 0)
   {
      ret = libevdev_event_code_from_name(EV_KEY, value);
      if (ret != -1)
         MOUSE_RIGHT_BUTTON = (unsigned int)ret;
      else
         printf("[E] %-25s  failed\n", "");
   }
   else if (strcmp("MOUSE_MIDDLE_BUTTON", name) == 0)
   {
      ret = libevdev_event_code_from_name(EV_KEY, value);
      if (ret != -1)
         MOUSE_MIDDLE_BUTTON = (unsigned int)ret;
      else
         printf("[E] %-25s  failed\n", "");
   }
   else if (strcmp("SCROLLING_MODE_KEY", name) == 0)
   {
      ret = libevdev_event_code_from_name(EV_KEY, value);
      if (ret != -1)
         SCROLLING_MODE_KEY = (unsigned int)ret;
      else
         printf("[E] %-25s  failed\n", "");
   }
   else if (strcmp("SCROLLING_UP_KEY", name) == 0)
   {
      ret = libevdev_event_code_from_name(EV_KEY, value);
      if (ret != -1)
         SCROLLING_UP_KEY = (unsigned int)ret;
      else
         printf("[E] %-25s  failed\n", "");
   }
   else if (strcmp("SCROLLING_DOWN_KEY", name) == 0)
   {
      ret = libevdev_event_code_from_name(EV_KEY, value);
      if (ret != -1)
         SCROLLING_DOWN_KEY = (unsigned int)ret;
      else
         printf("[E] %-25s  failed\n", "");
   }
   else if (strcmp("SCROLLING_RIGHT_KEY", name) == 0)
   {
      ret = libevdev_event_code_from_name(EV_KEY, value);
      if (ret != -1)
         SCROLLING_RIGHT_KEY = (unsigned int)ret;
      else
         printf("[E] %-25s  failed\n", "");
   }
   else if (strcmp("SCROLLING_LEFT_KEY", name) == 0)
   {
      ret = libevdev_event_code_from_name(EV_KEY, value);
      if (ret != -1)
         SCROLLING_LEFT_KEY = (unsigned int)ret;
      else
         printf("[E] %-25s  failed\n", "");
   }
   else if (strcmp("SCROLLING_SPEED", name) == 0)
   {
      ret = atoi(value);
      if (ret != 0)
         SCROLLING_SPEED = ret;
      else
         printf("[E] %-25s  failed\n", "");
   }
   else if (strcmp("SCROLLING_SPEEDUP_KEY", name) == 0)
   {
      ret = libevdev_event_code_from_name(EV_KEY, value);
      if (ret != -1)
         SCROLLING_SPEEDUP_KEY = (unsigned int)ret;
      else
         printf("[E] %-25s  failed\n", "");
   }
   else if (strcmp("SCROLLING_SPEEDUP_FACTOR", name) == 0)
   {
      ret = atoi(value);
      if (ret != 0)
         SCROLLING_SPEEDUP_FACTOR = ret;
      else
         printf("[E] %-25s  failed\n", "");
   }
   else if (strcmp("PASS_THROUGH_KEY", name) == 0)
   {
      ret = libevdev_event_code_from_name(EV_KEY, value);
      if (ret != -1)
      {
         PASS_THROUGH_KEY[pass_through_key_count] = (unsigned int)ret;
         pass_through_key_count += 1;
      }
      else
         printf("[E] %-25s  failed\n", "");
   }
}

static void parse_and_load(xmlTextReaderPtr reader)
{
   printf("[I] start loading settings...\n");

   // name -> value -> name -> value -> ... loop
   // should be guaranteed by .dtd
   xmlChar const *attrs[SETTING_ATTRIBUTES]
                     = {BAD_CAST "DEVICE",
                        BAD_CAST "POINTER_MODE_KEY",
                        BAD_CAST "SCROLLING_MODE_KEY"};
   xmlChar const *name;
   do
   {
      xmlChar const *value = xmlTextReaderConstValue(reader);

      if (xmlTextReaderNodeType(reader) == XML_READER_TYPE_ELEMENT)
      {
         if ((xmlTextReaderDepth(reader) == SETTING_ROOT_DEPTH)
          || (xmlTextReaderDepth(reader) == SETTING_MODE_DEPTH))
         {
            // attributes
            for (int i  = 0; i < SETTING_ATTRIBUTES; i++)
            {
               xmlChar const *attr_value
                           = xmlTextReaderGetAttribute(reader, attrs[i]);
               if (attr_value != NULL)
               {
                  printf("[I] %-25s: %s\n", attrs[i], attr_value);
                  set_value((char const *)attrs[i],
                            (char const *)attr_value);
               }
            }
         }

         if (xmlTextReaderDepth(reader) == SETTING_NAME_DEPTH)
            name = xmlTextReaderConstName(reader);
      }

      if ((xmlTextReaderNodeType(reader) == XML_READER_TYPE_TEXT)
       && (xmlTextReaderDepth(reader) == SETTING_VALUE_DEPTH))
      {
         printf("[I] %-25s: %s\n", name, value);
         set_value((char const *)name, (char const *)value);
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
