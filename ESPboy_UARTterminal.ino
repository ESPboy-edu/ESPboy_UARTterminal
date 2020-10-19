#include "ESPboyInit.h"
#include "ESPboyTerminalGUI.h"

ESPboyInit myESPboy;
ESPboyTerminalGUI* terminalGUIobj = NULL;

uint8_t hexPrintFlag = 1;

String capitaliseString(String str){
  for(uint8_t i = 0; i< str.length(); i++)
    if(str[i] >= 'a' && str[i] <= 'f') str.setCharAt(i,(char)((uint8_t)str[i]-32));
  return (str);
}

void setup(){
  myESPboy.begin("ESPboy UART terminal");
  terminalGUIobj = new ESPboyTerminalGUI(&myESPboy.tft, &myESPboy.mcp);
  terminalGUIobj->printConsole(F("UART terminal v1.0"), TFT_MAGENTA, 1, 0);
  terminalGUIobj->printConsole(F("set to 115200 baud"), TFT_MAGENTA, 1, 0);
  terminalGUIobj->printConsole("", TFT_BLACK, 1, 0);
  terminalGUIobj->printConsole(F("COMMANDS:"), TFT_MAGENTA, 1, 0);
  terminalGUIobj->printConsole(F(". - change baud"), TFT_MAGENTA, 1, 0);
  terminalGUIobj->printConsole(F("+ - HEX print ON"), TFT_MAGENTA, 1, 0);
  terminalGUIobj->printConsole(F("- - HEX print OFF"), TFT_MAGENTA, 1, 0);
  terminalGUIobj->printConsole("", TFT_BLACK, 1, 0);
  delay(2000);
  terminalGUIobj->printConsole(F("Ready."), TFT_MAGENTA, 1, 0);
  terminalGUIobj->printConsole("", TFT_BLACK, 1, 0);
}


void loop(){
 uint8_t keyPressed;
 uint16_t count;
 uint8_t buf[(128-4)/GUI_FONT_WIDTH];
 String toPrint1;
 String toPrint2;

  keyPressed=0;
  while (!Serial.available() && !keyPressed){
    keyPressed = myESPboy.getKeys();
    delay(1);}
    
  Serial.flush();

  if(keyPressed) {
    toPrint1 = terminalGUIobj->getUserInput();
    terminalGUIobj->printConsole(toPrint1, TFT_GREEN, 1, 0);
    Serial.print(toPrint1);
    if(toPrint1[0]=='.'){
      uint32_t baudInt = 0;
      toPrint1 = toPrint1.substring(1);
      baudInt = toPrint1.toInt();
      if (baudInt){
        Serial.end();
        delay(100);
        Serial.begin(baudInt);
        delay(100);
        toPrint1 = "UART set: " + (String)baudInt;
        terminalGUIobj->printConsole(toPrint1, TFT_MAGENTA, 1, 0);
      }
    }
   if(toPrint1[0]=='+'){
    hexPrintFlag = 1;
    terminalGUIobj->printConsole(F("HEX print ON"), TFT_MAGENTA, 1, 0);
    }
   if(toPrint1[0]=='-'){
    hexPrintFlag = 0;
    terminalGUIobj->printConsole(F("HEX print OFF"), TFT_MAGENTA, 1, 0);
    }
  }

  if (Serial.available()){
    count = 0;
  
    while (Serial.available() && count < ((128-4)/GUI_FONT_WIDTH)) {
      buf[count] = Serial.read();
      count++;
      delay(1);}
    
    toPrint1=""; 
    toPrint2="";
    
    for (uint8_t i=0; i<count; i++){
      if (buf[i] > 31 && buf[i] < 127) toPrint1+=(char)buf[i];
      else toPrint1+=".";
      if (buf[i] < 16) toPrint2 += "0";
      toPrint2 += String(buf[i],HEX);
      toPrint2 += " ";
      }
     
    terminalGUIobj->printConsole(toPrint1, TFT_YELLOW, 1, 0);
    if(hexPrintFlag)
      terminalGUIobj->printConsole(capitaliseString(toPrint2), TFT_BLUE, 1, 0);
  }
  delay(100);
}
