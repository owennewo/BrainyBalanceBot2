#include <Display.h>
#include <TFT_eSPI.h>
#include <Data.h>
#include <User_Defines.h>
TFT_eSPI tft = TFT_eSPI(135, 240);
TFT_eSprite sprite = TFT_eSprite(&tft);
  
void DisplaySetup() {
    tft.init();
    tft.setRotation(1);
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_WHITE);
    tft.setCursor(0, 0);
    tft.setTextDatum(ML_DATUM);
    tft.setTextSize(2);

    sprite.setColorDepth(8);
    sprite.setTextSize(2);
    sprite.createSprite(tft.width(), tft.height());


    if (PIN_TFT_BL > 0) { // TFT_BL has been set in the TFT_eSPI library in the User Setup file TTGO_T_Display.h
         pinMode(PIN_TFT_BL, OUTPUT); // Set backlight pin to output mode
         digitalWrite(PIN_TFT_BL, TFT_BACKLIGHT_ON); // Turn backlight on. TFT_BACKLIGHT_ON has been set in the TFT_eSPI library in the User Setup file TTGO_T_Display.h
    }

    tft.setSwapBytes(true);
}

void DisplayVoltage()
{
    uint16_t v = analogRead(PIN_BATTERY_ADC);
    float battery_voltage = ((float)v / 4095.0) * 2.0 * 3.3 * (VREF / 1000.0);
    String voltage = String(battery_voltage) + " V";
    sprite.setTextColor(TFT_BLUE);
    sprite.drawString(voltage,  180, 5 );

}

void drawStats(float value, float max, int y, uint32_t color) {

    float x1 = map(value, -max, max, 0, sprite.width());
    float x2 = sprite.width() / 2;

    if (x1 < x2) {
        sprite.fillRect(x1, y, x2 - x1, 8, color);
    } else {
        sprite.fillRect(x2, y, x1 - x2, 8, color);
    }
    
}

void DisplayMotion(BotState bot) {
    drawStats(bot.steeringCommand, MAX_SPEED, 60, TFT_GREEN);
    drawStats(bot.speedCommand, MAX_SPEED, 70, TFT_ORANGE);
    
    drawStats(bot.speed, MAX_SPEED, 80, TFT_GREEN);
    drawStats(bot.speedTarget, MAX_SPEED, 90, TFT_ORANGE);

    drawStats(bot.angle, CRASH_ANGLE, 100, TFT_BLUE);
    drawStats(bot.angleTarget, CRASH_ANGLE, 110, TFT_RED);
}

void DisplayStatus(boolean crashed) {
    if (crashed) {
        sprite.setTextColor(TFT_RED);
        sprite.drawString("CRASH!",  5, 5 );
    } else {
        sprite.setTextColor(TFT_GREEN);
        sprite.drawString("OK!",  5, 5 );
    }
}

void DisplayLoop(BotState bot) {
    
    Serial.printf("\ra:%5.1f at:%5.1f av:%3.2f s:%5d st:%5d i:%4.3f", bot.angle, bot.angleTarget, bot.angularSpeed, bot.speed, bot.speedTarget, bot.sampleInterval);
    
    static uint64_t timeStamp = 0;
    if (millis() - timeStamp > 100) {
        timeStamp = millis();
        sprite.fillSprite(TFT_BLACK);
        DisplayStatus(bot.crashed);
        DisplayVoltage();
        DisplayMotion(bot);
        sprite.pushSprite(0,0);
    }
}