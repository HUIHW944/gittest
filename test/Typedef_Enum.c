#include <stdio.h>
#include "Platform_Types.h"

typedef enum {
    MONDAY,    // 0
    TUESDAY,   // 1
    WEDNESDAY, // 2
    THURSDAY,  // 3
    FRIDAY,    // 4
    SATURDAY,  // 5
    SUNDAY     // 6
}Typedef_Enum_TEweekdayType;  

const char* Typedef_Enum_GetWeekday(Typedef_Enum_TEweekdayType day) {
    switch(day) {
        case MONDAY:    return "Monday";
        case TUESDAY:   return "Tuesday";
        case WEDNESDAY: return "Wednesday";
        case THURSDAY:  return "Thursday";
        case FRIDAY:    return "Friday";
        case SATURDAY:  return "Saturday";
        case SUNDAY:    return "Sunday";
        default:        return "Invalid";
    }
}

int Typedef_Enum_DaysToWeekend(Typedef_Enum_TEweekdayType today) {
    if (today == SATURDAY || today == SUNDAY) {
        return 0;  
    }
    return SATURDAY - today;
}

int main() {
    Typedef_Enum_TEweekdayType today = WEDNESDAY;
    
    printf("Today is %s\n", Typedef_Enum_GetWeekday(today));
    printf("Days to weekend: %d\n", Typedef_Enum_DaysToWeekend(today));
    
    printf("\nAll weekdays:\n");
    for (Typedef_Enum_TEweekdayType d = MONDAY; d <= SUNDAY; d++) {
        printf("%s: %d days to weekend\n", 
               Typedef_Enum_GetWeekday(d), 
               Typedef_Enum_DaysToWeekend(d));
    }
    
    return 0;
}
