#include "Rocket.h"

Rocket rocket;

void setup() {
  rocket.setup();
}

void loop() {
  rocket.run_flight_program();
}
