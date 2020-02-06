#include "parameters.h"

/*
  Mode and zones
  mode 1: Start→A
  mode 2: A→B(RemoteControl)
  mode 3: B→C
  mode 4: C(Before U-turn)
  mode 5: C(After U-turn)
  mode 6: C→A
  mode 7: A→Goal
*/

<<<<<<< HEAD
void setup() {
  pinMode(RF, OUTPUT);
  pinMode(RB, OUTPUT);
  pinMode(LF, OUTPUT);
  pinMode(LB, OUTPUT);
  for (i = 0; i < 8; i++) {
    pinMode(ModeSW[i], INPUT_PULLUP);
  }
  location_info = SWCheck();
  Serial.begin(9600);//PC (Debug)
  Serial1.begin(9600);//ESP (Wireless)
  Serial.print("=================================================================");
  Serial.println(location_info);
=======
void setup()
{
    pinMode(RF, OUTPUT);
    pinMode(RB, OUTPUT);
    pinMode(LF, OUTPUT);
    pinMode(LB, OUTPUT);
    for (i = 0; i < 8; i++)
    {
        pinMode(ModeSW[i], INPUT_PULLUP);
    }
    location_info = SWCheck();
    Serial.begin(9600);  //PC (Debug)
    Serial1.begin(9600); //ESP (Wireless)
    Serial2.begin(9600); //sub Arduino (Timer)
    Serial2.write("1");
    Serial.print("=================================================================");
    Serial.println(location_info);
>>>>>>> 890a12c4843f6aeb0894b9bbf5ba5f5d3806bdea
}

void loop()
{
    // Serial.print("=MODE=");
    // Serial.println(SWCheck());
    int sum_black = 5;
    const bool R1_black = analogRead(R1) < DR1;
    const bool R2_black = analogRead(R2) < DR2;
    const bool CT_black = analogRead(CT) < DCT;
    const bool L1_black = analogRead(L2) < DL2;
    const bool L2_black = analogRead(L1) < DL1;
    sum_black = R1_black ? sum_black : sum_black - 1;
    sum_black = R2_black ? sum_black : sum_black - 1;
    sum_black = CT_black ? sum_black : sum_black - 1;
    sum_black = L2_black ? sum_black : sum_black - 1;
    sum_black = L1_black ? sum_black : sum_black - 1;
    Serial.print(L1_black);
    Serial.print(L2_black);
    Serial.print(CT_black);
    Serial.print(R2_black);
    Serial.print(R1_black);
    Serial.print(" ");
    Serial.print(sum_black);
    Serial.print(" ");
    Serial.println(location_info);

    if (sum_black <= 2)
    {
        flag = true;
        if (CT_black)
        {
            MoveStraight();
        }
        else
        {
            if ((R1_black || R2_black) && (L1_black || L2_black))
            {
                MoveStop(1);
            }
            else
            {
                if (R1_black || R2_black)
                {
                    MoveSpinR();
                }
                else if (L1_black || L2_black)
                {
                    MoveSpinL();
                }
                else
                {
                    MoveStraight();
                }
            }
        }
    }
    else
    {

        Serial.print("==================================================================");
        Serial.println(location_info);
        if (flag)
        {
            switch (location_info)
            {
            // start line
            case 0:
                Serial.print("StartLine passed");
            // point A
            case 1:
                Serial.print("on A");
                MoveStop(3000);
                break;
            //  point B
            case 2:
                //左右に揺れる
                Serial.print("on B");
                MoveSpinL();
                delay(500);
                MoveSpinR();
                delay(1000);
                MoveSpinL();
                delay(1000);
                MoveSpinR();
                delay(1000);
                // 黒になるまで旋回
                while (analogRead(CT) > DCT)
                {
                    MoveL();
                }
                Serial.println("Line Ditected");
                break;
            // point C path
            case 3:
                MoveStop(500);
                MoveL_UntillB();
                break;
            // point C
            case 4:
                MoveStraight();
                delay(500);
                MoveOnC();
                break;
            // point C path again
            case 5:
                MoveStop(500);
                MoveL_UntillB();
                break;
            // point A again
            case 6:
                MoveStop(3000);
                break;
            default:
                break;
            }
            Serial.println("Flag Changed");
            location_info += 1;
            flag = false;
        }
    }
}
