/*
  Spidey Ornament - Speaker-Triggered Motor Controller
  Hardware: ATtiny402 (megaTinyCore) + DRV8838 H-bridge

  PCB pin mapping, verified against Spidey_Compact_C1:
    PA1 = AUDIO_ADC (speaker sensing, AC-coupled and biased to VCC/2)
    PA2 = MOTOR_PH  (motor direction)
    PA3 = MOTOR_PWM (DRV8838 EN, PWM motor speed)

  Sequence: speaker sound -> raise -> pause -> lower ->
            wait until speaker has been quiet for 1 s -> rearm.

  Hardware note: R8's original 100k pullup must be replaced or
  bridged as repaired on the prototype so DRV8838 nSLEEP is HIGH.
*/

const uint8_t AUDIO_PIN  = PIN_PA1;
const uint8_t MOTOR_PH   = PIN_PA2;
const uint8_t MOTOR_EN   = PIN_PA3;

// Motor values from the original working prototype.
const uint8_t MOTOR_SPEED = 165;  // 0..255, about 65% duty cycle
const unsigned long UP_TIME_MS   = 8000UL;
const unsigned long TOP_PAUSE_MS = 2000UL;
const unsigned long DOWN_TIME_MS = 8000UL;

// Change LOW to HIGH if the first movement lowers Spider-Man.
const uint8_t UP_PH_LEVEL = LOW;

// Speaker detection: amplitude in ADC counts, not absolute ADC value.
// The new PCB biases its audio input near half the battery voltage.
const uint16_t AUDIO_THRESHOLD = 40;  // peak-to-peak, 10-bit ADC
const uint8_t AUDIO_SAMPLES = 48;
const uint8_t SOUND_CONFIRM_WINDOWS = 2;
const unsigned long REARM_SILENCE_MS = 1000UL;

enum SequenceState {
  WAIT_FOR_SOUND,
  RAISING,
  PAUSING_AT_TOP,
  LOWERING,
  WAIT_FOR_SILENCE
};

SequenceState state = WAIT_FOR_SOUND;
unsigned long stateStartedAt = 0;
unsigned long silenceStartedAt = 0;
bool countingSilence = false;
uint8_t consecutiveSoundWindows = 0;

void stopMotor() {
  analogWrite(MOTOR_EN, 0);  // Disable H-bridge: coast
  digitalWrite(MOTOR_EN, LOW);
}

void startMotor(bool raising) {
  // Ensure the bridge is off while selecting direction.
  stopMotor();
  uint8_t direction = raising ? UP_PH_LEVEL
                             : (UP_PH_LEVEL == LOW ? HIGH : LOW);
  digitalWrite(MOTOR_PH, direction);
  analogWrite(MOTOR_EN, MOTOR_SPEED);
}

// Returns true if we saw sufficient variation in a short ADC window.
// Using (max - min) works with the 100k/100k midpoint bias network.
bool speakerIsActive() {
  int minimum = 1023;
  int maximum = 0;

  for (uint8_t i = 0; i < AUDIO_SAMPLES; ++i) {
    int sample = analogRead(AUDIO_PIN);
    if (sample < minimum) minimum = sample;
    if (sample > maximum) maximum = sample;
  }

  return (maximum - minimum) >= AUDIO_THRESHOLD;
}

void setup() {
  pinMode(MOTOR_PH, OUTPUT);
  pinMode(MOTOR_EN, OUTPUT);
  pinMode(AUDIO_PIN, INPUT); // No pull-up: the PCB provides midpoint bias

  stopMotor();
  digitalWrite(MOTOR_PH, UP_PH_LEVEL);

  // megaTinyCore defaults to 10-bit ADC; state explicitly for clarity.
  analogReadResolution(10);
  // Allow extra ADC sample time for the relatively high impedance
  // of the speaker-sense bias circuit.
  analogSampleDuration(20);

  delay(500);  // Let power and the AC-coupled audio input stabilize.
}

void loop() {
  const bool soundNow = speakerIsActive();
  const unsigned long now = millis();

  switch (state) {
    case WAIT_FOR_SOUND:
      if (soundNow) {
        if (consecutiveSoundWindows < SOUND_CONFIRM_WINDOWS) {
          ++consecutiveSoundWindows;
        }
      } else {
        consecutiveSoundWindows = 0;
      }

      if (consecutiveSoundWindows >= SOUND_CONFIRM_WINDOWS) {
        consecutiveSoundWindows = 0;
        startMotor(true);
        stateStartedAt = now;
        state = RAISING;
      }
      break;

    case RAISING:
      if (now - stateStartedAt >= UP_TIME_MS) {
        stopMotor();
        stateStartedAt = now;
        state = PAUSING_AT_TOP;
      }
      break;

    case PAUSING_AT_TOP:
      if (now - stateStartedAt >= TOP_PAUSE_MS) {
        startMotor(false);
        stateStartedAt = now;
        state = LOWERING;
      }
      break;

    case LOWERING:
      if (now - stateStartedAt >= DOWN_TIME_MS) {
        stopMotor();
        countingSilence = false;
        state = WAIT_FOR_SILENCE;
      }
      break;

    case WAIT_FOR_SILENCE:
      if (soundNow) {
        countingSilence = false;
      } else if (!countingSilence) {
        silenceStartedAt = now;
        countingSilence = true;
      } else if (now - silenceStartedAt >= REARM_SILENCE_MS) {
        countingSilence = false;
        consecutiveSoundWindows = 0;
        state = WAIT_FOR_SOUND;
      }
      break;
  }
}
