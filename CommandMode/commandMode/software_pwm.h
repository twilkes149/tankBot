#ifndef __SOFTWARE_PWM__
#define __SOFTWARE_PWM__

const int frequency = 490;                    // Hz
const double period = (1.0/frequency) * 100000; // Conversion to us
int pin;


// PWM signal is HIGH_Time + LOW_Time = period
// pin is the pin
class Software_PWM {
  private:
    int pin;
    int percent;
    long highTime;
    long lowTime;
    long changeTime;
    
    enum pwm_state_t {
      _HIGH = HIGH,
      _LOW = LOW,
      _STOPPED = 5
    } state;
    
  public:
    Software_PWM() {
      pin = 2;
      percent = 50;
    }

    Software_PWM(int _pin, int _percent) {
      pin = _pin;
      percent = _percent;
      Software_PWM_init();
    }

    Software_PWM_init() {
      pinMode(pin, OUTPUT);
      highTime = period * ((double) percent / 100.0);
      lowTime = period - highTime;
      changeTime = micros();
      state = _HIGH;      
    }

    Software_PWM_start() {
      state = _HIGH;
    }

    Software_PWM_stop() {
      state = _STOPPED;
      digitalWrite(pin, LOW);
    }

    Software_PWM_setPercent(int _percent) {
      percent = _percent;

      Software_PWM_init(); // re-calc the on/off time
    }

    // This function needs to be called once every loop iteration
    // ... it is what keeps track of time so we can toggle the pin
    Software_PWM_tick() {
      if (state == _STOPPED) {
        return;
      }
      
      switch (state) {
        case _HIGH:
          if (micros() >= changeTime + highTime) {
            digitalWrite(pin, LOW);
            state = _LOW;
            changeTime = micros();           
          }
        break;

        case _LOW:
        if (micros() >= changeTime + lowTime) {
            digitalWrite(pin, HIGH);
            state = _HIGH;
            changeTime = micros();            
          }
        break;
      }
    }
};


#endif
