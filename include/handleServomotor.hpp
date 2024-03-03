
int direccionEEPROM = 10;
int ang = 0;

Servo myservo;
Servo SpulgarA;
Servo SpulgarB;

int SPosicion = 0;

void initServos()
{
    myservo.attach(12);

    SpulgarA.attach(13);
    SpulgarB.attach(14);

    EEPROM.begin(512);
    ang = EEPROM.read(direccionEEPROM);

    Serial.print("Valor leído de la EEPROM: ");
    Serial.println(ang);
}

void movementServo(Servo &servo, int SAngulo)
{

    if (SAngulo > ang)
    {
        for (ang; ang < SAngulo; ang++)
        {
            servo.write(ang);
            delay(8);
        }
    }
    else
    {

        for (ang; ang > SAngulo; ang--)
        {
            servo.write(ang);
            delay(8);
        }
    }

    ang = SAngulo;
    EEPROM.write(direccionEEPROM, ang); // Almacena el valor en la dirección dada
    EEPROM.commit();
}

void selectServo(const char *Dedo, int SAngulo)
{
    Serial.print("Llegooooo :) ");

    SAngulo = constrain(SAngulo, 0, 180);

    if (strcmp(Dedo, "Indice") == 0)
    {

        movementServo(myservo, SAngulo);
    }
}