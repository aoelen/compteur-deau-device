/*
# This sample codes is for testing the ORP meter V1.0.
 # Editor : YouYou
 # Date   : 2013.11.26
 # Product: ORP meter
 # SKU    : SEN0165
*/
#define VOLTAGE 5.00    //system voltage
#define OFFSET 13        //zero drift voltage
//#define LED LED_BUILTIN         //operating instructions

double orpScriptValue;

#define ArrayLenthOrp  40    //times of collection

int orpArray[ArrayLenthOrp];
int orpArrayIndex=0;

double avergearrayOrp(int* arr, int number){
  int i;
  int max,min;
  double avg;
  long amount=0;
  if(number<=0){
    printf("Error number for the array to avraging!/n");
    return 0;
  }
  if(number<5){   //less than 5, calculated directly statistics
    for(i=0;i<number;i++){
      amount+=arr[i];
    }
    avg = amount/number;
    return avg;
  }else{
    if(arr[0]<arr[1]){
      min = arr[0];max=arr[1];
    }
    else{
      min=arr[1];max=arr[0];
    }
    for(i=2;i<number;i++){
      if(arr[i]<min){
        amount+=min;        //arr<min
        min=arr[i];
      }else {
        if(arr[i]>max){
          amount+=max;    //arr>max
          max=arr[i];
        }else{
          amount+=arr[i]; //min<=arr<=max
        }
      }//if
    }//for
    avg = (double)amount/(number-2);
  }//if
  return avg;
}

int loopOrp(void) {
  static unsigned long orpTimer=millis();   //analog sampling interval
  static unsigned long printTime=millis();
  if(millis() >= orpTimer)
  {
    orpTimer=millis()+20;
    orpArray[orpArrayIndex++]=analogRead(ORP_PIN);    //read an analog value every 20ms
    if (orpArrayIndex==ArrayLenthOrp) {
      orpArrayIndex=0;
    }   
    orpScriptValue=((30*(double)VOLTAGE*1000)-(75*avergearrayOrp(orpArray, ArrayLenthOrp)*VOLTAGE*1000/1024))/75-OFFSET;  

    //convert the analog value to orp according the circuit
  }
  if(millis() >= printTime)   //Every 800 milliseconds, print a numerical, convert the state of the LED indicator
  {
  printTime=millis()+800;
  }
  return (int)orpScriptValue;
}
