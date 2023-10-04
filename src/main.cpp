#include <Arduino.h>

class Timer {
 public:
  using TickType = unsigned long;
  static const TickType TicksPerSecond = 1000000;
  Timer() : mStartMicros(micros()) {}

  TickType elapsedTime() const { return micros() - mStartMicros; }

 private:
  TickType mStartMicros;
};

// Class to wrap up a call back function
// that will be called periodically.

// Define a CallBack as a function pointer
// that takes no arguments and has no return
// value.
using CallBack = void (*)();

class PeriodicTimer {
 public:
  using TickType = unsigned long;
  static const TickType TicksPerSecond = 1000000;

  PeriodicTimer(CallBack callback, TickType callDeltaMicros)
      : mCallback(callback),
        mStartMicros(micros()),
        mCallDeltaMicros(callDeltaMicros) {}

  // Call tick() as frequently as possible.
  // There is overhead to this call probably on the order of
  // 4 to 10 micro seconds. Longer if it calls the callback.
  // When the callDeltaMicros time has elapsed the callback
  // function will be called and the timing will be reset.
  void tick() {
    auto currentTime = micros();
    if (currentTime - mStartMicros > mCallDeltaMicros) {
      mStartMicros = currentTime;
      (*mCallback)();
    }
  }

 private:
  CallBack mCallback;
  TickType mStartMicros;
  TickType mCallDeltaMicros;
};

void setup() { Serial.begin(9600); }

void countA() {
  static int count = 0;
  Serial.print("A = ");
  Serial.println(count);
  count++;
}

void countB() {
  static int count = 0;
  Serial.print("  B = ");
  Serial.println(count);
  count += 1;
}

const PeriodicTimer::TickType interval = PeriodicTimer::TicksPerSecond * .1;
const PeriodicTimer::TickType intervalA = interval;
const PeriodicTimer::TickType intervalB = interval * 10;

PeriodicTimer counterA(countA, intervalA);
PeriodicTimer counterB(countB, intervalB);

void loop() {
  Timer timer;
  counterA.tick();
  counterB.tick();
  Serial.print("Calling counters took: ");
  Serial.print(timer.elapsedTime());
  Serial.println(" uS.");
}