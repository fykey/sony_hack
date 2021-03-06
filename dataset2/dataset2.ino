#include <Camera.h>
#include <SDHCI.h>
SDClass theSD;
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"

#define TFT_RST 8
#define TFT_DC  9
#define TFT_CS  10

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS ,TFT_DC ,TFT_RST);
int gCounter = 0;
int i = 0, j = 0, k = 0, intPin = 4;


void setup() {
  pinMode(LED0, OUTPUT);
  pinMode(intPin, INPUT_PULLUP);
  Serial.begin(115200);
  theSD.begin();
  tft.begin();
  tft.setRotation(3);
  theCamera.begin();
  theCamera.startStreaming(true, CamCB);
  theCamera.setStillPictureImageFormat(
    320, 240, CAM_IMAGE_PIX_FMT_YUV422);//420かも

}
void CamCB(CamImage img){//写真を撮られるときに呼び出される
  if (img.isAvailable()) {
    img.convertPixFormat(CAM_IMAGE_PIX_FMT_RGB565);
    tft.drawRGBBitmap(0, 0, (uint16_t *)img.getImgBuff(), 320, 240);
    
  }
}


void loop() {
  digitalWrite(LED0, HIGH);
  Serial.print("take picture ");
  Serial.println(j, DEC);
  CamImage img = theCamera.takePicture();
  if(!img.isAvailable()){
    Serial.println("take picture failed");return;

  }
  CamImage small;
  CamErr err = img.clipAndResizeImageByHW(
    small, 48, 8, 271, 231, 28, 28);

    if(err){
      Serial.println("ERROR: " + String(err));
      return;
    }
    
    if(!small.isAvailable()){
      Serial.println("clipAndResize failed"); return;
    }
    char filename[16] = {0};
    sprintf(filename, "PICT%03d.pgm", gCounter);
    File myFile = theSD.open(filename, FILE_WRITE);
    myFile.println("P5");
    myFile.println("28 28");
    myFile.println("255");
    uint16_t* buf = (uint16_t*)small.getImgBuff();

    for(int i = 0; i < 28; ++i){
      for(int j = 0; j < 28; ++j){
        uint8_t val = (uint8_t)((*buf & 0xff00) >> 8);
        myFile.write(val);
        ++buf;
      }
    }
    ++gCounter;
    myFile.close();
      theCamera.startStreaming(true, CamCB);
  digitalWrite(LED0, LOW);
  if(j > 300){
    Serial.println("お疲れ様でした");
    delay(5000);
  }
  j++;


}
