# Smart Parking System Test Results

| Test ID | Test Case | Expected Result | Actual Result | Status |
|---|---|---|---|---|
| TC01 | All slots free | 4 available |  | |
| TC02 | Slot 1 occupied | 3 available |  | |
| TC03 | Two slots occupied | 2 available |  | |
| TC04 | Three slots occupied | 1 available |  | |
| TC05 | All slots occupied | Parking Full |  | |
| TC06 | Vehicle leaves | Available count increases |  | |
| TC07 | LCD update | Correct status shown |  | |
| TC08 | Green LED | ON when space available |  | |
| TC09 | Red LED | ON when full |  | |
| TC10 | Buzzer | ON when full |  | |
| TC11 | Servo | Gate opens when allowed |  | |
| TC12 | Reset | System returns to initial state |  | |