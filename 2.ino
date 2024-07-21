#include <Servo.h>

const int numSensors = 4; // 센서의 수
const int sensorPins[numSensors] = {A0, A1, A2, A3}; // 압력 센서가 연결된 아날로그 핀
const int servoPins[numSensors] = {9, 10, 11, 12};  // 서보 모터가 연결된 디지털 핀

const int pressureThreshold = 100; // 압력 센서의 값을 기준으로 하는 임계값 (0~1023)
const unsigned long pressureDuration = 500; // 0.5초 (500 밀리초)

unsigned long pressureStartTimes[numSensors] = {0, 0, 0, 0}; // 각 센서에 대한 압력이 시작된 시간 기록
Servo servos[numSensors];  // 서보 객체 배열 생성

// 압력 센서 값의 평균을 계산하는 함수
int getAverageSensorValue(int pin) {
  const int numReadings = 10; // 평균을 계산할 읽기 수
  long sum = 0;
  for (int i = 0; i < numReadings; i++) {
    sum += analogRead(pin);
    delay(10); // 읽기 사이의 지연
  }
  return sum / numReadings;
}

void setup() {
  Serial.begin(9600);
  for (int i = 0; i < numSensors; i++) {
    servos[i].attach(servoPins[i]); // 각 서보 모터 핀 설정
    servos[i].write(0); // 서보 모터를 0도 위치로 초기화
  }
}

void loop() {
  for (int i = 0; i < numSensors; i++) {
    int sensorValue = getAverageSensorValue(sensorPins[i]); // 평균 압력 센서 값 읽기
    Serial.print("Sensor ");
    Serial.print(i);
    Serial.print(" Average Value: ");
    Serial.println(sensorValue);

    if (sensorValue > pressureThreshold) { // 압력 감지
      if (pressureStartTimes[i] == 0) {
        pressureStartTimes[i] = millis(); // 압력이 처음 감지되었을 때 시간 기록
      } else if (millis() - pressureStartTimes[i] >= pressureDuration) {
        servos[i].write(90); // 압력이 0.5초 이상 지속되면 해당 서보 모터를 90도로 움직임
        delay(10); // 서보 모터의 동작 유지 시간
        servos[i].write(0); // 서보 모터를 0도로 되돌림
        pressureStartTimes[i] = 0; // 타이머 초기화
      }
    } else {
      pressureStartTimes[i] = 0; // 압력이 감지되지 않으면 타이머 초기화
    }
  }

  delay(10); // 반복 간격
}
