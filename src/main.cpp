#include <Arduino.h>
#include <FastLED.h>
#define WP 6
#define WR 5
#define WB 4
#define WN 3
#define WQ 2
#define WK 1
#define BP 12
#define BR 11
#define BB 10
#define BN 9
#define BQ 8
#define BK 7
#define NO 0
#define OutSta true
#define dataIn 3
#define clockIN 5
#define load 4
int state[64]={
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0
};
int addressing[64] = {
  63, 62, 61, 60, 55, 54, 53, 52,
  56, 57, 58, 59, 48, 49, 50, 51,
  47, 46, 45, 44, 39, 38, 37, 36,
  40, 41, 42, 43, 32, 33, 34, 35,
  31, 30, 29, 28, 23, 22, 21, 20,
  24, 25, 26, 27, 16, 17, 18, 19,
  15, 14, 13, 12, 7, 6, 5, 4,
  8, 9, 10, 11, 0, 1, 2, 3
};
int boardstatus[8][8] = {
    {WR, WN, WB, WK, WQ, WB, WN, WR},
    {WP, WP, WP, WP, WP, WP, WP, WP},
    {NO, NO, NO, NO, NO, NO, NO, NO},
    {NO, NO, NO, NO, NO, NO, NO, NO},
    {NO, NO, NO, NO, NO, NO, NO, NO},
    {NO, NO, NO, NO, NO, NO, NO, NO},
    {BP, BP, BP, BP, BP, BP, BP, BP},
    {BR, BN, BB, BK, BQ, BB, BN, BR}};
bool ispickup = false;
int pickup = NO;
int pickupx,pickupy;
bool readyeat = false;
CRGB leds[64];
void chessRead();
void displayclean();
int display(int ,int);
void setup()
{
  Serial.begin(115200);
  FastLED.addLeds<WS2812B, 2, GRB>(leds, 64);
  for(int i=0;i<64;i++){
    
    leds[i]=CRGB(255-(i/8)*32,0,255-(i%8)*32);
  }
  FastLED.show();
  // displayclean();
}
void loop()
{
  chessRead();
  for(int i=0;i<64;i++){
    if(i%8==7)
      Serial.println(state[i]);
    else
      Serial.print(state[i]);
  }
  delay(2000);
  /*for (int i = 0; i < 64; i++)
  {
    int y=i/8;
    int x=i%8;
    if (state[i] && boardstatus[y][x] != NO && !ispickup)
    {
      ispickup = true;
      pickupx=x;
      pickupy=y;       
      switch (boardstatus[y][x])
      {
      case BP:
        leds[i - 8] = CRGB(255, 255, 0);
        if (y==6)
          leds[i - 16] = CRGB(255, 255, 0);
        if (boardstatus[y - 1][x + 1] > 6)
          leds[i - 7] = CRGB(255, 0, 0);
        if (boardstatus[y - 1][x - 1] > 6)
          leds[i - 9] = CRGB(255, 0, 0);
        break;

      case WP:
        leds[i + 8] = CRGB(255, 255, 0);
        if (y==1)
          leds[i + 16] = CRGB(255, 255, 0);
        if (boardstatus[y + 1][x + 1] > 6)
          leds[i + 9] = CRGB(255, 0, 0);
        if (boardstatus[y + 1][x - 1] > 6)
          leds[i + 7] = CRGB(255, 0, 0);
        break;

      case WN:
      case BN:
        if (x < 6 && y < 7)
          display(x + 2, y + 1);
        if (x < 7 && y < 6)
          display(x + 1, y + 2);
        if (x > 1 && y > 0)
          display(x - 2, y - 1);
        if (x > 0 && y > 1)
          display(x - 1, y - 2);
        if (x < 6 && y > 0)
          display(x + 2, y - 1);
        if (x < 7 && y > 1)
          display(x + 1, y - 2);
        if (x > 1 && y < 7)
          display(x - 2, y + 1);
        if (x > 0 && y < 6)
          display(x - 1, y + 2);
        break;
      case WB:
      case BB:        
        if(x>y){
          for(int w=x+1;w<8;w++){
            if(display(w,y+w-x))break;
          }
          for(int w=y-1;w>=0;w--){
            if(display(x+w-y,w))break;
          }
        }
        else{
          for(int w=y+1;w<8;w++){
            if(display(x+w-y,w))break;
          }
          for(int w=x-1;w>=0;w--){
            if(display(w,y+w-x))break;
          }
        }
        if(8-x>y){
          for(int w=x-1;w>=0;w--){
            if(display(w,y+x-w))break;
          }
          for(int w=y-1;w>=0;w--){
            if(display(x+y-w,w))break;
          }
        }
        else{
          for(int w=x+1;w<8;w++){
            if(display(w,y+x-w))break;
          }
          for(int w=y+1;w<8;w++){
            if(display(x+y-w,w))break;
          }
        }
        
        break;
      case WR:
      case BR:        
        for(int w=x+1;w<8;w++){
          if(display(w,y))break;
        }
        for(int w=x-1;w>=0;w--){
          if(display(w,y))break;
        }
        for(int w=y+1;y<8;y++){
          if(display(x,w))break;
        }
        for(int w=y-1;y>=0;y--){
          if(display(x,w))break;
        }
        
        break;
      case WK:
      case BK:        
        if(x<7&&y<7)display(x+1,y+1);
        if(x>0&&y<7)display(x-1,y+1);
        if(x>0&&y>0)display(x-1,y-1);
        if(x<7&&y>0)display(x+1,y-1);
        if(x<7)display(x+1,y);
        if(x>0)display(x-1,y);
        if(y<7)display(x,y+1);
        if(y>0)display(x,y-1);        
        break;
       case WQ:
       case BQ:        
        if(x>y){
          for(int w=x+1;w<8;w++){
            if(display(w,y+w-x))break;
          }
          for(int w=y-1;w>=0;w--){
            if(display(x+w-y,w))break;
          }
        }
        else{
          for(int w=y+1;w<8;w++){
            if(display(x+w-y,w))break;
          }
          for(int w=x-1;w>=0;w--){
            if(display(w,y+w-x))break;
          }
        }
        if(8-x>y){
          for(int w=x-1;w>=0;w--){
            if(display(w,y+x-w))break;
          }
          for(int w=y-1;w>=0;w--){
            if(display(x+y-w,w))break;
          }
        }
        else{
          for(int w=x+1;w<8;w++){
            if(display(w,y+x-w))break;
          }
          for(int w=y+1;w<8;w++){
            if(display(x+y-w,w))break;
          }
        }
        for(int w=x+1;w<8;w++){
          if(display(w,y))break;
        }
        for(int w=x-1;w>=0;w--){
          if(display(w,y))break;
        }
        for(int w=y+1;y<8;y++){
          if(display(x,w))break;
        }
        for(int w=y-1;y>=0;y--){
          if(display(x,w))break;
        }        
        break; 
      }      
      pickup=boardstatus[y][x];
      Serial.println(i+1);
      boardstatus[y][x]=NO; 
      leds[i] = CRGB(0, 255, 0);
      FastLED.show();
    }
    if(ispickup && !state[i] && (leds[i]==CRGB(255, 255, 0)||leds[i]==CRGB(0, 255, 0)))
    {
      ispickup=false;        
      Serial.println(i+1);
      boardstatus[y][x]=pickup;
      boardstatus[pickupy][pickupx]=NO;
      displayclean();
      FastLED.show();
    }
    if(ispickup && state[i] && leds[i]==CRGB(255,0,0) && !readyeat){
        readyeat=true;
    }
    if(ispickup && !state[i] && leds[i]==CRGB(255,0,0) && readyeat){
      ispickup=false;
      readyeat=false;
      Serial.println(i+1);
      boardstatus[y][x]=pickup;
      boardstatus[pickupy][pickupx]=NO;
      displayclean();
      FastLED.show();
    }
  }*/
}
int display(int x, int y)
{
  if (boardstatus[y][x] == 0)
  {
    leds[y * 8 + x] = CRGB(255, 255, 0);
  }
  if ((boardstatus[y][x] > 6 && pickup <= 6) || (boardstatus[y][x] <= 6 && pickup > 6))
  {
    leds[y * 8 + x] = CRGB(255, 0, 0);
  }
  if(boardstatus[y][x]!=0)
    return 1;
  return 0;
}
void displayclean(){
  for(int i=0;i<64;i++){
    leds[i]=CRGB(0,0,0);
    }
    FastLED.show();
}
void chessRead()
{
  digitalWrite(load, 0);
  delayMicroseconds(3);
  digitalWrite(load, 1);
  for (int i = 0; i < 64; i++)
    {
      state[addressing[i]]=digitalRead(dataIn);
      digitalWrite(clockIN, 1);
      delayMicroseconds(3);
      digitalWrite(clockIN, 0);
    }
    
}