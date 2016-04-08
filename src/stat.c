#include <pebble.h>
#include "stat.h"

#define KEY_NIT 0
#define KEY_RJ 1
#define KEY_AGUARDE 2

static Window *s_window;
static GFont s_res_gothic_18;
static GFont s_res_gothic_18_bold;
static GFont s_res_gothic_14;
static GFont s_bit;

static GFont s_res_gothic_24_bold;
static GFont s_leco;
static Layer *s_layer_nit;
static Layer *s_layer_rj;
static TextLayer *s_aguarde;

static TextLayer *s_txt_nit;
static TextLayer *s_txt_rj;
static TextLayer *s_tempo_nit;
static TextLayer *s_tempo_rj;
static TextLayer *s_time_layer;
static Layer *s_canvas_layer;


static void canvas_update_proc(Layer *layer, GContext *ctx) {
  // Custom drawing happens here!
	graphics_draw_line(ctx,GPoint(PBL_IF_ROUND_ELSE(90,72),PBL_IF_ROUND_ELSE(32,24)),GPoint(PBL_IF_ROUND_ELSE(90,72),PBL_IF_ROUND_ELSE(120,130))); 
}

static void initialise_ui(void) {
  s_window = window_create();
  window_set_background_color(s_window, GColorClear);
  #ifndef PBL_SDK_3
    window_set_fullscreen(s_window, false);
  #endif

	// Load the custom font
	s_leco = fonts_get_system_font(FONT_KEY_LECO_42_NUMBERS);
	
  s_res_gothic_18_bold = fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD);
	s_res_gothic_18 = fonts_get_system_font(FONT_KEY_GOTHIC_18);
  s_res_gothic_24_bold = fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD);
	s_res_gothic_14 = fonts_get_system_font(FONT_KEY_GOTHIC_14);
	
	s_bit = fonts_get_system_font(FONT_KEY_BITHAM_30_BLACK);
	
	//s_time_layer
	s_time_layer = text_layer_create(GRect(PBL_IF_ROUND_ELSE(47,37),PBL_IF_ROUND_ELSE(3,2),88,25));
	text_layer_set_text_alignment(s_time_layer,GTextAlignmentCenter);
  text_layer_set_font(s_time_layer, s_res_gothic_18);
	layer_add_child(window_get_root_layer(s_window), (Layer *)s_time_layer);	
  
  // s_aguarde
  s_aguarde = text_layer_create(GRect(PBL_IF_ROUND_ELSE(29,11),PBL_IF_ROUND_ELSE(130,130),125,25));
  text_layer_set_text(s_aguarde, "Aguarde...");
  text_layer_set_text_alignment(s_aguarde, GTextAlignmentCenter);
  text_layer_set_font(s_aguarde, s_res_gothic_18);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_aguarde);
  
	GRect bounds = layer_get_bounds(window_get_root_layer(s_window));

	// Create canvas layer
	s_canvas_layer = layer_create(bounds);
	// Assign the custom drawing procedure
	layer_set_update_proc(s_canvas_layer, canvas_update_proc);

	// Add to Window
	layer_add_child(window_get_root_layer(s_window), s_canvas_layer);

  // s_tempo_nit
  s_tempo_nit = text_layer_create(GRect(PBL_IF_ROUND_ELSE(0,0), PBL_IF_ROUND_ELSE(70,60), 86, 48));
  text_layer_set_text(s_tempo_nit, "--");
  text_layer_set_text_alignment(s_tempo_nit, GTextAlignmentCenter);
  text_layer_set_font(s_tempo_nit, s_leco);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_tempo_nit);
  
  // s_tempo_rj
  s_tempo_rj = text_layer_create(GRect(PBL_IF_ROUND_ELSE(96,78), PBL_IF_ROUND_ELSE(70,60), 82, 48));
  text_layer_set_text(s_tempo_rj, "--");
  text_layer_set_text_alignment(s_tempo_rj, GTextAlignmentCenter);
  text_layer_set_font(s_tempo_rj, s_leco);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_tempo_rj);
	
	// s_txt_nit
  s_txt_nit = text_layer_create(GRect(PBL_IF_ROUND_ELSE(0,2), PBL_IF_ROUND_ELSE(42,26),86,32));
  text_layer_set_text(s_txt_nit, "NIT");
  text_layer_set_text_alignment(s_txt_nit, GTextAlignmentCenter);
  text_layer_set_font(s_txt_nit, s_bit);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_txt_nit);

		// s_txt_rj
  s_txt_rj = text_layer_create(GRect(PBL_IF_ROUND_ELSE(96,78),PBL_IF_ROUND_ELSE(42,26),82,32));
  text_layer_set_text(s_txt_rj, "RIO");
  text_layer_set_text_alignment(s_txt_rj, GTextAlignmentCenter);
  text_layer_set_font(s_txt_rj, s_bit);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_txt_rj);

}

static void destroy_ui(void) {
  window_destroy(s_window);
  //layer_destroy(s_layer_nit);
	text_layer_destroy(s_time_layer);
//layer_destroy(s_layer_rj);
  text_layer_destroy(s_aguarde);
//  bitmap_layer_destroy(s_img_nit);
 // bitmap_layer_destroy(s_image_rj);
  text_layer_destroy(s_tempo_nit);
  text_layer_destroy(s_tempo_rj);
 // gbitmap_destroy(s_res_img_nit);
 // gbitmap_destroy(s_res_img_rj);
}

//AppMessage STUFF

static void inbox_received_callback(DictionaryIterator *iterator, void *context) {
	// Store incoming information
	static char nit_buffer[32];
	static char rj_buffer[32];
	static char aguarde_buffer[32];	
	
	static char nit_layer_buffer[32];
	static char rj_layer_buffer[32];
	static char aguarde_layer_buffer[32];
	
  // Read tuples for data
	Tuple *timeNit = dict_find(iterator, KEY_NIT); //search for "key_nit" in iterator
	Tuple *timeRJ = dict_find(iterator, KEY_RJ);
	Tuple *aguarde = dict_find(iterator,KEY_AGUARDE);
	
	// If all data is available, use it
	if(timeNit && timeRJ && aguarde ) {
		snprintf(nit_buffer, sizeof(nit_buffer), "%s", timeNit->value->cstring);
		snprintf(rj_buffer, sizeof(rj_buffer), "%s", timeRJ->value->cstring);
		snprintf(aguarde_buffer, sizeof(aguarde_buffer), "%s", aguarde->value->cstring);
	}
	
	// Assemble full string and display
	// snprintf formata para o formato especificado
	int c = atoi(nit_buffer);
	if(c>29){
		text_layer_set_text_color(s_tempo_nit,GColorRed);
	}else{
		if (c<=29 && c > 15){
			text_layer_set_text_color(s_tempo_nit,GColorYellow);
		}else{
			if(c<=15 && c>=13){
				text_layer_set_text_color(s_tempo_nit,GColorGreen);
			}
		}
	}	

	snprintf(nit_layer_buffer, sizeof(nit_layer_buffer), "%s", nit_buffer);
	text_layer_set_text(s_tempo_nit, nit_layer_buffer);
	
	c = atoi(rj_buffer);
	if(c==13){
		text_layer_set_text_color(s_tempo_rj,GColorGreen);
	}else{
		if (c > 15){
			text_layer_set_text_color(s_tempo_rj,GColorYellow);
		}else{
			if(c>29){
				text_layer_set_text_color(s_tempo_rj,GColorRed);
			}
		}
	}

	snprintf(rj_layer_buffer, sizeof(rj_layer_buffer), "%s", rj_buffer);
	text_layer_set_text(s_tempo_rj, rj_layer_buffer);
	
	text_layer_set_font(s_aguarde, s_res_gothic_18);
	snprintf(aguarde_layer_buffer, sizeof(aguarde_layer_buffer), "Ult. boletim: %s", aguarde_buffer);
	text_layer_set_text(s_aguarde, aguarde_layer_buffer); // por enquanto só coloca o tempo
	
	
//	layer_set_hidden(s_aguarde,false);
	
	//vibe it. two times.
	vibes_double_pulse();
	//light?
	light_enable_interaction();
	
}

static void inbox_dropped_callback(AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Message dropped!");
}

static void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Outbox send failed!");
}

static void outbox_sent_callback(DictionaryIterator *iterator, void *context) {
  APP_LOG(APP_LOG_LEVEL_INFO, "Outbox send success!");
}


static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
 // text_layer_set_text(s_output_layer, "Select pressed!");
 // send a message to update travel time
	
	// set s_aguarde to "Aguarde..."
	text_layer_set_font(s_aguarde, s_res_gothic_18_bold);
	text_layer_set_text(s_aguarde, "Aguarde...");
	
	// Prepare dictionary
	DictionaryIterator *iterator;
	app_message_outbox_begin(&iterator);
	
	// Write data
	int key = 0;
	int value = 1;
	dict_write_int(iterator, key, &value, sizeof(int), true /* signed */);

	// Send the data!
	app_message_outbox_send();
	
}

static void click_config_provider(void *context) {
  // Register the ClickHandlers
 // window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
  //window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
}


static void update_time() {
  // Get a tm structure
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);

  // Write the current hours and minutes into a buffer
  static char s_buffer[8];
  strftime(s_buffer, sizeof(s_buffer), clock_is_24h_style() ?
                                          "%H:%M" : "%I:%M", tick_time);

  // Display this time on the TextLayer
  text_layer_set_text(s_time_layer, s_buffer);
}


static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
	update_time();
}


static void handle_window_unload(Window* window) {
  destroy_ui();
}

void show_status(void) {
	
  initialise_ui();
  window_set_window_handlers(s_window, (WindowHandlers) {
    .unload = handle_window_unload,
  });

	window_set_click_config_provider(s_window, click_config_provider);
	
	// Register callbacks
		app_message_register_inbox_received(inbox_received_callback);
		app_message_register_inbox_dropped(inbox_dropped_callback);
		app_message_register_outbox_failed(outbox_failed_callback);
		app_message_register_outbox_sent(outbox_sent_callback);

		// Register with TickTimerService
		tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
	
	// Open AppMessage
	app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());

  window_stack_push(s_window, true);
}

void hide_status(void) {
  window_stack_remove(s_window, true);
}
