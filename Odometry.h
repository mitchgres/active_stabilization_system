#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

#include "Subsystem.h"

class Odometry: public Subsystem
{
  public:
    const float BNO055_SAMPLE_RATE_MS = 10;
    
    // Creating the object isn't enought, we must also run a setup function since there are things that must be done during the setup loop. 
    void setup();
    
    // Should be run every cycle to update the values for the odometry.
    void update();

    inline float get_x_orientation() const;
    inline float get_y_orientation() const;
    inline float get_z_orientation() const;

    inline float get_speed() const;

    inline void set_x_offset(float &x_offset);
    inline void set_y_offset(float &y_offset);
    inline void set_z_offset(float &z_offset);

  private:
    // These are used for calculating the speed of the rocket. 
    const float ACCELERATION_VELOCITY_TRANSITION = BNO055_SAMPLE_RATE_MS/1000.0;
    const float ACCELERATION_POSITION_TRANSITION = 0.5 * ACCELERATION_VELOCITY_TRANSITION * ACCELERATION_VELOCITY_TRANSITION;
    // For I2C Communication.  
    const static int BNO055_DEFAULT_I2C_ADDRESS = 0x28;
    const static int I2C_ID = 55;
    
    Adafruit_BNO055 _sensor = Adafruit_BNO055(I2C_ID, BNO055_DEFAULT_I2C_ADDRESS, &Wire);

    float _x_orientation;
    float _y_orientation;
    float _z_orientation;
    
    float _speed;

    float _x_orientation_offsets = 0;
    float _y_orientation_offsets = 0;
    float _z_orientation_offsets = 0;
};

void Odometry::setup()
{
  boolean is_connected = _sensor.begin();
  if (!is_connected){ set_error_status(IN_ERROR); } // If the sensor is connected then we'll continue with using it. Else we'll set the subsystem in error.
}

void Odometry::update()
{
  sensors_event_t orientation_data;
  sensors_event_t linear_acceleration_data;
  sensors_event_t gravity_vector_data;
  _sensor.getEvent(&orientation_data, Adafruit_BNO055::VECTOR_EULER);
  _sensor.getEvent(&linear_acceleration_data, Adafruit_BNO055::VECTOR_LINEARACCEL);
  _sensor.getEvent(&gravity_vector_data, Adafruit_BNO055::VECTOR_GRAVITY);

  _y_orientation = (acos(gravity_vector_data.acceleration.y / (9.8)) * 57.2958) - _y_orientation_offsets;
  _z_orientation = (acos(gravity_vector_data.acceleration.z / (9.8)) * 57.2958) - _z_orientation_offsets;

  Serial.println(gravity_vector_data.acceleration.y / (9.8));

  _x_orientation = orientation_data.orientation.x - _x_orientation_offsets; 
  //_y_orientation = (orientation_data.orientation.y * (orientation_data.orientation.z / abs(orientation_data.orientation.z))) - _y_orientation_offsets; // 
  //_z_orientation = orientation_data.orientation.z - _z_orientation_offsets;

  // Shifting the y-orientation into a 0-180 degree range instead of +-90. 
  //if (_y_orientation < 0){ _y_orientation =90 + (90 - abs(_y_orientation)); }
   
  _speed = (ACCELERATION_VELOCITY_TRANSITION * linear_acceleration_data.acceleration.x)/(cos((PI/180) * orientation_data.orientation.x)); // We're going to use Euler Angles for this speed calculations because the math works. :)
}

inline float Odometry::get_x_orientation() const { return _x_orientation; }

inline float Odometry::get_y_orientation() const { return _y_orientation; }

inline float Odometry::get_z_orientation() const { return _z_orientation; }

inline float Odometry::get_speed() const { return _speed; }

inline void Odometry::set_x_offset(float &x_offset) { _x_orientation_offsets = x_offset; }
    
inline void Odometry::set_y_offset(float &y_offset) { _y_orientation_offsets = y_offset; }
    
inline void Odometry::set_z_offset(float &z_offset) { _z_orientation_offsets = z_offset; }
