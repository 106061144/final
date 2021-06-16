#include "mbed.h"
#include "bbcar.h"
#include "mbed_rpc.h"

Ticker servo_ticker;
PwmOut pin5(D5), pin6(D6);
BufferedSerial xbee(D1, D0);
BBCar car(pin5, pin6, servo_ticker);
BufferedSerial pc(USBTX,USBRX); //tx,rx
BufferedSerial uart(A1,A0); //tx,rx
DigitalInOut ping(D11);
DigitalInOut pin10(D10);
DigitalOut myled(LED1);

Timer t;
Thread car1_t;
Thread car2_t;
Thread car3_t;
Thread car4_t;
Thread ping_t;
float angle=0.0;
float dis=0.0;

void PARTControl(Arguments *in, Reply *out);
RPCFunction rpcPART(&PARTControl, "PART");
int x, y, z, k;
int p1=36;
int p2=50;
int dir=0;
int step=0;

void ping_thread(){
    float val;
    ping.output();
    ping = 0; wait_us(200);
    ping = 1; wait_us(5);
    ping = 0; wait_us(5);

    ping.input();
    while(ping.read() == 0);
    t.start();
    while(ping.read() == 1);
    val = t.read();
    dis = val*17700.4f;
    t.stop();
    t.reset();
}


EventQueue queue(32 * EVENTS_EVENT_SIZE);
EventQueue event_queue(32 * EVENTS_EVENT_SIZE);

int Count=0;
int flag1=0;
void car_thread_1(){

    if(flag1==1){
        car.stop();
        ThisThread::sleep_for(9999);
        return;
    }
    
    if(dis<25){
        Count++;
        if(Count==20){
            Count=0;
            flag1=1;
            return;
        }
        car.stop();
        ThisThread::sleep_for(200ms);
        return;
    }else{
        Count=0;
    }
    if(angle>10.0){
        car.turn(80,-0.3);
        ThisThread::sleep_for(100ms);
    }else if(angle<(-10.0)){
        car.turn(80,0.3);
        ThisThread::sleep_for(100ms);
    }else {
        car.goStraight(100);
        ThisThread::sleep_for(100ms);
    }
    car.stop();
}

int flag2=0;
void car_thread_2(){// used for position calibration
    
    if(flag2){
        car.stop();
        ThisThread::sleep_for(9999);
        return;
    }

    if(dis<p1){
        car.goStraight(-150);
        ThisThread::sleep_for(100ms);
        car.stop();
        ThisThread::sleep_for(100ms);
        return;
    }

    if(step==1){
        if(dir==1){
            step=0;
            car.turn(100,-0.4);
            ThisThread::sleep_for(200ms);
            return;
        }else if(dir==2){
            step=0;
            car.turn(100,0.35);
            ThisThread::sleep_for(200ms);
            return;
        }
    }else if(step==0){
        if(dis>p2){
            car.goStraight(60);
            ThisThread::sleep_for(100ms);
            return;
        }
        if((angle<357.0) && (angle>180.0)){ //need to turn right
            dir=1;
            step=1;
            Count=0;
            car.turn(-60,-0.01);
            ThisThread::sleep_for(100ms);
            return;
        }else if((angle>3.0) && (angle<180.0)){ //need to turn left 
            dir=2;
            step=1;
            Count=0;
            car.turn(-60, 0.01);
            ThisThread::sleep_for(100ms);
            return;
        }else {
            Count++;
            if(Count==10){
                //printf("finish\n");
                flag2=1;
                Count=0;
                car.turn(80,-0.01);//turn 90 degree clockwise to
                ThisThread::sleep_for(100ms); //follow another line
                return;   
            }
            car.stop();
            ThisThread::sleep_for(200ms);
            return;
        }   
    }
}

int flag3=0;
void car_thread_3(){
    if(flag3){
        car.stop();
        ThisThread::sleep_for(9999);
        return;
    }
    if(angle>10.0){
        car.turn(50,-0.3);
        ThisThread::sleep_for(100ms);
    }else if(angle<(-10.0)){
        car.turn(50,0.3);
        ThisThread::sleep_for(100ms);
    }else {
        car.goStraight(50);
        ThisThread::sleep_for(100ms);
    }
    car.stop();
}

int flag4=0;
int flag4_1=0;
void car_thread_4(){
    if(flag4){
        flag4_1=1;
        flag4=0;
        car.turn(30,0.01);
        ThisThread::sleep_for(50ms);
        return;
    }
    if(flag4_1==1){
        car.stop();
        ThisThread::sleep_for(9999);
        return;
    }
    
    if(dis<20){
        car.turn(-80,0.01);
        ThisThread::sleep_for(100ms);
        return;
    }else{
        car.turn(40,0.7);
        ThisThread::sleep_for(100ms);
        return;
    }
}

void ping_(){
    event_queue.call_every(100ms,ping_thread);
    event_queue.dispatch();
}
void car_1(){
    queue.call_every(200ms,car_thread_1);
    queue.dispatch();
}

void car_2(){
    queue.call_every(1s,car_thread_2);
    queue.dispatch();
}

void car_3(){
    queue.call_every(200ms,car_thread_3);
    queue.dispatch();
}

void car_4(){
    queue.call_every(700ms,car_thread_4);
    queue.dispatch();
}

int num = 0;
int record=0;
char temp[128];
void line_follow_object(){ 
    myled=0;
   ping_t.start(ping_);
   car1_t.start(car_1);
   while(1){
        if(uart.readable()){
            char recv[1];
            uart.read(recv, sizeof(recv));
            if(record){
                temp[num] = recv[0];
                num++;
            }
            if(recv[0] == 'D'){
                record=1;
            }
            if(recv[0] == '\n'){
                num=0;
                record=0;
                angle = atof(temp);
                printf("angle:%f\n",angle);
            }
            if(flag1){
                myled=1;
                return;
            }
        }
   }
   
}

int record1=0;
void position_calibration(){

   myled=0;
   Count=0;
   ping_t.start(ping_);
   car2_t.start(car_2);
   while(1){
        if(uart.readable()){
            char recv[1];
            uart.read(recv, sizeof(recv));
            pc.write(recv, sizeof(recv));

            if(record1){
                if(recv[0] == 'F'){
                    record1=0;
                    angle = atof(temp);
                    printf("angle:%f ",angle);
                    printf("distance:%f\n",dis);
                }else{
                    temp[num] = recv[0];
                    num++;
                }
            }
            if(recv[0] == 'I'){
                record1=1;
            }
            
            if(recv[0] == '\n'){
                num=0;
                memset(temp, 0, 128);
            }
            if(flag2){
                myled=1;
                //xbee.write("finish part 3")
                return;
            }
        }     
   }
  return;
}

int check=0;
void circle(){
    myled=0;
    num=0;
    ping_t.start(ping_);
    car4_t.start(car_4);
    while(1){
        if(uart.readable()){
            char recv[1];
            uart.read(recv, sizeof(recv));
            pc.write(recv, sizeof(recv));
            //printf("distance : %f\n",dis);
            if(check){
                if(recv[0]==y){ //check ID
                    flag4=1;
                    myled=1;
                    return;
                }
                check=0;
            }
            if(recv[0]=='\n'){
                check=1;
            }
        }     
   }
   return;
}

void line_follow_destination(){
   myled=0;
   ping_t.start(ping_);
   car3_t.start(car_3);
   while(1){
        if(uart.readable()){
            char recv[1];
            uart.read(recv, sizeof(recv));
            pc.write(recv, sizeof(recv));
            if(record){
                temp[num] = recv[0];
                num++;
            }
            if(recv[0] == 'P'){
                flag3=1;
                myled=1;
                return;
            }
            if(recv[0] == 'D'){
                record=1;
            }
            if(recv[0] == '\n'){
                num=0;
                record=0;
                angle = atof(temp);
                printf("Angle:%f\n",angle);
            }
        }
   }
   return;
}

void PARTControl (Arguments *in, Reply *out){
    bool success = true;
    x = in->getArg<int>();
    if(x==1 || x==3){
        y = in->getArg<char>();
    }
    
    printf("This is %d",x);
    
    if(x==0){
        line_follow_object();
    }else if(x==1){
        circle();
    }else if(x==2) {
        position_calibration();
    }else if(x==3) {
        line_follow_destination();
    }
    else return;
}

int main() {
   uart.set_baud(9600);
   pc.set_baud(9600);
   char buf[256], outbuf[256];
   FILE *devin = fdopen(&xbee, "r");
   FILE *devout = fdopen(&xbee, "w");
   while (1) {
      memset(buf, 0, 256);
      for( int i = 0; ; i++ ) {
         char recv = fgetc(devin);
         if(recv == '\n') {
            printf("\r\n");
            break;
         }
         buf[i] = fputc(recv, devout);
      }
      printf("%s",buf);
   RPC::call(buf, outbuf);
   printf("%s\r\n", outbuf);
   }
}