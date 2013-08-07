SimpleMenuLayerTutorial
=======================

A tutorial on using SimpleMenuLayer in the Pebble SDK

Let's Go!
=========

First thing you want to do is [install the Pebble SDK](http://developer.getpebble.com/). I like to use [Vagrant](http://www.vagrantup.com/) to do my Pebble
development. You can find a nice setup for that [here](http://forums.getpebble.com/discussion/5951/install-the-sdk-in-minutes-on-any-operating-system-with-vagrant)!
I'm going to make the assumption that you've used the pathing and naming that the install guide reccomends. If not you'll have to alter
any file paths that don't match.

### Creating the project
`cd` into the directory you would like to create the project, and run `~/pebble-dev/PebbleSDK-1.12/Pebble/tools/create_pebble_project.py ~/pebble-dev/PebbleSDK-1.12/Pebble/sdk SimpleMenuLayerTutorial`
to create the folder `SimpleMenuLayerTutorial` which will contain the boilerplate for our app. `cd` into this directory,
and run `./waf configure` to get everything setup. Then run `./waf build` to ensure that everything is building ok. If you're
unable to successfuly build the app, go back through the SDK setup guide to ensure that everything is right.

### Code it up
You'll notice that a few directories and files have been created for us by running that python script above. You can read
a little about what's been generated [here](http://developer.getpebble.com/1/GettingStarted/HelloWorld).
The only file we'll need to work with for this tutorial is `src/SimpleMenuLayerTutorial.c`. Go ahead and open that
up in your text editor of choice.

Let's go ahead and alter the `PBL_APP_INFO` bit to give the app a new name.
```c
PBL_APP_INFO(MY_UUID,
             "My App", "My Company",
             1, 0, /* App version */
             DEFAULT_MENU_ICON,
             APP_INFO_STANDARD_APP);
```

The Pebble SDK doesn't allow dynamic memory allocation, so let's go ahead and get all of our variables in place that we'll need for our app.
Add the following below `Window window`.

```c
static SimpleMenuLayer s_simple_menu_layer;
static SimpleMenuItem s_fruit_menu_items[4];
static SimpleMenuItem s_animal_menu_items[4];
static SimpleMenuSection s_menu_sections[2];
static char *s_fruit_names[4] = {"Apple", "Banana", "Grape", "Pear"};
static char *s_animal_names[4] = {"Cat", "Dog", "Horse", "Fish"};
```

Our app is going to display a list of fruits and animals, seperated into two titled sections. `s_simple_menu_layer` will be
used to hold our SimpleMenuLayer, `s_fruit_menu_items` and `s_animal_menu_items` will hold each item in our menu. 
`s_menu_sections` will contain both the fruit and animal sections. `s_fruit_names` and `s_animal_names` will contain the names of
all of our items.

Below this block of code, add the following function prototypes

```c
void setup_window();
void window_load();
void setup_menu_items();
void setup_menu_sections();
```

in `setup_window` we'll configure the app's window. `window_load` will be a callback that is called when the window loads.
This is where we'll setup and add `s_simple_menu_layer`. `setup_menu_items` and `setup_menu_sections` will be used to setup
our item and section variables.

Let's change the `handle_init` function to look like this

```c
void handle_init(AppContextRef ctx) 
{
  setup_menu_items();
  setup_menu_sections();
  setup_window();
  window_stack_push(&window, true /* Animated */);
}
```

When the application init's we'll go ahead and setup all of our items and sections, setup our window, and finally push our window
to the stack. In the Pebble SDK, you push and pop windows onto a window stack that feels very similar to `UINavigationController` in the
iOS SDK.

Let's go ahead and code up `setup_window`

```c
void setup_window()
{
  window_init(&window, "Window Name");
  window_set_window_handlers(&window, (WindowHandlers){
    .load = window_load
    });
}
```

Here we init the window, and set the window's handlers. There are handlers for a few events on the window (load / unload / appear / etc)
but the only one we need to worry about is load.

Now we can implement `setup_menu_items` and `setup_menu_secctions`.

```c
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
```

This is pretty straight forward. Our SimpleMenuLayer is going to want an array of `SimpleMenuSection` to display. `SimpleMenuSection`
needs to have a title, items (`SimpleMenuItem`), and a count of the number of items. `SimpleMenuItem` in our case just has a title.
You can also add an icon, subtitle, and callback (function to call when the item is selected) to `SimpleMenuItem`, but we're not going to
worry about that in this demo.

The last thing we need to do is implement our `window_load` function.

```c
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
```

Here we grab the window's root layer (which we know is ready to go because the window has loaded). We use this during
`simple_menu_layer_init` to properly size or `SimpleMenuLayer`, We also pass in the window that will contain our `SimpleMenuLayer`, our
sections that we've setup (`s_menu_sections`), the number of sections, and NULL for the callback context (because we're not using that).

Now we should be ready to build and test our app! If you want to make sure everything is coded properly you can compare against
the completed version [here](https://github.com/andrewroycarter/SimpleMenuLayerTutorial/blob/master/src/SimpleMenuLayerTutorial.c)

In order to build the app, run `./waf build` from within the project directory. Assuming everything compiles ok (again check the above file for reference) we can
go ahead and put it on the watch. To do this, we need to http serve our project directory so we can use the iOS and Android Pebble app to install
our app. From the project directory run `python -m SimpleHTTPServer `. This is an easy way to serve a directory! Now visit the website
we're serving from your phone (if your IP address was 192.168.1.1 the address would be `http://192.168.1.1:8000` and navigate to 
`/build/SimpleMenuLayerTutorial.pbw`. Now you should be able to install the application on your watch!

If I've missed anything, or you're having any trouble, please file an issue or pull request. I'm not the best writer in the world, and this
is the first development writeup I've ever done, so any contributions or criticism is more than welcome!
