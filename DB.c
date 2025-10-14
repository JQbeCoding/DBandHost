/**
 * This file will be the database for the directory. I , by myself,
 * like an idiot, will manually create the query instructions and
 * how to handle the logic of this stuff. This will not only define
 * the structs but also read from and write to files to be used
 * within the employee site.
 *
 * @author Ja'Quis Franklin, Developer at MayDayz Smokn BBQ
 * @date 10/07/2025
 */

#include <stdio.h>
#include <stdlib.h>
#include "employee.h"

#define MAX_EMPLOYEES 200
#define PASSWORD_BUFFER 150

int main(void)
{

    // Testing functions
    struct Employee *employees = (struct Employee *)calloc(MAX_EMPLOYEES, sizeof(struct Employee));
    if (employees == NULL)
    {
        perror("Failed to allocate memory for employees.\n");
        return 1;
    }
    else
    {
        printf("Allocation succesful\n");
    }

    struct Employee employee1 = createEmployee("Ja'Quis", "Avshia", "Franklin", "704-967-5408", "jaquis.franklin@maydayz.com", "6500 Monteith Drive",
                                               "M-3455663", "Avashia100604!!", "10/06/04", "06/01/24", "Admin", "CTO", 334844843, 01234567, 56.673445, "000-01-9989");
    struct Employee employee2 = createEmployee("Lamar", "Jevon", "Henderson", "213-555-8192", "lamar.henderson@soxietytech.com", "8429 Sunset Blvd, Apt 12C", "HndrsonL001", "P@ssW0rd!93", "11/22/1993", "07/15/2022",
                                               "Engineer", "Backend Developer", 547829103, 10293847, 50.75, "000-47-8392");
    struct Employee employee3 = createEmployee("Lamar", "Jacob", "Green", "213-555-8192", "lamar.green@soxietytech.com", "9248 Sunrise Blvd, Apt 12A", "GreenL001", "P@ssW0rd!94", "11/22/2003", "07/15/2025",
                                               "Engineer", "Backend Developer", 547829103, 10293847, 50.75, "000-47-8392");

    *employees = employee1;
    *(employees + 1) = employee2;
    *(employees + 2) = employee3;

    size_t passcode_array_size = sizeof(employee1.passcode);

    char *passwords = (char *)calloc(MAX_EMPLOYEES, PASSWORD_BUFFER);
    if (passwords == NULL)
    {
        perror("Failed to allocate raw password buffer");
        free(employees);
        return 1;
    }
    for (int i = 0; i < 3; i++)
    {
        size_t raw_password_len = strlen(employees[i].passcode);
        memcpy(passwords + i * passcode_array_size, employees[i].passcode, passcode_array_size);
        *(passwords + i * passcode_array_size + raw_password_len) = '\0';
    }

    // adding employee with same name

    displayEmployees(employees, 10);
    searchEmployeeByName(employees);
    searchEmployeeByUsername(employees);
    char *current_raw_password = passwords + (0 * passcode_array_size);
    int password_hash = hashPasscode(current_raw_password, employee1.passcode, 188);
    displayEmployees(employees, 10);
    printf("\n");

    free(employees);
    free(passwords);
}
