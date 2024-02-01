#include <Wire.h> 
#include <EEPROM.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

int pb = 10;
int clk = 12;
int dt = 11;

int tampilan = 0;
int menu = 0;

int hasil;
int hitung = 0;

String menuawal;
int posisiawal;
unsigned long posisibuttonakhir = 0;

//nilai
int nilai1 = EEPROM.read(1);
int nilai2 = EEPROM.read(2);
int nilai3 = EEPROM.read(3);
int nilai4 = EEPROM.read(4);

//long press button
int durasi = 1000;
int waktuakhir = LOW;
unsigned long tekan = 0;
unsigned long lepas = 0;

//reset
void(*saya_reset) (void) = 0;

void setup() {
  Serial.begin(9600);
  pinMode(pb, INPUT_PULLUP);
  pinMode(clk, INPUT);
  pinMode(dt, INPUT);

  lcd.begin();
  lcd.setCursor(0,0);
  lcd.print("Menu Pengaturan");
  
  posisiawal = digitalRead(clk);
}

void loop(){
//counter rotasi
  int posisiakhir = digitalRead(clk);
  if(posisiakhir != posisiawal){
    if(digitalRead(dt) != posisiakhir){
       if (tampilan == 0){
           menu++; 
          if (menu > 4){
             menu = 1;
          }  
           mainmenu(menu);
       }
       if (tampilan == 1){
           if( menu == 1 ){
            nilai1++;
           } 
           if (menu == 2){
            nilai2++;
           }
           if (menu == 3){
            nilai3++;
           }
           if (menu == 4){
            nilai4++;
           }
           submenu(menu);
       }
    } else {
      if (tampilan == 1){
           menu--; 
          if ( menu < 0){
             menu = 4;
          }
           mainmenu(menu);
       }
       if (tampilan == 0){
           if( menu == 1 ){
            nilai1--;
           } 
           if (menu == 2){
            nilai2--;
           }
           if (menu == 3){
            nilai3--;
           }
           if (menu == 4){
            nilai4--;
           }
           submenu(menu);
           
       }
    }
     posisiawal = posisiakhir;
  }
//counter button
  int posisibutton = digitalRead(pb);
  if(posisibutton == LOW){
    if( millis() - posisibuttonakhir > 500){
     
      switch(tampilan){
        case 0:
        Serial.println(tampilan);
        tampilan = 1;
        mainmenu(menu);
        break;
        case 1:
        Serial.println(tampilan);
        tampilan = 0;
        submenu(menu);
        break;
        case 2:
        Serial.println(tampilan);
        tampilan = 2;
        break;
        
      }
    } 
     posisibuttonakhir = millis();
  }



 //save and reset
   if(waktuakhir == HIGH && posisibutton == LOW){       
      tekan = millis();
   }else if(waktuakhir == LOW && posisibutton == HIGH) { 
      lepas = millis();
  
      long durasitekan = lepas - tekan;

      if( durasitekan > durasi && durasitekan < 2000){

          EEPROM.write(1, nilai1);
          EEPROM.write(2, nilai2);
          EEPROM.write(3, nilai3);
          EEPROM.write(4, nilai4);
      
          lcd.clear();
          lcd.setCursor(0,0);\
          lcd.print("saved");
          
      }else if (durasitekan > 2000){
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("reset");
          saya_reset();
      }
   
    }
      waktuakhir = posisibutton;
  }
  

void mainmenu(int menu){
  switch(menu){
    case 1:
       lcd.clear();
       lcd.setCursor(0,0);
       lcd.print("Menu pilihan");
       lcd.setCursor(0,1);
       lcd.print("1. set RPM");
       break;
    case 2:
       lcd.clear();
       lcd.setCursor(0,0);
       lcd.print("Menu pilihan");
       lcd.setCursor(0,1);
       lcd.print("2. set RPS");
       break;
    case 3:
    lcd.clear();
       lcd.setCursor(0,0);
       lcd.print("Menu pilihan");
       lcd.setCursor(0,1);
       lcd.print("3. set Kecepatan");
       break;
    case 4:
    lcd.clear();
       lcd.setCursor(0,0);
       lcd.print("Menu pilihan");
       lcd.setCursor(0,1);
       lcd.print("4. set PID");
       break;
  }
}

void submenu( int menu ){
  switch(menu){
    case 1:
       lcd.clear();
       lcd.setCursor(0,0);
       lcd.print("1. set RPM");
       lcd.setCursor(0,1);
       lcd.print("nilai = ");
       lcd.setCursor(9,1);
       lcd.print(nilai1);
       break;
    case 2:
       lcd.clear();
       lcd.setCursor(0,0);
       lcd.print("2. set RPS");
       lcd.setCursor(0,1);
       lcd.print("nilai = ");
       lcd.setCursor(9,1);
       lcd.print(nilai2);
       break;
    case 3:
       lcd.clear();
       lcd.setCursor(0,0);
       lcd.print("3. set Kecepatan");
       lcd.setCursor(0,1);
       lcd.print("nilai = ");
       lcd.setCursor(9,1);
       lcd.print(nilai3);
       break;
    case 4:
       lcd.clear();
       lcd.setCursor(0,0);
       lcd.print("4. set PID");
       lcd.setCursor(0,1);
       lcd.print("nilai = ");
       lcd.setCursor(9,1);
       lcd.print(nilai4);
       break;    
  }
}
