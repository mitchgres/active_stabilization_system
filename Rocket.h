#include "Canard.h"
#include "Odometry.h"

const int SERIAL_OUTPUT_FREQUENCY = 9600;

class Rocket
{
   public:
      void setup();
      void run_flight_program();
   private:
     const short CANARD_ONE_PIN = 3;
     const short CANARD_TWO_PIN = 6;
     const short CANARD_THREE_PIN = 11;
     const short CANARD_FOUR_PIN = 10;
     
     Canard _canard_one;
     Canard _canard_two;
     Canard _canard_three;
     Canard _canard_four;

     Odometry _odometry;
};

void Rocket::setup()
{
  Serial.begin(SERIAL_OUTPUT_FREQUENCY);
  _canard_one.setup(CANARD_ONE_PIN);
  _canard_two.setup(CANARD_TWO_PIN);
  _canard_three.setup(CANARD_THREE_PIN);
  _canard_four.setup(CANARD_FOUR_PIN);

  _odometry.setup();
}

void Rocket::run_flight_program()
{
  _odometry.update();

  _canard_two.set_angle(-(_odometry.get_y_orientation() - 90));
  _canard_four.set_angle(-_canard_two.get_angle());

  _canard_one.set_angle((_odometry.get_z_orientation() - 90));
  _canard_three.set_angle(-_canard_one.get_angle());
}
