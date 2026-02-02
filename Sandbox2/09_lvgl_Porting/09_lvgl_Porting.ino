// Version 1.2.0 - EEZ Flow Integration & Thread Safety
#define LV_LVGL_H_INCLUDE_SIMPLE
#define EEZ_FOR_LVGL 1

#include <eez-framework.h>
#include <Arduino.h>
#include <esp_display_panel.hpp>
#include <lvgl.h>
#include <ui/lvgl_v8_port.h>

// Extern "C" Block für die Zusammenarbeit von C (EEZ) und C++ (Arduino)
//extern "C" {
    #include <ui/ui.h>
    #include <ui/vars.h>
    // Falls EEZ Studio zusätzliche Flow-Dateien exportiert hat:
    // #include "actions.h" 
//}

using namespace esp_panel::drivers;
using namespace esp_panel::board;

#define native_var_t unit32_t 
int32_t nativ_val;

int32_t get_var_nativ_val() {
    return nativ_val;
}

void set_var_nativ_val(int32_t value) {
    nativ_val = value;
}


void setup() {
    Serial.begin(115200);

    // 1. Hardware Initialisierung (Waveshare Demo)
    Serial.println("Board Init...");
    Board *board = new Board();
    board->init();
    assert(board->begin());

    // 2. LVGL Port Initialisierung
    Serial.println("LVGL Port Init...");
    lvgl_port_init(board->getLCD(), board->getTouch());

    // 3. EEZ Studio UI & Flow Initialisierung
    Serial.println("EEZ UI & Flow Init...");
    
    // Wir müssen den Mutex sperren, wenn wir LVGL-Objekte initialisieren
    if (lvgl_port_lock(-1)) {
        ui_init(); 
        lvgl_port_unlock();
    }

    Serial.println("System Ready.");
}

void loop() {
    /* * ui_tick() verarbeitet die Flow-Logik von EEZ Studio.
     * Da Flows oft LVGL-Objekte (wie Label-Texte) ändern, 
     * MUSS dies innerhalb des lvgl_port_lock erfolgen.
     */
    if (lvgl_port_lock(-1)) {
        ui_tick(); 
        lvgl_port_unlock();
    }

    // Kleiner Delay, um den anderen Tasks Zeit zu geben
    delay(5);
}