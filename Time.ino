void updateTime() {
  time_t now = time(nullptr);
  if (now != -1) {
    strftime(dateString, sizeof(dateString), "%d%m%Y", localtime(&now));
    strftime(timeString, sizeof(timeString), " %H%M", localtime(&now));
  }
  Serial.println(dateString);
  Serial.println(timeString);
  delay(500);
  display9.println(String(dateString));
  display10.println(String(timeString));
}
