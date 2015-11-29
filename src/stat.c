#include <pebble.h>
#include "stat.h"

#define KEY_NIT 0
#define KEY_RJ 1
#define KEY_AGUARDE 2

static Window *s_window;
static GFont s_res_gothic_18;
static GFont s_res_gothic_18_bold;
static GFont s_res_gothic_14;
static GBitmap *s_res_img_nit;
static GBitmap *s_res_img_rj;
static GFont s_res_gothic_24_bold;
static Layer *s_layer_nit;
static Layer *s_layer_rj;
static TextLayer *s_aguarde;
static BitmapLayer *s_img_nit;
static BitmapLayer *s_image_rj;
static TextLayer *s_txt_nit;
static TextLayer *s_txt_rj;
static TextLayer *s_tempo_nit;
static TextLayer *s_tempo_rj;

static void initialise_ui(void) {
  s_window = window_create();
  window_set_background_color(s_window, GColorClear);
  #ifndef PBL_SDK_3
    window_set_fullscreen(s_window, false);
  #endif
  
  s_res_gothic_18_bold = fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD);
	s_res_gothic_18 = fonts_get_system_font(FONT_KEY_GOTHIC_18);
  s_res_img_nit = gbitmap_create_with_resource(RESOURCE_ID_IMG_NIT);
  s_res_img_rj = gbitmap_create_with_resource(RESOURCE_ID_IMG_RJ);
  s_res_gothic_24_bold = fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD);
	s_res_gothic_14 = fonts_get_system_font(FONT_KEY_GOTHIC_14);

	// s_layer_nit
  s_layer_nit = layer_create(GRect(PBL_IF_ROUND_ELSE(10,8),PBL_IF_ROUND_ELSE(24,14),128,46));
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_layer_nit);
  
  // s_layer_rj
	s_layer_rj = layer_create(GRect(PBL_IF_ROUND_ELSE(10,8),PBL_IF_ROUND_ELSE(123,113),128,46));
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_layer_rj);
  
  // s_aguarde
  s_aguarde = text_layer_create(GRect(PBL_IF_ROUND_ELSE(29,11),PBL_IF_ROUND_ELSE(79,68),125,20));
  text_layer_set_text(s_aguarde, "Aguarde...");
  text_layer_set_text_alignment(s_aguarde, GTextAlignmentCenter);
  text_layer_set_font(s_aguarde, s_res_gothic_18);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_aguarde);
  
  // s_img_nit
  s_img_nit = bitmap_layer_create(GRect(PBL_IF_ROUND_ELSE(34,10), PBL_IF_ROUND_ELSE(31,21),56,22));
  bitmap_layer_set_bitmap(s_img_nit, s_res_img_nit);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_img_nit);
  
  // s_image_rj
  s_image_rj = bitmap_layer_create(GRect(PBL_IF_ROUND_ELSE(94,83),PBL_IF_ROUND_ELSE(111,101),56,28));
  bitmap_layer_set_bitmap(s_image_rj, s_res_img_rj);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_image_rj);
  
  // s_tempo_nit
  s_tempo_nit = text_layer_create(GRect(PBL_IF_ROUND_ELSE(93,73), PBL_IF_ROUND_ELSE(30,20), 66, 24));
  text_layer_set_text(s_tempo_nit, "?? Min.");
  text_layer_set_text_alignment(s_tempo_nit, GTextAlignmentCenter);
  text_layer_set_font(s_tempo_nit, s_res_gothic_24_bold);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_tempo_nit);
  
  // s_tempo_rj
  s_tempo_rj = text_layer_create(GRect(PBL_IF_ROUND_ELSE(27,12), PBL_IF_ROUND_ELSE(111,100), 59, 24));
  text_layer_set_text(s_tempo_rj, "?? Min.");
  text_layer_set_text_alignment(s_tempo_rj, GTextAlignmentCenter);
  text_layer_set_font(s_tempo_rj, s_res_gothic_24_bold);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_tempo_rj);
	
	// s_txt_nit
  s_txt_nit = text_layer_create(GRect(PBL_IF_ROUND_ELSE(34,10), PBL_IF_ROUND_ELSE(56,46),56,22));
  text_layer_set_text(s_txt_nit, "Niterói");
  text_layer_set_text_alignment(s_txt_nit, GTextAlignmentCenter);
  text_layer_set_font(s_txt_nit, s_res_gothic_14);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_txt_nit);

		// s_txt_rj
  s_txt_rj = text_layer_create(GRect(PBL_IF_ROUND_ELSE(93,83),PBL_IF_ROUND_ELSE(138,131),56,28));
  text_layer_set_text(s_txt_rj, "R. Janeiro");
  text_layer_set_text_alignment(s_txt_rj, GTextAlignmentCenter);
  text_layer_set_font(s_txt_rj, s_res_gothic_14);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_txt_rj);

}

static void destroy_ui(void) {
  window_destroy(s_window);
  layer_destroy(s_layer_nit);
  layer_destroy(s_layer_rj);
  text_layer_destroy(s_aguarde);
  bitmap_layer_destroy(s_img_nit);
  bitmap_layer_destroy(s_image_rj);
  text_layer_destroy(s_tempo_nit);
  text_layer_destroy(s_tempo_rj);
  gbitmap_destroy(s_res_img_nit);
  gbitmap_destroy(s_res_img_rj);
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

		snprintf(nit_buffer, sizeof(nit_buffer), "%s Min.", timeNit->value->cstring);
		snprintf(rj_buffer, sizeof(rj_buffer), "%s Min.", timeRJ->value->cstring);
		snprintf(aguarde_buffer, sizeof(aguarde_buffer), "%s", aguarde->value->cstring);
	}

	// Assemble full string and display
	// snprintf formata para o formato especificado
	snprintf(nit_layer_buffer, sizeof(nit_layer_buffer), "%s", nit_buffer);
	text_layer_set_text(s_tempo_nit, nit_layer_buffer);
	
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

	// Open AppMessage
	app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());

  window_stack_push(s_window, true);
}

void hide_status(void) {
  window_stack_remove(s_window, true);
}
