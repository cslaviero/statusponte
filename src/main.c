#include <pebble.h>
#include <stat.h>


void handle_init(void) {
	show_status();
}

void handle_deinit(void) {
	hide_status();
}


int main(void) {
  handle_init();
  app_event_loop();
  handle_deinit();
}
