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
#define SETTING_ITEMS 16
#define SETTING_NAME_DEPTH 2
#define SETTING_VALUE_DEPTH 3

char *DEVICE;

// for keycode, corresponds to KEY_RESERVED at the initial value
unsigned int POINTER_MODE_KEY;
unsigned int UP_KEY;
unsigned int DOWN_KEY;
unsigned int RIGHT_KEY;
unsigned int LEFT_KEY;
int POINTER_MOVEMENT_SPEED = 1;
unsigned int POINTER_SPEEDUP_KEY;
int POINTER_SPEEDUP_FACTOR = 1;

unsigned int MOUSE_LEFT_BUTTON;
unsigned int MOUSE_RIGHT_BUTTON;
unsigned int MOUSE_MIDDLE_BUTTON;

unsigned int SCROLLING_MODE_KEY;
int SCROLLING_SPEED = 1;
unsigned int SCROLLING_SPEEDUP_KEY;
int SCROLLING_SPEEDUP_FACTOR = 1;

static void set_value(xmlChar const *name, xmlChar const *value)
{
   int ret;
   if (strcmp("DEVICE", (char const *)name) == 0)
   {
      DEVICE = strdup((char const *)value);
   }
   else if (strcmp("POINTER_MODE_KEY", (char const *)name) == 0)
   {
      ret = libevdev_event_code_from_name(EV_KEY, (char const *)value);
      if (ret != -1)
         POINTER_MODE_KEY = (unsigned int)ret;
      else
         printf("[E] %-25s  failed\n", "");

   }
   else if (strcmp("UP_KEY", (char const *)name) == 0)
   {
      ret = libevdev_event_code_from_name(EV_KEY, (char const *)value);
      if (ret != -1)
         UP_KEY = (unsigned int)ret;
      else
         printf("[E] %-25s  failed\n", "");
   }
   else if (strcmp("DOWN_KEY", (char const *)name) == 0)
   {
      ret = libevdev_event_code_from_name(EV_KEY, (char const *)value);
      if (ret != -1)
         DOWN_KEY = (unsigned int)ret;
      else
         printf("[E] %-25s  failed\n", "");
   }
   else if (strcmp("RIGHT_KEY", (char const *)name) == 0)
   {
      ret = libevdev_event_code_from_name(EV_KEY, (char const *)value);
      if (ret != -1)
         RIGHT_KEY = (unsigned int)ret;
      else
         printf("[E] %-25s  failed\n", "");
   }
   else if (strcmp("LEFT_KEY", (char const *)name) == 0)
   {
      ret = libevdev_event_code_from_name(EV_KEY, (char const *)value);
      if (ret != -1)
         LEFT_KEY = (unsigned int)ret;
      else
         printf("[E] %-25s  failed\n", "");
   }
   else if (strcmp("POINTER_MOVEMENT_SPEED", (char const *)name) == 0)
   {
      ret = atoi((char const *)value);
      if (ret != 0)
         POINTER_MOVEMENT_SPEED = ret;
      else
         printf("[E] %-25s  failed\n", "");
   }
   else if (strcmp("POINTER_SPEEDUP_KEY", (char const *)name) == 0)
   {
      ret = libevdev_event_code_from_name(EV_KEY, (char const *)value);
      if (ret != -1)
         POINTER_SPEEDUP_KEY = (unsigned int)ret;
      else
         printf("[E] %-25s  failed\n", "");
   }
   else if (strcmp("POINTER_SPEEDUP_FACTOR", (char const *)name) == 0)
   {
      ret = atoi((char const *)value);
      if (ret != 0)
         POINTER_SPEEDUP_FACTOR = ret;
      else
         printf("[E] %-25s  failed\n", "");
   }
   else if (strcmp("MOUSE_LEFT_BUTTON", (char const *)name) == 0)
   {
      ret = libevdev_event_code_from_name(EV_KEY, (char const *)value);
      if (ret != -1)
         MOUSE_LEFT_BUTTON = (unsigned int)ret;
      else
         printf("[E] %-25s  failed\n", "");
   }
   else if (strcmp("MOUSE_RIGHT_BUTTON", (char const *)name) == 0)
   {
      ret = libevdev_event_code_from_name(EV_KEY, (char const *)value);
      if (ret != -1)
         MOUSE_RIGHT_BUTTON = (unsigned int)ret;
      else
         printf("[E] %-25s  failed\n", "");
   }
   else if (strcmp("MOUSE_MIDDLE_BUTTON", (char const *)name) == 0)
   {
      ret = libevdev_event_code_from_name(EV_KEY, (char const *)value);
      if (ret != -1)
         MOUSE_MIDDLE_BUTTON = (unsigned int)ret;
      else
         printf("[E] %-25s  failed\n", "");
   }
   else if (strcmp("SCROLLING_MODE_KEY", (char const *)name) == 0)
   {
      ret = libevdev_event_code_from_name(EV_KEY, (char const *)value);
      if (ret != -1)
         SCROLLING_MODE_KEY = (unsigned int)ret;
      else
         printf("[E] %-25s  failed\n", "");
   }
   else if (strcmp("SCROLLING_SPEED", (char const *)name) == 0)
   {
      ret = atoi((char const *)value);
      if (ret != 0)
         SCROLLING_SPEED = ret;
      else
         printf("[E] %-25s  failed\n", "");
   }
   else if (strcmp("SCROLLING_SPEEDUP_KEY", (char const *)name) == 0)
   {
      ret = libevdev_event_code_from_name(EV_KEY, (char const *)value);
      if (ret != -1)
         SCROLLING_SPEEDUP_KEY = (unsigned int)ret;
      else
         printf("[E] %-25s  failed\n", "");
   }
   else if (strcmp("SCROLLING_SPEEDUP_FACTOR", (char const *)name) == 0)
   {
      ret = atoi((char const *)value);
      if (ret != 0)
         SCROLLING_SPEEDUP_FACTOR = ret;
      else
         printf("[E] %-25s  failed\n", "");
   }
}

static void parse_and_load(xmlTextReaderPtr reader)
{
   printf("[I] start loading settings...\n");

   // name -> value -> name -> value -> ... loop
   // should be guaranteed by .dtd
   xmlChar const *name;
   do
   {
      xmlChar const *value = xmlTextReaderConstValue(reader);

      if ((xmlTextReaderNodeType(reader) == XML_READER_TYPE_ELEMENT)
        && xmlTextReaderDepth(reader) == SETTING_NAME_DEPTH)
         name = xmlTextReaderConstName(reader);

      if ((xmlTextReaderNodeType(reader) == XML_READER_TYPE_TEXT)
        && xmlTextReaderDepth(reader) == SETTING_VALUE_DEPTH)
      {
         printf("[I] %-25s: %s\n", name, value);
         set_value(name, value);
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