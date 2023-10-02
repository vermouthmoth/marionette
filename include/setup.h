//        s
//      (o5o)
//     /7(=)\_
// VermouthMoth

#define PASS_THROUGH_KEY_MAX 256
#define REMAP_KEY_MAX        256
#define NONE_MODE              0 // neither pointer mode nor scrolling mode
#define POINTER_MODE           1
#define SCROLLING_MODE         2

extern char *DEVICE;

extern unsigned int POINTER_MODE_KEY;
extern unsigned int POINTER_UP_KEY;
extern unsigned int POINTER_DOWN_KEY;
extern unsigned int POINTER_RIGHT_KEY;
extern unsigned int POINTER_LEFT_KEY;
extern int POINTER_MOVEMENT_SPEED;
extern unsigned int POINTER_SPEEDUP_KEY;
extern int POINTER_SPEEDUP_FACTOR;

extern unsigned int MOUSE_LEFT_BUTTON;
extern unsigned int MOUSE_RIGHT_BUTTON;
extern unsigned int MOUSE_MIDDLE_BUTTON;

extern unsigned int SCROLLING_MODE_KEY;
extern unsigned int SCROLLING_UP_KEY;
extern unsigned int SCROLLING_DOWN_KEY;
extern unsigned int SCROLLING_RIGHT_KEY;
extern unsigned int SCROLLING_LEFT_KEY;
extern int SCROLLING_SPEED;
extern unsigned int SCROLLING_SPEEDUP_KEY;
extern int SCROLLING_SPEEDUP_FACTOR;

typedef struct
{
   unsigned int keycode;
   //char const *keyname;
   int mode; // POINTER_MODE or SCROLLING_MODE
} PassThroughKey;

extern PassThroughKey pass_through_keys[PASS_THROUGH_KEY_MAX];
extern int pass_through_keys_count;

// one Remap for one key binding
typedef struct
{
   unsigned int in_keycode;
   char const *in_keycode_name;
   unsigned int out_keycode;
   char const *out_keycode_name;
   int mode; // NONE_MODE or POINTER_MODE or SCROLLING_MODE
} RemapKey;

extern RemapKey remap_keys[REMAP_KEY_MAX];
extern int remap_keys_count;

extern void setup(char const *file_name);
