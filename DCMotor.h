class DCMotor{
private:
  const uint8_t* PIN;
  bool rotate, isClockwise;
  
  void writePIN(){
    digitalWrite(PIN[0], isClockwise && rotate ? LOW : HIGH);
    digitalWrite(PIN[1], !isClockwise && rotate ? LOW : HIGH);
  }

public:
  DCMotor(const uint8_t* PIN_, bool rot = false, bool clockwise = false) {
    PIN = PIN_;
    rotate = rot;
    isClockwise = clockwise;
    writePIN();
  }

  void setRotate(bool rot){
    rotate = rot;
    writePIN();
  }

  void setClockwise(bool clockwise) {
    isClockwise = clockwise;
    writePIN();
  }
};