
import processing.serial.*;
Serial puerto;
String data ="";
int rquad=40; //Radio del quadrado

void setup()
{
   size(800,600);
   println(Serial.list());
   puerto = new Serial(this,Serial.list()[0],9600);
   puerto .bufferUntil('\n');
  background(255,255,255);//Fondo de color blanco
}
void draw()
{
  background(255,255,255);//Fondo de color blanco
  /**
  *  RECTANGULO TEMPERATURA C
  */

  fill(#00FFBF);
  rect(0,0,400,300);
   /**
  *  RECTANGULO TEMPERATURA TENSCION Uv
  */
  
 
  fill(#B40431);
  rect(400,0,400,300);
   /**
  *  RECTANGULO TEMPERATURA PRESION MB
  */
     
  fill(#40FF00);
  rect(0,300,400,300);
  
   /**
  *  RECTANGULO TEMPERATURA PORCENTAJE DE HUMERAD
  */

  fill(#FF8000);
  rect(400,300,400,300);
  
  fill(#FFFFFF); // COLOR BLANCO A LAS LETRAS
  textSize(50);
  String[] datos = new String[4];
  datos = data.split(",");
  //String(dht.temperature_C),String(tension),String(presion),String(dht.humidity)
  if( datos.length > 1 ) {
  text("Temperatura  C" , 10 , 40);
  text(datos[0],50,100);
  
  text("UV mV" , 440 , 40);
  text(datos[1],490,100);
  
  text("Presion mb" , 10 , 360);
  text(datos[2],50,500);
  
  text("Humedad %" , 440 , 360);
  text(datos[3],490,500);
  }
}

void serialEvent(Serial puerto)
{
  
  String dato = puerto.readStringUntil('\n'); 
  if(dato != null)
  {
    println(dato);
     data = dato;
  }
}
