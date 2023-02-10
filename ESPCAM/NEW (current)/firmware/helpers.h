//So adding an l will add a println
#ifdef ENES100_HELPERS
#pragma message "You are using the ENES100 version of helpers.h"
#define put(x) {Enes100.print(F(#x ": ")); Enes100.print(x);}
#define putn(x, n) {Enes100.print(F(#x ": ")); Enes100.print(x, n);}
#define putl(x) {Enes100.print(F(#x ": ")); Enes100.println(x);}
#define putnl(x, n) {Enes100.print(F(#x ": ")); Enes100.println(x, n);}
#define ps(x) Enes100.print(F(x));
#define psl(x) Enes100.println(F(x));
#define p(x) Enes100.print(x);
#define pl(x) Enes100.println(x);
#else
#define put(x) {Serial.print(F(#x ": ")); Serial.print(x);}
#define putn(x, n) {Serial.print(F(#x ": ")); Serial.print(x, n);}
#define putl(x) {Serial.print(F(#x ": ")); Serial.println(x);}
#define putnl(x, n) {Serial.print(F(#x ": ")); Serial.println(x, n);}
#define ps(x) Serial.print(F(x));
#define psl(x) Serial.println(F(x));
#define p(x) Serial.print(x);
#define pl(x) Serial.println(x);
#define plot(x) {Serial.print(F(#x ":")); Serial.print(x); Serial.print('\t');}
#endif

#define putArrLong(arr, n) { for(int i = 0; i < n; i++) {ps(#arr "["); p(i); ps("] = "); pl(arr[i]);} };
#define putArrLongl(arr, n) { for(int i = 0; i < n; i++) {ps(#arr "["); p(i); ps("] = "); pl(arr[i]);} };

#define putArr(arr, n) { ps(#arr "[] =  {"); for(int i = 0; i < n; i++) {p(arr[i]); if(i!=n-1) ps(", ");} ps("};")};
#define putArrl(arr, n) { ps(#arr "[] =  {"); for(int i = 0; i < n; i++) {p(arr[i]); if(i!=n-1) ps(", ");} psl("};")};


#define prgb(c) {Serial.print("{"); Serial.print(c.r); Serial.print(","); Serial.print(c.g); Serial.print(","); Serial.print(c.b); Serial.print("}");};
#define prgbl(c) {Serial.print("{"); Serial.print(c.r); Serial.print(","); Serial.print(c.g); Serial.print(","); Serial.print(c.b); Serial.println("}");};
#define waitForKey(during) {while (!Serial.available()) {during}; while (Serial.available()) Serial.read();}
#define every(n, during) {static int INTERNAL_COUNTER = 0; INTERNAL_COUNTER++; if(INTERNAL_COUNTER == n) {during; INTERNAL_COUNTER=0;}};
#define exit() {while(true);}

float mapf(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void(* reset) (void) = 0;