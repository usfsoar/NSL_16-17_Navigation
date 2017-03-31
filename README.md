# SOAR_NSL_Navigation

## About
- Code to aim a camera from a lander ejected from alt. of 1 mile to a point on the ground
- Competing in the 2016-2017 NASA NSL ([NASA student Launch](http://www.usfsoar.com/projects/nsl-2016-2017/))
- Built by students in SOAR ([Society Of Aeronautics and Rockety](http://www.usfsoar.com/)) at USF ([University of South Florida](http://www.usf.edu/))
- University of South Florida

## Documentation
The code consists of several libraries along with the main `.ino` file, including `Lander` and `Timer`:

### `Lander` Library
While the primary program controls timing, checks, and initlization, the `Lander` library serves to control the functions of the lander itself, and as such includes the `landerDOF`, `landerServos`, and `landerGPS` libraries, as well as several functions:

#### Functions
* `void pointTo(float targetLoc[2])`

This function combines the data from the 10 degrees of freedom board and the GPS sensor to point the servos at a specific point defined by `targetLoc` where `targetLoc` is an array in the format `{float latitude, float longitude}`.

* `bool init()`

Checks if the lander sucessfully initiated and returns a boolean value. For `true` to be returned, the GPS, 10 degrees of freedom sensor, and servo controller must initialize succesfully.

#### `landerDOF` Library
The `landerDOF` library processes data from the [Adafruit 10-DOF IMU Breakout](https://www.adafruit.com/product/1604) sensor, including altitude data from pressure and temperature, and magnetometer data. 

**Documentation coming soon.**

#### `landerServos` Library
This library provides functions for servo control. There are two [Hitech 5625-MG](http://www.servodatabase.com/servo/hitec/hs-5625mg) servos attached to a [16-Channel 12-bit PWM Servo Controller](https://www.adafruit.com/product/815). The pan servo is referred to as `1` and tilt as `2` when setting angles and pins.

##### Functions
* `void setAngle(int servo, int angle)`

Takes a servo (`1` or `2` as defined above) and sets it to an angle in degrees, using the PWM constraints set in the library.

* `void setPin(int servo, int pin)`

Allows for easily changing the servo pin settings.

* `int constrainPWM(int val, int limitA, int limitB)`

Essentially the same as the standard [`constrain()`](https://www.arduino.cc/en/reference/constrain) function but does not require the constraints to be in order. Used for constraining arbitrarily ordered values, as we often want to `map` angles in reverse.

* `bool init()`

Initiates the PWM communication. Always returns true (does not check if servos are properly connected).

#### `landerGPS` Library
Provides functions for easily accessing GPS data from the GPS module.

**This is not currently enabled.**

##### Functions
* `float * getCurrentLatLon()`

Returns a two-item array in the format `{float latitude, float longitude}` with the most recently found GPS location of the lander.

* `bool init()`

Initiates communication with the GPS module.

### `Timer` Library
This library provides functions to manage the primary timer of the system, which allows for easy timing of behavior. Runs a timer in the background when started.

#### Functions and Variables
* `void pause(), reset(), start(), restart()`

Self-explanatory functions for managing timer behavior.

* `int64_t getElapsedTime()`

Returns the elapsed time since the timer was started.

* `bool isRunning`

Returns `true` if the timer is currently running. The timer will not be running if any initiation processes fail.