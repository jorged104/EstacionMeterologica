/**
 *  IMPORTACIONES
 */
#include <cactus_io_AM2302.h>
#include <SFE_BMP180.h>
#include <Wire.h>

#define AM2302_PIN 2
SFE_BMP180 pressure;

const bool debug = false;  // define el debug por el puerto serial
String server = "192.168.1.9";
//String server = "192.168.43.249";
int pinUV = A0;
int valorSensor = 0;
String UV_index = "0";
int tension = 0 ;

AM2302 dht(AM2302_PIN);
double presion;

void setup() {
  
  Serial.begin(9600);
  Serial1.begin(115200);
  pinMode(pinUV, INPUT);
  // put your setup code here, to run once:
  delay(1000);
  if (pressure.begin())
  
    Serial.println("BMP180 init success");
  else
  {
   
    Serial.println("BMP180 init fail (disconnected?)\n\n");
    while(1); // Pause forever.
   }
  /*escribir("AT+RST",2000,"OK");
  delay(1000);
  if(escribir("AT+CWMODE=1",1000,"OK"))
   resultado("MODO 1 " );
  delay(500);
  escribir("AT+CWJAP=\"CLARO_65E146\",\"d50DaEeEe0\"",10000,"OK");
  delay(5000);
  while(!escribir("AT+CIPSTATUS",1000,"2"))
  {
    Serial.println(" Reiniciar ...... " );
    delay(2000);
  }
  if(escribir("AT+CIPMUX=1",1000,"OK"))
   resultado("MODO MULTIPLES CONECCIONES " );
   */
}

void loop() {

   sensar();
  // enviarDatos(String(dht.temperature_C),String(tension),String(presion),String(dht.humidity));
   int cont =0 ;  
   while(cont < 60 )  //Delay de un minuto para enviar informacion al servidor
   {
    sensar();
    delay (1000); // retraso de un segundo
    enviarProcessing(String(dht.temperature_C),String(tension),String(presion),String(dht.humidity));  //Enviar informacion a processing 
    cont++;
   }
}
void sensar()
{
   presion = getPressure();
   dht.readHumidity();
   dht.readTemperature();
   UV();
}
void enviarDatos(String temperatura,String uv,String presion,String humedad)
{
  if ( escribir("AT+CIPSTART=4,\"TCP\",\"" + server + "\",5000",3000,"OK") )
  {
    resultado(" CONECTADO AL SERVIDOR ...... ");
    delay(2000);
    String peticionHTTP= "GET /agregar?datos="+temperatura+","+uv+","+presion+","+humedad+" HTTP/1.1";
    String tam = "AT+CIPSEND=4," +  String(peticionHTTP.length());
    resultado(tam);
    bool enviar = escribir(tam,10000,">");
    delay(2000);
    if(enviar )
    {
      resultado("LISTO PARA ENVIAR ......");
      bool peticion = escribir(peticionHTTP,10000,"SEND OK");
      delay(2000);
      if (peticion)
        resultado("INFORMACION ENVIARDA....");
      else
        resultado("INFOMRACION NO ENVIADA...");  
      escribir("AT+CIPCLOSE=4",1000,"OK");   
    }
    else
    {
      resultado("No  se puedo conectar cerrar coneccion ");
      escribir("AT+CIPCLOSE=4",1000,"OK");
    }
    
    
  }
}
bool escribir(String cmd,const int tiempoEspera,String buscar )
{
  String respuesta = "";
   Serial1.println(cmd);
   long int time = millis();
   while( (time+tiempoEspera) > millis())
   {
     while(Serial1.available())
     {
        char c = Serial1.read();
        respuesta += c;
     }
   }
   if(debug)
   {
    Serial.println(respuesta);
   }
   if(respuesta.indexOf(buscar) > 0 )
    return true;
   return false;
}
/**
 * Funcion que muestra en la pantalla serial un mensaje si la variable global de debug es true
 * @param msg : Mensaje a mostrar
 */
void resultado(String msg)
{
  if(debug)
  Serial.println(msg);
}

double getPressure()
{
  char status;
  double T,P,p0,a;
  status = pressure.startTemperature();
  if (status != 0)
  {
    delay(status);
    status = pressure.getTemperature(T);
    if (status != 0)
    {
      status = pressure.startPressure(3);
      if (status != 0)
      {
        delay(status);
        status = pressure.getPressure(P,T);
        if (status != 0)
        {
          return(P);
        }
        else Serial.println("error \n");
      }
      else Serial.println("error \n");
    }
    else Serial.println("error \n");
  }
  else Serial.println("error \n");
}

void UV()
{
  valorSensor = analogRead(pinUV);
   tension = (valorSensor * (5.0 / 1023.0)) * 1000;
  //Compara com valores tabela UV_Index
  if (tension > 0 && tension < 50)
  {
    UV_index = "0";
  }
  else if (tension > 50 && tension <= 227)
  {
    UV_index = "0";
  }
  else if (tension > 227 && tension <= 318)
  {
    UV_index = "1";
  }
  else if (tension > 318 && tension <= 408)
  {
    UV_index = "2";
  }
  else if (tension > 408 && tension <= 503)
  {
    UV_index = "3";
  }
  else if (tension > 503 && tension <= 606)
  {
    UV_index = "4";
  }
  else if (tension > 606 && tension <= 696)
  {
    UV_index = "5";
  }
  else if (tension > 696 && tension <= 795)
  {
    UV_index = "6";
  }
  else if (tension > 795 && tension <= 881)
  {
    UV_index = "7";
  }
  else if (tension > 881 && tension <= 976)
  {
    UV_index = "8";
  }
  else if (tension > 976 && tension <= 1079)
  {
    UV_index = "9";
  }
  else if (tension > 1079 && tension <= 1170)
  {
    UV_index = "10";
  }
  else if (tension > 1170)
  {
    UV_index = "11";
  }
}

void enviarProcessing(String temperatura,String uv,String presion,String humedad)
{
  String data = temperatura;
  data += "," + uv;
  data += "," + presion;
  data += "," + humedad;
  Serial.println(data);
}

