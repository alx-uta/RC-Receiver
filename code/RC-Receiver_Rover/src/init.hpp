/**
 * RC Receiver – ESP32 / SX1280
 * https://github.com/alx-uta/RC-Receiver
 * 
 * Alex Uta
 * microknot.dev
 */

Config _RX_CONFIG;

/**
 * RadioLib Init
 * 
 * This is used to transmit the data
 */
SX1280 radio = new Module(
  _RX_CONFIG.SX1280_NSS,
  _RX_CONFIG.SX1280_DI01,
  _RX_CONFIG.SX1280_NRST,
  _RX_CONFIG.SX1280_BUSY
);

// flag to indicate that a packet was received
volatile bool receivedFlag = false;

// disable interrupt when it's not needed
volatile bool enableInterrupt = true;

// this function is called when a complete packet
// is received by the module
// IMPORTANT: this function MUST be 'void' type
//            and MUST NOT have any arguments!
#if defined(ESP8266) || defined(ESP32)
  ICACHE_RAM_ATTR
#endif
void setFlag(void) {
  // check if the interrupt is enabled
  if(!enableInterrupt) {
    return;
  }

  // we got a packet, set the flag
  receivedFlag = true;
}

/**
 * Servo Control
*/
Servo SERVO;
Servo ESC;

/**
 * Define the tasks and cores
 */
static int taskCore1 = 0;
TaskHandle_t task_txReceiveData;

static int taskCore2 = 1;
TaskHandle_t task_updateTFT;

/**
 *
 * RX init
 */
Rx _RX(
    // Config
    _RX_CONFIG
);


// Lights Init
#define LED_PIN     32        // GPIO 32
#define NUM_LEDS    17        // Total number of LEDs
#define DELAY_MS    100       // Delay between lighting each LED

Adafruit_NeoPixel _LIGHTS(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

/**
 * Rover init
 */
Rover _ROVER(
    _RX_CONFIG,
    _RX,
    _LIGHTS
);

// Maximum payload size
const int _payload_size = 18;
uint8_t _payload[_payload_size];
