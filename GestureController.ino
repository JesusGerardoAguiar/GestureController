
const int trigPin1 = 11; // the number of the trigger output pin ( sensor 1 )
const int echoPin1 = 10; // the number of the echo input pin ( sensor 1 )     
const int trigPin2 = 6;  // the number of the trigger output pin ( sensor 2 ) 
const int echoPin2 = 5;  // the number of the echo input pin ( sensor 2 ) 

////////////////////////////////// variables used for distance calculation 
long duration;                               
int distance1, distance2; 
float r;
unsigned long temp=0;
int temp1=0;
int l=0;
////////////////////////////////

void find_distance (void);
//esta funcion retorna el valor en cm
  

void find_distance (void)                   
{ 
  digitalWrite(trigPin1, LOW);
  delayMicroseconds(2); 
  digitalWrite(trigPin1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin1, LOW);

  duration = pulseIn(echoPin1, HIGH, 5000);// aquí esta función de pulso no esperará más de 5000us para que vuelva el sonido ultrasónico. (debido a esto no medirá más de 60 cm)
                                           // ayuda a este proyecto a usar el control de gestos en el espacio definido devolvera cero si la distancia es superior a 60 m.(uitamos las manos delante de los sensores).                                           
  r = 3.4 * duration / 2;                  // Calculo para obtener la distancia en cm usando el tiempo retornado anteriormente.     
  distance1 = r / 100.00;
  /////////////////////////////////////////Lo de arriba es para el sensor de la izquierda y lo de abajo pa la derecha.
  digitalWrite(trigPin2, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin2, LOW);

  duration = pulseIn(echoPin2, HIGH, 5000);
  r = 3.4 * duration / 2;     
  distance2 = r / 100.00;
  delay(100);
}

void setup() 
{
  Serial.begin(9600);
  pinMode(trigPin1, OUTPUT); 
  pinMode(echoPin1, INPUT) ;
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);
  delay (1000);
    
}

void loop()
{
  find_distance(); // esta función almacenará la distancia actual medida por el sensor ultrasónico en la variable global "distancia 1 y distancia 2"
                  
  
  if(distance1<=35 && distance1>=15) // una vez q colocamos nuestras manos frente al sensor derecho en el rango entre 15 y 35 cm, esta condición se convierte en verdadera.
  { 
    temp=millis();                   //almacena la hora actual en la variable temp ("millis" Devuelve la cantidad de milisegundos desde que Arduino comenzó a ejecutar el programa actual)
    while(millis()<=(temp+300))      // este loop mide la distancia por otros 300 milisegundos. (Ayuda a encontrar la diferencia entre deslizar y mantener nuestra mano frente al sensor derecho)
    find_distance();
    if(distance1<=35 && distance1>=15)// esta condición será verdadera si colocamos nuestra mano en frente del sensor derecho durante más de 300 milisegundos.
    {
     temp=distance1;                         // almacena la posición actual de tu mano en temp.
     while(distance1<=50 || distance1==0)    // este loop se ejecutará hasta que quites la mano del sensorR.
     {
       find_distance();                      // llama a esta función continuamente para obtener los datos en vivo.
       if((temp+6)<distance1)               // esta condición se vuelve verdadera si alejamos la mano del sensor derecho (** pero delante de él). aquí "temp + 6" es para calibración.
       {
       Serial.println("down");               // manda el serial "down"
       }
       else if((temp-6)>distance1)           // esta condición se vuelve verdadera si acercamos la mano del sensor derecho
       {
        Serial.println("up");                // manda el serial "up"
       }
     }
    }
    else                                     // esta condición se vuelve verdadera si pasamos la mano por el sensor derecho 
    {
      Serial.println("next");                // manda el serial "next"
    }
  }

  else if(distance2<=35 && distance2>=15)  // esta condición será verdadera si colocamos nuestra mano en frente del sensor izquier durante más de 300 milisegundos.
  { 
  
    temp=millis();                           
  
    while(millis()<=(temp+300))             
    {
       find_distance();
       if(distance1<=35 && distance1>=15)  
       {
         Serial.println("change");         // manda el serial "change"
         l=1;                              // almacena 1 en la variable l. (Evita que el programa entre en la próxima condición if)
         break;                           
       }
    }
    
    if(l==0)                               // esta condición sera true, solo si pasamos nuestra mano en frente del sensor izquierdo.
    {
    Serial.println("previous");            // manda serial "previous".
    while(distance2<=35 && distance2>=15) 
    find_distance();                      
    }
    l=0;                                  // hace l = 0 para la siguiente iteracion.
   }
   
}
