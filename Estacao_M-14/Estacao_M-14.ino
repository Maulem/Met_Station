//**********************************
//CRIADO POR MAULEM

#include <Adafruit_BMP085.h>
#include <LiquidCrystal_PCF8574.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>


//CONSTANTES//
const int DHT_PIN = 2;
const int lcd_address = 0x27;
const int lcd_columns = 16;
const int lcd_rows    = 2;
const int pinochuva = A2;
const int animacoes = "si"; //"sim" PARA ATIVAR AS ANIMACOES//
const String serial = "on"; //"on" PARA ATIVAR O SERIAL PRINT//

//VARIAVEIS//
int Clock = 1800;
float umid;
float temp;
float tempDHT22;
int leitura_A0;
float volt;
float pressao;
float tempBMP085;
float bin;
double tensao, res, lum;
float lump;
String lumCE;
float altitude;
float temperatura;
float chuva;
float chuvar;
String chuvas = "Zero";
int solo;
String solos = "Seco";

//OBJETO LCD//
LiquidCrystal_PCF8574 lcd(lcd_address);

//OBJETO BMP//
Adafruit_BMP085 bmp;

//OBJETO DHT//
DHT dht(DHT_PIN, DHT22);


void setup() {
  
  //INICIA COMUNICACAO SERIAL//
  Serial.begin(9600);
  
  //REFERENCIA ANALOGICA INTERNA//
  analogReference(DEFAULT);
  
  //INICIA O DHT//
  dht.begin();
  
  //INICIA O LCD//
  lcd.begin(lcd_columns, lcd_rows);
  lcd.setBacklight(100); //LIGA A LUZ DE FUNDO//
  Serial.println("System on!");

  //INICIA o FC37 (CHUVA)//
  pinMode(pinochuva, INPUT);

  //INICIA o SOLO//
  pinMode(A3, INPUT);
  
  //TESTE BMP//
  if (bmp.begin()){
    Serial.println("BMP running!");
  }
  else{
    Serial.println("BMP error!");
  }
  
  //INICIA AS ANIMACOES//
  if (animacoes == "sim"){
    animacao();
  }
}

void loop() {
  //PRINT
    medicoes();
    serialPrint();
    lcd.setCursor(0,0);
    lcd.print(temperatura);
    lcd.setCursor(5,0);lcd.print(" *C|");
    lcd.print(" Umid: ");lcd.print(umid);lcd.setCursor(18,0);lcd.print(" %");
    lcd.setCursor(0,1);
    lcd.print("Chance de chuva: ");lcd.print(chuvas);
    








    delay(Clock);
    delay(Clock);
  delay(Clock);
    medicoes();
    serialPrint();
    lcd.setCursor(0,0);
    lcd.print("Chance de chuva:          ");
    lcd.setCursor(0,1);
    lcd.print("  ");lcd.print(chuva);lcd.print(" % ");lcd.print(chuvas);
  delay(Clock);
    medicoes();
    serialPrint();
    lcd.setCursor(0,0);
    lcd.print("Umidade do solo:          ");
    lcd.setCursor(0,1);
    lcd.print("   ");lcd.print(solo);lcd.print(" %  ");lcd.print(solos);
  delay(Clock);

  delay(Clock);
    medicoes();
    serialPrint();
    lcd.setCursor(0,0);
    lcd.print("    Pressao:          ");
    lcd.setCursor(0,1);
    lcd.print("    ");lcd.print(pressao);lcd.print(" ATM       ");
  delay(Clock);
    medicoes();
    serialPrint();
    lcd.setCursor(0,0);
    lcd.print("  Luminosidade:          ");
    lcd.setCursor(0,1);
    lcd.print(lumCE);lcd.print(" (");lcd.print(lump);lcd.print(" %)     ");
  delay(Clock);
    medicoes();
    serialPrint();
    lcd.setCursor(0,0);
    lcd.print("   Altitude:          ");
    lcd.setCursor(0,1);
    lcd.print(" ");lcd.print(altitude);lcd.print(" metros    ");
  delay(Clock);
}


//A FUNCAO medicoes SERVE PARA MEDIR TODOS OS DADOS//

void  medicoes(){
  //SOLO
  solo = analogRead(A3);
  solo = map(solo,550,0,0,100);
  solo = solo+100;
  if (solo >= 50) {
    solos = "Umido         ";
  }
  if (solo < 50) {
    solos = "Seco         ";
  }
  
  //FC37
  chuvar = analogRead(pinochuva);
  chuva = ((chuvar-300)/725)*100;
  chuva = (chuva-100) * -1;
  if (chuva < 20) {
    chuvas = " Zero      ";
  }
  if (chuva >= 20){
    if (chuva < 50){
      chuvas = "Fraca        ";
    }
  }
  if (chuva >= 50) {
    chuvas = "Forte        ";
  }
  
  //DHT22
  umid = dht.readHumidity();
  tempDHT22 = dht.readTemperature();
  umid = 1.00*umid+0.00; //Estabilizar com 2 casas
    
  //BMP085
  pressao = bmp.readPressure()/100;
  pressao = pressao/986.92; //TRANSFORMA A PRESSAO EM ATM
  altitude = bmp.readAltitude();
  tempBMP085 = bmp.readTemperature();
  
  //LDR
  bin = analogRead(A1);
  tensao = bin *5.0f/1023;
  res = 1000 * (5.0f/tensao +1);
  lum = pow(10, 5-log10(res));
  lump = (lum * 100) / 50;
  if (lump > 50) {
    lumCE = "Claro";
    }
  else {
    lumCE = "Escuro";
    }
  //TEMPERATURA MÉDIA
  temperatura = (tempBMP085 + tempDHT22) / 2;
  
}
//A FUNCAO serialPrint SERVE PARA PLOTAR NO MONITOR SERIAL PARA TESTES DOS SENSORES//

void serialPrint(){
  if (serial == "on") {
    Serial.println("*************************************");
    //LINHA 1//
    Serial.print("UmidDHT: ");Serial.print(umid);Serial.print(" % //TempDHT: ");Serial.print(tempDHT22);Serial.println(" C");
    //LINHA 2//
    Serial.print("TempBMP: ");Serial.print(tempBMP085);Serial.print(" C //Pressao: ");Serial.print(pressao);Serial.println(" ATM");
    //LINHA 3//
    Serial.print("Chuva R: ");Serial.print(chuvar);Serial.print(" ");Serial.print(" //Chuva:");Serial.print(chuva);Serial.println(" %");
    //LINHA 4//
    Serial.print("Lum: ");Serial.print(lum);Serial.print("  // Lum%:");Serial.print(lump);Serial.print("  // Solo:");Serial.println(solo);
  }
  else { Serial.println("Serial off");}
}


//ANIMACOES INICIAIS//


void animacao(){
  int n;
  int m;
  int l;
  int o;
  lcd.setCursor(0,0);
  lcd.print("Inicializando");
  lcd.setCursor(13,0);
  int t = 300;
  for(m = 0; m < 3; m++){
    delay(t);
    lcd.setCursor(13,0);
    lcd.print("   ");
    delay(t);
    lcd.setCursor(13,0);
    lcd.print(".  ");
    delay(t);
    lcd.setCursor(13,0);
    lcd.print(".. ");
    delay(t);
    lcd.setCursor(13,0);
    lcd.print("...");
  }
  lcd.setCursor(0,0);
  lcd.print("...Carregando...");
  for(n = 0; n < 17; n++){
    lcd.setCursor(n,1);
    lcd.print("=");
    delay(100);
  }
  lcd.setCursor(0,0);
  lcd.print("                      ");
  lcd.setCursor(0,1);
  lcd.print("                      ");
  for(l = 3; l <18 ; l++){
    int temp = 17-l;
    lcd.setCursor(temp,0);
    lcd.print("  Bem vindo a       ");
    delay(200);
  }
  for(o = 3; o <18 ; o++){
    int tempo = 17-o;
    lcd.setCursor(tempo,1);
    lcd.print("  estacao M-14!         ");
    delay(200);
  }
  delay(500);
}
