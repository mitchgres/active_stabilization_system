#include <Servo.h>

class Canard
{
  public:
    void setup(int);
    inline void set_angle(double);
    inline double get_angle() const;
    inline void set_offset(double);
   private:
    Servo _servo;
    double _angle;
    double _angle_offset;

};

void Canard::setup(int pin_number)
{
  _servo.attach(pin_number);
}

inline void Canard::set_angle(double angle)
{
  // The input is in a domain from +-90 degrees. It should be noted that offsets might limit this domain. 

  angle = angle + _angle_offset;

  if (angle >= 90){_angle = 90;}
  else if (angle <= -90){_angle = -90;}
  else { _angle = angle; }

  _servo.write(_angle + 90);
}

inline double Canard::get_angle() const { return _angle; }

// Note that this will limit the angles that the fin can move. A signifigant offset shouldn't be used. 
inline void Canard::set_offset(double angle_offset){ _angle_offset = angle_offset; }
