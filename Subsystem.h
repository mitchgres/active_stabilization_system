class Subsystem
{
  public:
    const short IN_ERROR = 1;
    const short NOT_IN_ERROR = 0;
  
    inline boolean is_in_error() const; 
  protected:
     boolean _is_in_error;

     inline void set_error_status(short error_status);
};

inline boolean Subsystem::is_in_error() const { return _is_in_error; }

inline void Subsystem::set_error_status(short error_status) { _is_in_error = (error_status == IN_ERROR); }
