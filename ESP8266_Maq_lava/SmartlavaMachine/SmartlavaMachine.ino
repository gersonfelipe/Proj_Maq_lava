
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Arquivo:   NodeMCU_Lamp.ino
//Tipo:      Codigo-fonte para utilizar no NodeMCU atraves da IDE do Arduino
//Autor:     Marco Rabelo para o canal Infortronica Para Zumbis (www.youtube.com/c/InfortronicaParaZumbis)
//Descricao: Curso de ESP8266 - Controlando uma lampada do celular, PC, etc.
//Video:     https://youtu.be/t103WbEr2Kc
////////////////////////////////////////////////////////////////////////////////////////////////////////////


//needed for library
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>         //https://github.com/tzapu/WiFiManager
#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino


//Nome da sua rede Wifi
const char* ssid = "";
//Senha da rede
const char* password = "";
//Criando o servidor web na porta 80
WiFiServer server(80);

//Pino do NodeMCU que estara conectado ao rele
#define pin0  16 //Equivalente ao D0 no NodeMCU
#define pin1  5 //Equivalente ao D1 no NodeMCU
#define pin2  4 //Equivalente ao D2 no NodeMCU
#define pin3  0 //Equivalente ao D3 no NodeMCU
#define pin4  2  //Equivalente ao D4 no NodeMCU
#define pin5  14 //Equivalente ao D5 no NodeMCU
#define pin6  12 //Equivalente ao D6 no NodeMCU
#define pin7  13 //Equivalente ao D7 no NodeMCU
#define pin8  15 //Equivalente ao D8 no NodeMCU
#define pin9  3  //Equivalente ao RX no NodeMCU

IPAddress ip;
int EntradaA0 = A0;
int ValorNivel;
String nivel;
String agitacao;
String enxague;
String centrifug;
String avanca;
String Power;


//Funcao que sera executada apenas ao ligar o ESP8266
void setup() {  
  Serial.begin(115200);
  WiFiManager wifiManager;
  wifiManager.autoConnect("SMART LAVA MAQ");
  Serial.println("Conectado!!");
  ip = WiFi.localIP();  
  //Preparando o pino, que esta lidago ao rele
  pinMode(pin0, OUTPUT);
  pinMode(pin1, OUTPUT);
  pinMode(pin2, OUTPUT);
  pinMode(pin3, OUTPUT);
  pinMode(pin4, OUTPUT);
  pinMode(pin5, OUTPUT);
  pinMode(pin6, OUTPUT);
  pinMode(pin7, OUTPUT);
  pinMode(pin8, OUTPUT);
  pinMode(pin9, OUTPUT); 
  digitalWrite(pin0, LOW);
  digitalWrite(pin1, LOW);
  digitalWrite(pin2, LOW);
  digitalWrite(pin3, LOW);
  digitalWrite(pin4, LOW);
  digitalWrite(pin5, LOW);
  digitalWrite(pin6, LOW);
  digitalWrite(pin7, LOW);
  digitalWrite(pin8, LOW);
  digitalWrite(pin9, LOW);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  //Iniciando o servidor Web
  server.begin();
  
  
}

//Funcao que sera executada indefinidamente enquanto o NodeMCU estiver ligado.
void loop() {
  
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  //Verificando se o servidor recebeu alguma requisicao
  while (!client.available()) {
    delay(1);
  }

  //Obtendo a requisicao vinda do browser
  String req = client.readStringUntil('\r');

  //Sugestao dada por Enrico Orlando
  if (req == "GET /favicon.ico HTTP/1.1") {
    req = client.readStringUntil('\r');
  }

  client.flush();
  logicanivel();

  //Iniciando o buffer que ira conter a pagina HTML que sera enviada para o browser.
  String buf = "";

  buf += "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\n";
  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  buf += "<head> ";
  //bufmetas += "<meta http-equiv=\"refresh\"content=\"2,URL=http://192.168.4.1/\">";
  buf += "<meta charset='UTF-8'> ";
  buf += "<meta http-equiv='cache-control' content='max-age=0' /> ";
  buf += "<meta http-equiv='cache-control' content='no-cache' /> ";
  buf += "<meta http-equiv='expires' content='0' /> ";
  buf += "<meta http-equiv='expires' content='Tue, 01 Jan 1980 1:00:00 GMT' /> ";
  buf += "<meta http-equiv='pragma' content='no-cache' /> ";
  buf += "<title>MAQ</title> ";  
  buf += "</head> ";
  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++  
  buf += "<body> ";
  buf += "<div class='container'> ";
  buf += "<div class='row'> ";
  buf += "<div class='col tweleve'> ";
  buf += "<p align='center'><font size='10'>Controle Maquina de Lavar</font></p> ";
  buf += "</div> ";
  buf += "</div> "; 
  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ 
  buf += "<div class='row'> "; 
  buf += "<p>Nivel - "+nivel+"</p> ";
  buf += "<p>Agitacao - "+agitacao+"</p> ";
  buf += "<p>Enxague - "+enxague+"</p> ";
  buf += "<p>Centrifug - "+centrifug+"</p> ";
  buf += "<p>Avanca - "+avanca+"</p> ";
  buf += "</div> ";
  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ 
  buf += "</div> ";
  buf += "</body> ";
  buf += "</html> ";

  //Enviando para o browser a 'pagina' criada.
  client.print(buf);
  client.flush();


 //Analisando a requisicao recebida para decidir se liga ou desliga a lampada
 // P1,P2,P3,P4,P5 N1,N2,N3,N4 
  //Analisando a requisicao recebida para decidir se liga ou desliga a lampada
  if (req.indexOf("l") != -1) {
    digitalWrite(pin0, !digitalRead(pin0));    
  }
   else if (req.indexOf("i") != -1) {
    digitalWrite(pin0, !digitalRead(pin0));
  } 
  //+++++++++++++++++++++++++++++Testes Nivel+++++++++++
  else if (req.indexOf("N1") != -1) {
    digitalWrite(pin1, !digitalRead(pin1));
  }
   else if (req.indexOf("N2") != -1) {
    digitalWrite(pin2, !digitalRead(pin2));
  }
  else if (req.indexOf("N3") != -1) {
    digitalWrite(pin3, !digitalRead(pin3));
  }
   else if (req.indexOf("N4") != -1) {
    digitalWrite(pin4, !digitalRead(pin4));
  }
  //++++++++++++++++++++++++++++++++++++++++++++++++  
  //+++++++++++++++++++++++++++++Testes Programas+++++++++++
  else if (req.indexOf("P1") != -1) {
    digitalWrite(pin1, !digitalRead(pin1));
  }
   else if (req.indexOf("P2") != -1) {
    digitalWrite(pin2, !digitalRead(pin2));
  }
  else if (req.indexOf("P3") != -1) {
    digitalWrite(pin3, !digitalRead(pin3));
  }
   else if (req.indexOf("P4") != -1) {
    digitalWrite(pin4, !digitalRead(pin4));
  }
   else if (req.indexOf("P5") != -1) {
    digitalWrite(pin5, !digitalRead(pin5));
  }
  //++++++++++++++++++++++++++++++++++++++++++++++++  
  //+++++++++++++++++++++++Testes Avançar+++++++++++
  else if (req.indexOf("V1") != -1) {
    digitalWrite(pin5, !digitalRead(pin5));
  }
   else if (req.indexOf("V2") != -1) {
    digitalWrite(pin6, !digitalRead(pin6));
  }
  else if (req.indexOf("V3") != -1) {
    digitalWrite(pin7, !digitalRead(pin7));
  }
   else if (req.indexOf("V4") != -1) {
    digitalWrite(pin8, !digitalRead(pin8));
  }
  //++++++++++++++++++++++++++++++++++++++++++++++++ 
  //++++++++++++++++++++Agitação++++++++++++++++++++
  else if (req.indexOf("A1") != -1) {
    digitalWrite(pin1, !digitalRead(pin1));
  }
  else if (req.indexOf("A2") != -1) {
    digitalWrite(pin2, !digitalRead(pin2));
  } 
  //++++++++++++++++++++++++++++++++++++++++++++++++ 
  //++++++++++++++++++++Enxague+++++++++++++++++++++
  else if (req.indexOf("E1") != -1) {
    digitalWrite(pin3, !digitalRead(pin3));
  }
  else if (req.indexOf("E2") != -1) {
    digitalWrite(pin4, !digitalRead(pin4));
  } 
  else if (req.indexOf("E3") != -1) {
    digitalWrite(pin5, !digitalRead(pin5));
  } 
  //++++++++++++++++++++++++++++++++++++++++++++++++ 
  //++++++++++++++++++++Agitação++++++++++++++++++++
  else if (req.indexOf("C1") != -1) {
    digitalWrite(pin1, !digitalRead(pin1));
  }
  else if (req.indexOf("C2") != -1) {
    digitalWrite(pin2, !digitalRead(pin2));
  } 
  //++++++++++++++++++++++++++++++++++++++++++++++++ 
   
  else
  {
    //Requisicao invalida!
    client.stop();
  }


 /////////////////testes//////////////////////////
if(digitalRead(pin1) == true){
     agitacao = "turbo";   
  
     enxague = "tripo";
    
     centrifug = "extra";
    
     avanca = "molho";
    
}else if(digitalRead(pin2) == true){
    
     agitacao = "normal";
   
     enxague = "duplo";
     
     centrifug = "normal";
     
     avanca = "lavar";
     
}else if(digitalRead(pin3) == true){
     
     enxague = "unico";
     
     avanca = "enxaguar";
     
}else if(digitalRead(pin4) == true){
     
     avanca = "centrifugar";
     
}else{
   
     agitacao = "";
     
     enxague = "";
     
     centrifug = "";
     
     avanca = "";
 }
}//Fim do loop

void logicanivel() {
    ValorNivel = analogRead(EntradaA0);// saida analogica de A/D 0 a 1024
    Serial.println(ValorNivel);
    
    if(ValorNivel >= 786 && ValorNivel < 1024){
      nivel = "Alto";
    }
    if(ValorNivel >= 512 && ValorNivel < 786){
      nivel = "Medio";
    }
    if(ValorNivel >= 100 && ValorNivel < 512){
      nivel = "Baixo";
    }
    if(ValorNivel < 100){
      nivel = "extrabaixo";
    }
    
    
  
}


