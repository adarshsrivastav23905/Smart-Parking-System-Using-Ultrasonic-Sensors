# Smart Parking System Test Results

| Test ID | Test Case | Expected Result | Actual Result | Status |
|---|---|---|---|---|
| TC01 | All slots free | 4 available | 4 available | Pass |
| TC02 | Slot 1 occupied | 3 available | 3 available | Pass |
| TC03 | Two slots occupied | 2 available | 2 available | Pass |
| TC04 | Three slots occupied | 1 available | 1 available  | Pass |
| TC05 | All slots occupied | Parking Full | Parking full | Pass |
| TC06 | Vehicle leaves | Available count increases | Available count increases | Pass |
| TC07 | LCD update | Correct status shown | Correct status shown | Pass|
| TC08 | Green LED | ON when space available | ON when space available | Pass |
| TC09 | Red LED | ON when full | ON when full | Pass |
| TC10 | Buzzer | ON when full | ON when full | Pass |
| TC11 | Servo | Gate opens when allowed | Gate opens when allowed | Pass |
| TC12 | Reset | System returns to initial state | system returns to initial state | Pass |
