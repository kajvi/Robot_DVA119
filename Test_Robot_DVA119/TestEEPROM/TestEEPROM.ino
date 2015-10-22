/* LoadAndSaveSettings
 * Joghurt 2010
 * Demonstrates how to load and save settings to the EEPROM
 */
// Contains EEPROM.read() and EEPROM.write()
#include <EEPROM.h>

#define C_THIS_VERSION "Test eeprom"

// ID of the settings block
#define CONFIG_VERSION "ls1"

// Tell it where to store your config data in EEPROM
#define CONFIG_START 32

// Example settings structure
struct StoreStruct {
  // This is for mere detection if they are your settings
  char version[4];
  // The variables of your settings
  int a, b;
  char c;
  long d;
  float e[6];
};
struct StoreStruct storage;


void loadConfig() {
  // To make sure there are settings, and they are YOURS!
  // If nothing is found it will use the default settings.
     for (unsigned int t=0; t<sizeof(storage); t++)
      *((char*)&storage + t) = EEPROM.read(CONFIG_START + t);
}

void saveConfig() {
  for (unsigned int t=0; t<sizeof(storage); t++)
    EEPROM.write(CONFIG_START + t, *((char*)&storage + t));
}

void setup() {
  Serial.begin(9600); // set up Serial library at 9600 bps
  Serial.println(C_THIS_VERSION);
  loadConfig();
  Serial.print("a: " + String(storage.a) + "\t b: " + String(storage.b) + "\t c: " + storage.c + "\t d: " + String(storage.d) + "\n");
  //saveConfig();
  Serial.print("a: " + String(storage.a) + "\t b: " + String(storage.b) + "\t c: " + storage.c + "\t d: " + String(storage.d) + "\n");
}

void loop() {
 /*
  // [...]
  int i = storage.c - 'a';
  // [...]

  // [...]
  storage.c = 'a';
  //if (ok)
  saveConfig();
  // [...]
*/
}
