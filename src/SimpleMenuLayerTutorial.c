#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"


#define MY_UUID { 0xED, 0x8A, 0xEC, 0xFA, 0x23, 0xE0, 0x4F, 0x26, 0xB4, 0xD9, 0xE2, 0x0C, 0x0C, 0xB8, 0x6B, 0xD6 }
PBL_APP_INFO(MY_UUID,
             "My App", "My Company",
             1, 0, /* App version */
             DEFAULT_MENU_ICON,
             APP_INFO_STANDARD_APP);

static Window window;
static SimpleMenuLayer s_simple_menu_layer;
static SimpleMenuItem s_fruit_menu_items[4];
static SimpleMenuItem s_animal_menu_items[4];
static SimpleMenuSection s_menu_sections[2];
static char *s_fruit_names[4] = {"Apple", "Banana", "Grape", "Pear"};
static char *s_animal_names[4] = {"Cat", "Dog", "Horse", "Fish"};

void setup_window();
void window_load();
void setup_menu_items();
void setup_menu_sections();

void setup_menu_items()
{
  for (int i = 0; i < (int)(sizeof(s_fruit_names) / sizeof(s_fruit_names[0])); i++)
  {
    s_fruit_menu_items[i] = (SimpleMenuItem){
      .title = s_fruit_names[i],
    };
  }

  for (int i = 0; i < (int)(sizeof(s_animal_names) / sizeof(s_animal_names[0])); i++)
  {
    s_animal_menu_items[i] = (SimpleMenuItem)
    {
      .title = s_animal_names[i],
    };
  }
}

void setup_menu_sections()
{
  s_menu_sections[0] = (SimpleMenuSection)
  {
    .title = "Fruits",
    .items = s_fruit_menu_items,
    .num_items = sizeof(s_fruit_menu_items) / sizeof(s_fruit_menu_items[0])
  };

  s_menu_sections[1] = (SimpleMenuSection)
  {
    .title = "Animals",
    .items = s_animal_menu_items,
    .num_items = sizeof(s_animal_menu_items) / sizeof(s_animal_menu_items[0])
  };
}

void setup_window()
{
  window_init(&window, "Window Name");
  window_set_window_handlers(&window, (WindowHandlers){
    .load = window_load
    });
}

void window_load(Window *window)
{
  Layer *root_layer = window_get_root_layer(window);
  simple_menu_layer_init(&s_simple_menu_layer,
                         layer_get_bounds(root_layer),
                         window,
                         s_menu_sections,
                         sizeof(s_menu_sections) / sizeof(s_menu_sections[0]),
                         NULL
  );
  layer_add_child(root_layer, (Layer *)&s_simple_menu_layer);
}

void handle_init(AppContextRef ctx) 
{
  setup_menu_items();
  setup_menu_sections();
  setup_window();
  window_stack_push(&window, true /* Animated */);
}

void pbl_main(void *params)
{
  PebbleAppHandlers handlers = {
    .init_handler = &handle_init
  };
  app_event_loop(params, &handlers);
}
