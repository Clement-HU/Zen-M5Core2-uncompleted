#include "m5core2zen.hpp"

struct Vars {
  int *intvars;
};

int readInt(char *(&d), Vars &vars) {
  int i = 0, m = 1;
  if (*d == 'I') {
    d++;
    Serial.print("\n   ");
    i = vars.intvars[readInt(d, vars)] * 10;
  } else {
    if (*d == '-') {
      d++;
      m = -1;
    }
    if ((*d <= '9') && (*d >= '0')) {
      do {
        i += *d - '0';
        i *= 10;
        d++;
      } while ((*d <= '9') && (*d >= '0'));
    }
    d++;
  }
  Serial.print("READED INT ");
  Serial.print(i * m / 10);
  Serial.print('\n');
  return i * m / 10;
}

Zen::Zen() {}

Zen::~Zen() {}

void Zen::M5begin() {
  M5.begin();
  M5.Lcd.println("Zen interpreter for Core 2");
  M5.Lcd.println("Version -1.0.3");
  M5.Lcd.println("By Clement HU 2024-2025");
  disableCore0WDT();
  if (!SD.begin()) {
    M5.Lcd.println("SD Card Failed!");
    M5.Lcd.println("Install your SD card and reboot!");
    while (1)
      ;
  }
  M5.Lcd.println("SD card found!Now starting...");
  delay(500);
  M5.Lcd.fillScreen(0xffff);
}

char Zen::execute(char *name) {
  //Initalizing screen buffer
  TFT_eSprite buf(&M5.Lcd);
  buf.createSprite(240, 240);
  //Loading the file
  File f = SD.open(name, FILE_READ);
  if (!f) return 'e';
  //Reading the file
  int size = f.size();
  char *code = new char[size];
  f.read((uint8_t *)code, size);
  //Initalizing pointer
  char *ptr = code;
  //Core command buffer
  char lib;
  char action;
  int i1;
  //variables
  Vars vars;
  for (; ptr < code + size;) {
    lib = *ptr;
    ptr++;
    action = *ptr;
    ptr++;
    ptr++;
    Serial.print("RUNNING COMMAND ");
    Serial.print(lib);
    Serial.print(action);
    Serial.print('\n');
    switch (lib) {
      case '$':  //zen sys
        switch (action) {
          case 'Q':      //quit
            return 's';  //project running succeeded
          case 'D':      //delay
            delay(readInt(ptr, vars));
            break;
          case 'G':  //goto
            i1 = readInt(ptr, vars);
            if (i1 > 0)
              for (char *m = ptr; m < code + size; m++) {
                if (m >= code && m < code + size - 2 && *m == '$' && *(m + 1) == 'M') {
                  i1--;
                  if (i1 == 0) {
                    ptr = m + 3;
                    break;
                  }
                }
              }
            else if (i1 < 0)
              for (char *m = ptr; m >= code; m--) {
                if (m >= code && m < code + size - 2 && *m == '$' && *(m + 1) == 'M') {
                  i1++;
                  if (i1 == 0) {
                    ptr = m + 3;
                    break;
                  }
                }
              }
            break;
          case 'M':  //mark
            break;   //do nothing
        }
        break;
      case 'D':  //draw
        switch (action) {
          case 'U':  //update
            buf.pushSprite(80, 0);
            break;
          case 'L':  //line
            buf.drawLine(readInt(ptr, vars), readInt(ptr, vars), readInt(ptr, vars), readInt(ptr, vars), M5.Lcd.color565(readInt(ptr, vars), readInt(ptr, vars), readInt(ptr, vars)));
            break;
          case 'R':  //rect
            action = *ptr;
            ptr++;
            ptr++;
            Serial.print("READED SUB-COMMAND ");
            Serial.println(action);
            if (action == 'O')
              buf.drawRect(readInt(ptr, vars), readInt(ptr, vars), readInt(ptr, vars), readInt(ptr, vars), M5.Lcd.color565(readInt(ptr, vars), readInt(ptr, vars), readInt(ptr, vars)));
            else if (action == 'F')
              buf.fillRect(readInt(ptr, vars), readInt(ptr, vars), readInt(ptr, vars), readInt(ptr, vars), M5.Lcd.color565(readInt(ptr, vars), readInt(ptr, vars), readInt(ptr, vars)));
            else if (action == 'R')
              buf.drawRoundRect(readInt(ptr, vars), readInt(ptr, vars), readInt(ptr, vars), readInt(ptr, vars), readInt(ptr, vars), M5.Lcd.color565(readInt(ptr, vars), readInt(ptr, vars), readInt(ptr, vars)));
            else if (action == 'E')
              buf.fillRoundRect(readInt(ptr, vars), readInt(ptr, vars), readInt(ptr, vars), readInt(ptr, vars), readInt(ptr, vars), M5.Lcd.color565(readInt(ptr, vars), readInt(ptr, vars), readInt(ptr, vars)));
            break;
        }
        break;
      case 'V':  //variables
        switch (action) {
          case 'A':  //allocate
            action = *ptr;
            ptr += 2;
            Serial.print("READED SUB-COMMAND ");
            Serial.println(action);
            if (action == 'I')  //int
              vars.intvars = new int[readInt(ptr, vars)];
            break;
          case 'D':  //delete
            action = *ptr;
            ptr += 2;
            Serial.print("READED SUB-COMMAND ");
            Serial.println(action);
            if (action == 'I')  //int
              delete[] vars.intvars;
            break;
          case 'M':  //modify
            action = *ptr;
            ptr += 2;
            Serial.print("READED SUB-COMMAND ");
            Serial.println(action);
            if (action == 'I')  //int
              vars.intvars[readInt(ptr, vars)] = readInt(ptr, vars);
            break;
        }
        break;
      case 'O':  //operators
        switch (action) {
          case '+':  //add
            i1 = readInt(ptr, vars) + readInt(ptr, vars);
            Serial.println(i1);
            vars.intvars[readInt(ptr, vars)] = i1;
            break;
        }
    }
  }
  delete[] vars.intvars;
}