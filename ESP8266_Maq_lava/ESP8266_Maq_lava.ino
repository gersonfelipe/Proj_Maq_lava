////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Arquivo:   ESP8266_Maq_lava.ino
//Tipo:      Controle de maquina de lavar brastemp
//Autor:     Gerson felipe, Welington bonfim
//Descricao: App para controle de maquina de lava com interface web.
//Video:     
////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Blibiotecas
#include <EEPROM.h>
#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>         //https://github.com/tzapu/WiFiManager


//Nome da sua rede Wifi
const char* ssid = "G_AUTOMATION";

//Senha da rede
const char* password = "gerson2008";

//IP do ESP (para voce acessar pelo browser)
//IPAddress ip(192,168,0,8);

//IP do roteador da sua rede wifi
//IPAddress gateway(192,168,0,1);

//Mascara de rede da sua rede wifi
//IPAddress subnet(255, 255, 255, 0);

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
int EntradaA0 = A0;
String Temp;


IPAddress ip;
String pegaip;



//Funcao que sera executada apenas ao ligar o ESP8266
void setup() {  
  Serial.begin(115200);
  WiFiManager wifiManager;
  wifiManager.autoConnect("FlexHome");
  Serial.println("Conectado!!");
  //ESP.reset();
  //pegaip = "10.83.110.84";
  //pegaip = WiFi.localIP;  
  ip = WiFi.localIP();
  pegaip = ip.toString();// transforma uma variavel tipo(IPAddress)para string para poder envia para brause
  
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
  
  

  //Conectando a rede Wifi
  //WiFi.config(ip, gateway, subnet);
  //WiFi.begin(ssid, password);
  //Verificando se esta conectado,
  //caso contrario, espera um pouco e verifica de novo.
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  //Iniciando o servidor Web
  server.begin();
  
  
}

//Funcao que sera executada indefinidamente enquanto o NodeMCU estiver ligado.
void loop() {
  //delay (5000);
  //ESP.reset();
  //delay (5000);
  //Verificando se o servidor esta pronto.
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
  temperatura();

  //Iniciando o buffer que ira conter a pagina HTML que sera enviada para o browser.
  String bufmetas = "";
  String buf = "";

  bufmetas += "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\n";
  bufmetas += "<head> ";
  bufmetas += "<meta http-equiv=\"refresh\"content=\"2,URL=http://"+pegaip+"/\">";
  bufmetas += "<meta charset='UTF-8'> ";
  bufmetas += "<meta http-equiv='cache-control' content='max-age=0' /> ";
  bufmetas += "<meta http-equiv='cache-control' content='no-cache' /> ";
  bufmetas += "<meta http-equiv='expires' content='0' /> ";
  bufmetas += "<meta http-equiv='expires' content='Tue, 01 Jan 1980 1:00:00 GMT' /> ";
  bufmetas += "<meta http-equiv='pragma' content='no-cache' /> ";
  bufmetas += "<title>G_AUTOMATION</title> ";
  bufmetas += "<style> ";
  bufmetas += "body{font-family:Open Sans; color:#555555;} ";
  bufmetas += "h1{font-size:24px; font-weight:normal; margin:0.4em 0;} ";
  bufmetas += ".container { width: 100%; margin: 0 auto; } ";
  bufmetas += ".container .row { float: left; clear: both; width: 100%; } ";
  bufmetas += ".container .col { float: left; margin: 0 0 1.2em; padding-right: 1.2em; padding-left: 1.2em; } ";
  bufmetas += ".container .col.four, .container .col.twelve { width: 100%; } ";
  bufmetas += "@media screen and (min-width: 767px) { ";
  bufmetas += ".container{width: 100%; max-width: 1080px; margin: 0 auto;} ";
  bufmetas += ".container .row{width:100%; float:left; clear:both;} ";
  bufmetas += ".container .col{float: left; margin: 0 0 1em; padding-right: .5em; padding-left: .5em;} ";
  bufmetas += ".container .col.four { width: 50%; } ";
  bufmetas += ".container .col.tweleve { width: 100%; } ";
  buf += "} ";
  buf += "* {-moz-box-sizing: border-box; -webkit-box-sizing: border-box; box-sizing: border-box;} ";
  buf += "a{text-decoration:none;} ";
  buf += ".btn {font-size: 20px; white-space:nowrap; width:100%; padding:.8em 1.5em; font-family: Open Sans, Helvetica,Arial,sans-serif; ";
  buf += "line-height:18px; display: inline-block;zoom: 1; color: #fff; text-align: center; position:relative; ";
  buf += "-webkit-transition: border .25s linear, color .25s linear, background-color .25s linear; ";
  buf += "transition: border .25s linear, color .25s linear, background-color .25s linear;} ";
  buf += ".btn.btn-sea{background-color: #08bc9a; border-color: #08bc9a; -webkit-box-shadow: 0 3px 0 #088d74; box-shadow: 0 3px 0 #088d74;} ";
  buf += ".btn.btn-sea:hover{background-color:#01a183;} ";
  buf += ".btn.btn-sea:active{ top: 3px; outline: none; -webkit-box-shadow: none; box-shadow: none;} ";
  buf += "</style> ";
  buf += "</head> ";
  
  buf += "<body> ";
  buf += "<div class='container'> ";
  buf += "<div class='row'> ";
  buf += "<div class='col tweleve'> ";
  buf += "<p align='center'><font size='10'>Controle Relés</font></p> ";
  buf += "</div> ";
  buf += "</div> ";  
  String stat;
  buf += "<div class='row'> ";
  //Botão 1
  if(digitalRead(pin0)){stat="ON";}else{stat="OFF";}
  buf += "<div class='col four'> ";
  buf += "<a href='?f=R0' class='btn btn-sea'>Relé 1 "+stat+"</a> ";
  buf += "</div> ";
  //================
  //Botão 2
  if(digitalRead(pin1)){stat="ON";}else{stat="OFF";}
  buf += "<div class='col four'> ";
  buf += "<a href='?f=R1' class='btn btn-sea'>Relé 2 "+stat+"</a> ";
  buf += "</div> ";
  //================
  //Botão 3
  if(digitalRead(pin2)){stat="ON";}else{stat="OFF";}
  buf += "<div class='col four'> ";
  buf += "<a href='?f=R2' class='btn btn-sea'>Relé 3 "+stat+"</a> ";
  buf += "</div> ";
  //================
  //Botão 4
  if(digitalRead(pin3)){stat="ON";}else{stat="OFF";}
  buf += "<div class='col four'> ";
  buf += "<a href='?f=R3' class='btn btn-sea'>Relé 4 "+stat+"</a> ";
  buf += "</div> ";
  //================
  //Botão 5
  if(digitalRead(pin4)){stat="ON";}else{stat="OFF";}
  buf += "<div class='col four'> ";
  buf += "<a href='?f=R4' class='btn btn-sea'>Relé 5 "+stat+"</a> ";
  buf += "</div> ";
  //================
  //Botão 6
  if(digitalRead(pin5)){stat="ON";}else{stat="OFF";}
  buf += "<div class='col four'> ";
  buf += "<a href='?f=R5' class='btn btn-sea'>Relé 6 "+stat+"</a> ";
  buf += "</div> ";
  //================
  //Botão 7
  if(digitalRead(pin6)){stat="ON";}else{stat="OFF";}
  buf += "<div class='col four'> ";
  buf += "<a href='?f=R6' class='btn btn-sea'>Relé 7 "+stat+"</a> ";
  buf += "</div> ";
  //================
  //Botão 8
  if(digitalRead(pin7)){stat="ON";}else{stat="OFF";}
  buf += "<div class='col four'> ";
  buf += "<a href='?f=R7' class='btn btn-sea'>Relé 8 "+stat+"</a> ";
  buf += "</div> ";
  //================
  //Botão 9
  if(digitalRead(pin8)){stat="ON";}else{stat="OFF";}
  buf += "<div class='col four'> ";
  buf += "<a href='?f=R8' class='btn btn-sea'>Relé 9 "+stat+"</a> ";
  buf += "</div> ";
  //================
  //Botão 10
  if(digitalRead(pin9)){stat="ON";}else{stat="OFF";}
  buf += "<div class='col four'> ";
  buf += "<a href='?f=R9' class='btn btn-sea'>Relé 10 "+stat+"</a> ";
  buf += "</div> ";
  //================  
  buf += "</div> ";  
  buf += "<div class='col tweleve'> ";
  buf += "<p align='center'><font size='5'>G_Automation Solução em Automações Residencias</font></p> ";
  buf += "</div> ";
  
  buf += "</div> ";
  
  buf += "</div> ";  
  buf += "</br> "; 

  //buf += "<div class='col tweleve'> ";
  buf += "<p align='center'><font size='120'>"+Temp+"°C</font></p> ";
  //buf += "</div> "; 

  buf += "</body> ";
  buf += "</html> ";

  //Enviando para o browser a 'pagina' criada.
  client.print(bufmetas);
  client.print(buf);
  client.flush();


 //Analisando a requisicao recebida para decidir se liga ou desliga a lampada
  if (req.indexOf("R0") != -1) {
    digitalWrite(pin0, !digitalRead(pin0));
  }
  else if (req.indexOf("R1") != -1) {
    digitalWrite(pin1, !digitalRead(pin1));
  }
   else if (req.indexOf("R2") != -1) {
    digitalWrite(pin2, !digitalRead(pin2));
  }
  else if (req.indexOf("R3") != -1) {
    digitalWrite(pin3, !digitalRead(pin3));
  }
   else if (req.indexOf("R4") != -1) {
    digitalWrite(pin4, !digitalRead(pin4));
  }
   else if (req.indexOf("R5") != -1) {
    digitalWrite(pin5, !digitalRead(pin5));
  }
  else if (req.indexOf("R6") != -1) {
    digitalWrite(pin6, !digitalRead(pin6));
  }
  else if (req.indexOf("R7") != -1) {
    digitalWrite(pin7, !digitalRead(pin7));
  }
  else if (req.indexOf("R8") != -1) {
    digitalWrite(pin8, !digitalRead(pin8));
  }
  else if (req.indexOf("R9") != -1) {
    digitalWrite(pin9, !digitalRead(pin9));
  }
  else
  {
    //Requisicao invalida!
    client.stop();
  }

}
void temperatura() {
    Temp = analogRead(EntradaA0);
    //temperatura = (valorLido * 0.00488);
    //temperatura = temperatura * 100;
   // Serial.print("Temperatura actual: ");
    //Serial.println(temperatura);
    //delay(1000);
}


